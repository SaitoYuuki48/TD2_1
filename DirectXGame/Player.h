#pragma once
#include <stdio.h>
#include "model.h"
#include "WorldTransform.h"
#include "Input.h"
#include <Sprite.h>

class Player {

public:

	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	int CheakPanchi();

	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	

	private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//プログラムで使う変数
	int chargeTime ;//溜めてる時間を判定するタイマー
	bool panchiFlag ;//溜めてるかのフラグ

	int cheakPanchi;

	int panchiTimer;

	int PlayerHP=3;
};
