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

	unsigned int Time;
	// デスフラグ
	bool isDead_ = false;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle1_ = 0u;//真っ直ぐくるやつ
	uint32_t textureHandle2_ = 0u;//外回りするやつ
	uint32_t textureHandle3_ = 0u;//速度変化するやつ
	// キーボード入力
	Input* input_ = nullptr;

	// 乱数を生成して変数に代入
	int i = 0;
	float Spawnnumber = 0; // 出現パターン
	float SpawnTime = 0;   // 待機時間
	
	//敵の変数
	bool isStraight = false;//真っ直ぐくるやつ
	bool isOuter = false;//外回りするやつ
	bool isChange = false;//速度変化するやつ
	//滞在time
	int StayingTime = 0;

};