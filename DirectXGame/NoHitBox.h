﻿#pragma once
#include "Model.h"
#include "WorldTransform.h"

class NoHitBox {
public:
	/// <summary
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary
	/// 更新
	/// </summary>
	void Update();

	/// <summary
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

};
