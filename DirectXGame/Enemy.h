#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"


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