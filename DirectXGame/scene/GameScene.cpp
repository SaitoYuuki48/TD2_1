#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	// 自キャラの解放
	delete player_;
	//敵キャラの解放
	delete enemy_;
	//天球の解放
	delete skydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png"); // プレイヤー
	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// モデル生成
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// 敵の生成
	enemy_ = new Enemy();
	// 敵の初期化
	enemy_->Initialize(model_, textureHandle_);

	// ヒットボックス
	// 3Dモデルの生成
	modelHitBox_.reset(Model::CreateFromOBJ("hitBox", true));
	modelHitBox_->SetMaterialAlpha("hitBox", 0.3f);
	// ヒットボックスの生成
	hitBox_ = std::make_unique<HitBox>();
	// ヒットボックスの初期化
	hitBox_->Initialize(modelHitBox_.get());

	// 地面
	// 3Dモデルの生成
	modelGround_.reset(Model::CreateFromOBJ("Ground", true));
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	// 地面の初期化
	ground_->Initialize(modelGround_.get());

	// 天球のモデル
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	//カメラの初期化
	camera_->Initialize();

#ifdef _DEBUG


	// デバッグカメラの更新
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif // _DEBU
}

void GameScene::Update() {

	// カメラの更新
	camera_->Update();
	//デバッグカメラの更新
	debugCamera_->Update();
	
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_C) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	}

	if (input_->TriggerKey(DIK_V) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}

	// カメラの処理
	if (isDebugCameraActive_ == true) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = camera_->GetViewProjection().matView;
		viewProjection_.matProjection = camera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
		
	}
#endif // _DEBUG

	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	enemy_->Update();
	//天球の更新
	skydome_->Update();
	//床の更新
	ground_->Update();
	// ヒットボックス
	hitBox_->Update();
	
	//当たり判定
	GameScene::CheakAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	enemy_->Draw(viewProjection_);

	//天球の描画
	skydome_->Draw(viewProjection_);

	//床の描画
	ground_->Draw(viewProjection_);

	//ヒットボックス
	hitBox_->Draw(viewProjection_);

	/*for (Enemy* enemy : enemys_) {
	    enemy->Draw(viewProjection_);
	}*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheakAllCollisions() { 
	//変数の宣言
	Vector3 vecPlayer, vecEnemy;

	//２間点の距離
	float posAB;

	//敵の半径
	float enemyRadius = 4.0f;

	//殴る当たり判定の半径
	float hitRadius = 4.0f;

	//敵の座標を変数に入れる
	vecEnemy = enemy_->GetWorldPosition();

	//ヒットボックスの座標を変数に入れる
	vecPlayer = hitBox_->GetWorldPosition();

	//２間点の距離を求める
	posAB = (vecPlayer.x - vecEnemy.x) * (vecPlayer.x - vecEnemy.x) +
	        (vecPlayer.y - vecEnemy.y) * (vecPlayer.y - vecEnemy.y) +
	        (vecPlayer.z - vecEnemy.z) * (vecPlayer.z - vecEnemy.z);

	if (posAB <= (hitRadius + enemyRadius) * (hitRadius + enemyRadius)) {
		enemy_->OnCollision();

	}

}
