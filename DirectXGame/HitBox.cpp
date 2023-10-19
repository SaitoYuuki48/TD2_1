#include "HitBox.h"
#include <cassert>
#include "ImGuiManager.h"

void HitBox::Initialize(Model* model) {
	// 引数から受け取ったモデルが読み込まれているかチェック
	assert(model);
	// 引数からモデルとテクスチャハンドルを受け取る
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// x,y,z方向のスケーリングを設定
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	// x,y,z方向の回転を設定
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	// x,y,zの方向のを設定
	worldTransform_.translation_ = {0.0f, 2.0f, -50.0f};
}

void HitBox::Update() {
	worldTransform_.UpdateMatrix();



#ifdef _DEBUG

	ImGui::Begin("hitbox");

	float hitPos[] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	ImGui::SliderFloat3("EnemyPos", hitPos, 50.0f, -50.0f);

	ImGui::End();

	worldTransform_.translation_.x = hitPos[0];
	worldTransform_.translation_.y = hitPos[1];
	worldTransform_.translation_.z = hitPos[2];

#endif //_DEBUG

}

void HitBox::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 HitBox::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
