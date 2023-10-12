#include"Enemy.h"
#include <cassert>


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
	Vector3 Speed = {0, 0, -100.4f};
	/*worldTransform_.translation_ = Add(worldTransform_.translation_, leave);*/
}

void Enemy::Approach() {
	
	
}