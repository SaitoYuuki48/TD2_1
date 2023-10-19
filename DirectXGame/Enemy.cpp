#include"Enemy.h"
#include <cassert>
#include "compute.h"
#include "ImGuiManager.h"


void Enemy::Initialize(Model* model, uint32_t textureHandle) { // 初期化
	// NULLポインタチェック
	assert(model);

	// 引数からデータを受け取る
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	textureHandle_ = TextureManager::Load("resources/Enemy.png");
	isDead_ = false;
}

void Enemy::Draw(ViewProjection& viewProjection) {//描画
	if (isDead_ == false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
}


void Enemy::Update() {//// 更新
#pragma region 移動変数
	const float EnemySpeedX = 0.0f;
	const float EnemySpeedY = 0.0f;
	const float EnemySpeedZ = -0.2f;
	float UpMoveSpeed = 0.07f;
	float DownMoveSpeed = -0.1f;
#pragma endregion
#pragma region 回転変数
	const float RotateSpeedX = 15.0f;
	const float RotateSpeedY = 15.0f;
	const float RotateSpeedZ = 0.0f;
#pragma endregion
	//移動
	Vector3 move = {EnemySpeedX, EnemySpeedY, EnemySpeedZ};
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	float EnemyPos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	//回転
	Vector3 Rotate = {RotateSpeedX, RotateSpeedY, RotateSpeedZ};
	worldTransform_.rotation_ = Add(worldTransform_.translation_, Rotate);

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();


#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::SliderFloat3("EnemyPos", EnemyPos, 100.0f, -100.0f);
	ImGui::End();

	worldTransform_.translation_.x = EnemyPos[0];
	worldTransform_.translation_.y = EnemyPos[1];
	worldTransform_.translation_.z = EnemyPos[2];

#endif //_DEBUG

	// 上に行く
	if (worldTransform_.translation_.z <= -5 && worldTransform_.translation_.z > -30) {
		worldTransform_.translation_.y = worldTransform_.translation_.y+UpMoveSpeed;
	}
	//下に降りる
	if (worldTransform_.translation_.z < -30 && worldTransform_.translation_.y >= 0) {
		worldTransform_.translation_.y = worldTransform_.translation_.y + DownMoveSpeed;
	}

}


void Enemy::OnCollision() { isDead_ = true; }

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
