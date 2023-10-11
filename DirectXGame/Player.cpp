﻿#include <stdio.h>
#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

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

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//プログラムで使う変数の宣言
	chargeTime = 0;
}

void Player::Update() {
	worldTransform_.UpdateMatrix();

	//ボタンを短押しと長押しでアニメーションさせる
	if (input_->PushKey(DIK_SPACE)) {
		chargeTime++;
	} else {
	
	 if (chargeTime >= 1 && chargeTime <= 8) {
			chargeTime = 5000;
	 } else if (chargeTime >= 9) {
			chargeTime = 8000;
	 }
	
	}

	//test

#ifdef _DEBUG

	ImGui::Begin("Player");

	ImGui::Text("%d", chargeTime);

	ImGui::End();

#endif //_DEBUG


	////行列を定数バッファに転送
	//worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}