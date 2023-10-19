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
	cheakPanchi = 0;//数字版のフラグ

	//タイマー関連
	panchiTimer = 0;

	//HP初期化
	PlayerHP = 3;

}

void Player::Update() {
	worldTransform_.UpdateMatrix();

	//ボタンを短押しと長押しでアニメーションさせる
	if (input_->PushKey(DIK_SPACE)) {
		chargeTime++;
		panchiFlag = true;//パンチのフラグ
	} else {
	
		panchiFlag = false;
		//タイマーでパンチの種類を判定
	 if (chargeTime >= 1 && chargeTime <= 8) {
		 cheakPanchi=1;
		 //cheakPanchi = 0;   
		 chargeTime = 0;
	 } else if (chargeTime >= 9) {
			cheakPanchi=2;
		 chargeTime = 0;
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
	////溜めてないときは初期位置に座標を設定(後でconstで設定するといいかも)
	 worldTransform_.translation_ = {16.0f, -15.0f, -5.0f};
	}

	if (cheakPanchi != 0) {
	 panchiTimer++;
	 if (panchiTimer > 1) {
		panchiTimer = 0;
		cheakPanchi = 0;
	 }
	}

	

	////短押しの挙動
	//if (shortFlag == true) {
	// cheakPanchi = 1;
	// if (shortFlag == true && cheakPanchi == 1) {
	//	shortFlag = false;
	//	cheakPanchi = 0;
  	// }
	//}
	//
	////長押しの挙動
	//if (longFlag == true) {
	// cheakPanchi = 2;
	// if (longFlag == true && cheakPanchi == 2) {
	//		longFlag = false;
	//		cheakPanchi = 0;
	// }
	//}

#ifdef _DEBUG

	ImGui::Begin("Player");

	ImGui::Text("%d", cheakPanchi);
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

int Player::CheakPanchi() {
	int flag;

	flag = cheakPanchi;

	return flag;
}

void Player::OnCollision() {
	PlayerHP -= 1;
	if (PlayerHP >= 0)
	{

	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}