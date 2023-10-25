#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include<stdlib.h>
#include<time.h>

#include <list>


class Enemy {
public:
	////デストラクタ
	//~Enemy();

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void OnCollision();

	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }



	//
	//unsigned int Time;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_[3] = {0u,0u,0u};
	// キーボード入力
	Input* input_ = nullptr;

	// デスフラグ
	bool isDead_ = false;
	//// 乱数を生成して変数に代入
	int i = 0;
	float number = 0;
	float SpawnTime = 0;
	bool Straight = false; // 真っ直ぐ飛んでくる
	bool Outer = false;    // 外回り
	bool Change = false;   // 変換
	int Staytime = 0;//Stay

};