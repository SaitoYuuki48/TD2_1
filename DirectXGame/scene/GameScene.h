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

	//スコアの更新処理
	void ScoreUpdate();


	//敵のリスト
	std::list<Enemy*> enemys_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// シーンのリセット
	void sceneReset();

	bool isSceneEnd = false;

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() { return SceneType::kTitle; }

private:
	void SpawnInterval();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

private:

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//ペースアップ
	uint32_t PaseUptextureHandle_ = 0;
	
	// スプライト
	Sprite* sprite_ = nullptr;
	// ペースアップスプライト
	Sprite* PaseUpsprite_ = nullptr;

	//ペースアップ変数
	int PaseUpTimer = 0;
	bool isPaseUp = false;

	// モデル
	Model* model_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_; 

	// カメラ
	std::unique_ptr<Camera> camera_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;
	//プレイヤーの体力
	const int kPLAYERLIFE_ = 5;
	int playerLife_ = kPLAYERLIFE_;

	//敵を倒した数
	int enemyDefeats_ = 0;

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

	// ハートの画像
	Sprite* spriteLife_ = nullptr;

	//スコアの画像
	Sprite* spriteScore1_[10] = {nullptr};
	Sprite* spriteScore2_[10] = {nullptr};
	Sprite* spriteScore3_[10] = {nullptr};

	//リザルトのスコアの描画
	Sprite* spriteResult1_[10] = {nullptr};
	Sprite* spriteResult2_[10] = {nullptr};
	Sprite* spriteResult3_[10] = {nullptr};

	//スコアのメンバ変数
	int score = 0;
	int score1 = 0;
	int score2 = 0;
	int score3 = 0;

	int result1 = 0;
	int result2 = 0;
	int result3 = 0;

	//ゲームオーバーの画像
	Sprite* spriteGameOver_ = nullptr;
	Sprite* spriteKillEnemy_ = nullptr;

	//BGM
	uint32_t bgmDataHandle_ = 0;
	uint32_t bgmHandle_ = 0;

	//パンチのSE
	uint32_t panchiSoundHandle_ = 0;

	uint32_t panchiAiSoundHandle_ = 0;

	//爆発のSE
	uint32_t explosionSeHandle_ = 0;

	//音声再生ハンドル
	uint32_t seHandle_ = 0;

	//ヒットボックス描画切り替え
	bool changeHitbox = false;

	//乱数を使用するための変数
	//  乱数を生成して変数に代入
	int i = 0;
	float spawnTime = 0;

	//敵の発生間隔
	float spawnInterval = 120;

	//経過した時間
	float timer = 0;

	//ゲームオーバーかどうかのフラグ
	bool isGameOver = false;
	
};
