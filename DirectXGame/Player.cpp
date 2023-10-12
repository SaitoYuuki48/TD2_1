#include <stdio.h>
#include "Player.h"
#include "compute.h"
#include <cassert>
#include "ImGuiManager.h"
#include "math.h"

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
	panchiFlag = false;//溜めているかのフラグ
	//長短押しのフラグ
	shortFlag = false;
	longFlag = false;
}

void Player::Update() {
	worldTransform_.UpdateMatrix();

	//ボタンを短押しと長押しでアニメーションさせる
	if (input_->PushKey(DIK_SPACE)) {
		chargeTime++;
		panchiFlag = true;//パンチのフラグ
	} else {
	
		panchiFlag = false;

	 if (chargeTime >= 1 && chargeTime <= 8) {
			shortFlag = true;
	 } else if (chargeTime >= 9) {
			longFlag = true;
	 }
	
	}

	//長押ししているときのモーション
	if (panchiFlag == true) {
	 //溜めているときの腕の速度
	 Vector3 charge = {0.0f, -0.04f, 0.0f};
	 // 手が下に行き過ぎないようにするif文
	 if (worldTransform_.translation_.y <= -18.0f) {
			charge = {0.0f, 0.0f, 0.0f};
	 }
	 //手の位置の更新処理
	 worldTransform_.translation_ = Add(worldTransform_.translation_,charge);

	} else {
	//溜めてないときは初期位置に座標を設定(後でconstで設定するといいかも)
	 worldTransform_.translation_ = {16.0f, -15.0f, -5.0f};
	}

	

	//短押しの挙動
	if (shortFlag == true) {
	
	}

	//長押しの挙動
	if (longFlag == true) {
	
	}

#ifdef _DEBUG

	ImGui::Begin("Player");

	ImGui::Text("%d", chargeTime);
	ImGui::Text("SHORT%lf:LONG%lf", shortFlag,longFlag);

	ImGui::End();

#endif //_DEBUG


	////行列を定数バッファに転送
	//worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}