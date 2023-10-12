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
	textureHandle_ = TextureManager::Load("Enemy.png");
	isDead_ = false;
	/*worldTransform_.scale_ = {10.0f, 10.0f, 10.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};*/
}

void Enemy::Draw(ViewProjection& viewProjection) {//描画
	if (isDead_ == false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
}

void Enemy::Update() {// 更新

	const float EnemySpeed = -0.3f;
	Vector3 move = {0, 0, EnemySpeed};
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	float EnemyPos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	// スケーリング行列を宣言
	Matrix4x4 matScale;
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1;

	Matrix4x4 rotateXMatrix = MakeRotateXmatrix(worldTransform_.rotation_.x);
	Matrix4x4 rotateYMatriy = MakeRotateYmatrix(worldTransform_.rotation_.y);
	Matrix4x4 rotateZMatriz = MakeRotateZmatrix(worldTransform_.rotation_.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatriy, rotateZMatriz));
	// 平行移動行列を宣言
	Matrix4x4 matTrans;
	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][3] = 1;
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

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
}

void Enemy::Approach() {
	
	
}