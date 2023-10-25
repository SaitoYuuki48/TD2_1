#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include <memory>

#include "Player.h"
#include "Enemy.h"
#include "Scene.h"
#include "Skydome.h"
#include "Ground.h"
#include "HitBox.h"
#include "NoHitBox.h"
#include "Camera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//敵にパンチが当たってるか判定する関数
	void CheakAllCollisions();

	//敵の複数発生
	void EnemySpawn();

	void RandSpawn();

	


	//敵のリスト
	std::list<Enemy*> enemys_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	bool isSceneEnd = false;

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() { return SceneType::kTitle; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	Sprite* sprite_ = nullptr;
	// モデル
	Model* model_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_; 

	// フォローカメラ
	std::unique_ptr<Camera> camera_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;

	//ヒットボックス(当たり判定)
	std::unique_ptr<HitBox> hitBox_;
	//ヒットボックス(当たり判定)のモデル
	std::unique_ptr<Model> modelHitBox_;

	//当たっていないときのヒットボックス
	std::unique_ptr<NoHitBox> noHitBox_;
	// ヒットボックス(当たり判定)のモデル
	std::unique_ptr<Model> modelNoHitBox_;

	//スカイドーム
	Skydome* skydome_ = nullptr;
	// 天球の3Dモデル
	Model* modelSkydome_ = nullptr;//

	// 地面
	std::unique_ptr<Ground> ground_;
	// 3Dモデル
	std::unique_ptr<Model> modelGround_;

	//ヒットボックス描画切り替え
	bool changeHitbox=false;

	//乱数を使用するための変数
	//  乱数を生成して変数に代入
	int i = 0;
	float SpawnTime = 0;

	unsigned int Time;

};
