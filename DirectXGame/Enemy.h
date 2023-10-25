#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <stdlib.h>
#include <time.h>

#include <list>

class Enemy {
private:


public:
	enum MoveType {
		STRAIGHT,//ストレート(0)　水
		RIGHT,//右回り(1)　緑
		LEFT,//左回り(2) 紫
	};

	enum AttackType {
		SHORT,//0
		LONG,//1
		THROUGH,//2
	};
	////デストラクタ
	//~Enemy();

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void OnCollision();

	void SetType(int moveType,int attackType);

    int GetType();


	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

private:
	void StraightUpdate();

	void RightUpdate();

	void LeftUpdate();

private:
	//// 乱数を生成して変数に代入
	int i = 0;
	float number = 0;
	float SpawnTime = 0;
	bool Straight = true; // 真っ直ぐ飛んでくる
	bool Outer = false;   // 外回り
	bool Change = false;  // 変換
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_[3] = {0u, 0u, 0u};
	// キーボード入力
	Input* input_ = nullptr;

	// デスフラグ
	bool isDead_ = false;
	//速度
	float EnemySpeedZ = 0.0f;
	float UpMoveSpeed = 0.0f;
	float DownMoveSpeed = 0.0f;
	//回転速度
	const float RotateSpeedX = 0.0f;
	const float RotateSpeedY = 0.0f;

	MoveType moveType_;
	AttackType attackType_;

};