#include <stdio.h>
#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);

	//引数からデータを受け取る
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.scale_ = {3.0f, 8.0f, 3.0f};
	worldTransform_.rotation_ = {0.5f, -0.1f,-0.2f};
	worldTransform_.translation_ = {16.0f, -15.0f, -5.0f};
}

void Player::Update() {
	worldTransform_.UpdateMatrix();
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}