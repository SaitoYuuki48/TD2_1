#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include<stdlib.h>
#include<time.h>


class Enemy {
public:
	////デストラクタ
	//~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void OnCollision();

	Vector3 GetWorldPosition();

	// デスフラグ
	bool isDead_ = false;

	// 乱数を生成して変数に代入
	int i = 0;
	float number = 0;
	float SpawnTime = 0;
	/*unsigned int Time;*/
	


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;


};