#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	// 自キャラの解放
	delete player_;
	
	// 天球の解放
	delete skydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("resources/Arm.png"); // プレイヤー
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
	// enemy_ = new Enemy();
	// 敵の初期化
	// enemy_->Initialize(model_);

	// ヒットボックス
	// 3Dモデルの生成
	modelHitBox_.reset(Model::CreateFromOBJ("hitBox", true));
	modelHitBox_->SetMaterialAlpha("hitBox", 0.3f);
	// ヒットボックスの生成
	hitBox_ = std::make_unique<HitBox>();
	// ヒットボックスの初期化
	hitBox_->Initialize(modelHitBox_.get());

	// 当たっていないときのヒットボックス
	modelNoHitBox_.reset(Model::CreateFromOBJ("NoHitBox", true));
	modelNoHitBox_->SetMaterialAlpha("NoHitBox", 0.2f);
	// 生成
	noHitBox_ = std::make_unique<NoHitBox>();
	// 初期化
	noHitBox_->Initialize(modelNoHitBox_.get());

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

	// カメラの生成
	camera_ = std::make_unique<Camera>();
	// カメラの初期化
	camera_->Initialize();

	//パンチのSE
	panchiSoundHandle_ = audio_->LoadWave("se/panchi.mp3");

	//爆発のSE
	explosionSeHandle_ = audio_->LoadWave("se/explosion.mp3");

	// BGM
	bgmDataHandle_ = audio_->LoadWave("BGM/BGM.mp3");
	bgmHandle_ = audio_->PlayWave(bgmDataHandle_, true, 0.15f);

	// デバッグカメラの更新
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

#ifdef _DEBUG

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif // _DEBUG

	// 敵の生成
	EnemySpawn();
}

void GameScene::Update() {

	// カメラの更新
	camera_->Update();
	// デバッグカメラの更新
	debugCamera_->Update();
	//終わり後シーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		isSceneEnd = true;
	}

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_C) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	}

	if (input_->TriggerKey(DIK_V) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}
#endif // _DEBUG

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

	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	

	// 天球の更新
	skydome_->Update();
	// 床の更新
	ground_->Update();
	// ヒットボックス
	hitBox_->Update();
	noHitBox_->Update();

	

	// 当たり判定
	GameScene::CheakAllCollisions();

	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	if (playerLife_ <= 0) {

	} else {
		// 敵の発生間隔
		SpawnInterval();
	
		//ランダムな種類の敵の発生
		RandSpawn();
	}

	ImGui::Begin("Player");

	ImGui::Text("%d", playerLife_);
	ImGui::Text("%d", enemyDefeats_);
	ImGui::Text("%f", spawnInterval);

	ImGui::End();
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

	// 天球の描画
	skydome_->Draw(viewProjection_);

	// 床の描画
	ground_->Draw(viewProjection_);

	// 敵キャラの描画
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	if (playerLife_ >= 0) {
		// 自キャラの描画
		player_->Draw(viewProjection_);

		// ヒットボックス
		if (changeHitbox == false) {
			noHitBox_->Draw(viewProjection_);
		} else if (changeHitbox == true) {
			hitBox_->Draw(viewProjection_);
		}
	}
	

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
	// 変数の宣言
	Vector3 vecPlayer, vecEnemy;

	// ２間点の距離
	float posAB;

	// 敵の半径
	float enemyRadius = 7.0f;

	// 殴る当たり判定の半径
	float hitRadius = 4.0f;

	// パンチの種類を判別する変数
	int cheakPanchi;

	//敵の種類
	int attackType;

	//関数での代入
	cheakPanchi = player_->CheakPanchi();

	// 当たったカウント
	int hitCount = 0; 

	// 敵の座標を変数に入れる
	for (Enemy* enemy : enemys_) {
		vecEnemy = enemy->GetWorldPosition();
	    attackType = enemy->GetType();
		
		// ヒットボックスの座標を変数に入れる
		vecPlayer = hitBox_->GetWorldPosition();

		// ２間点の距離を求める
		posAB = (vecPlayer.x - vecEnemy.x) * (vecPlayer.x - vecEnemy.x) +
		        (vecPlayer.y - vecEnemy.y) * (vecPlayer.y - vecEnemy.y) +
		        (vecPlayer.z - vecEnemy.z) * (vecPlayer.z - vecEnemy.z);

		if (posAB <= (hitRadius + enemyRadius) * (hitRadius + enemyRadius)) {
			
			hitCount++;

			if (cheakPanchi == 1&& attackType == 0) { // ここでパンチと敵の種類が合ってたら消すようにする
				enemy->OnCollision();
				enemyDefeats_++;
				seHandle_ = audio_->PlayWave(panchiSoundHandle_, false);
			} else if (cheakPanchi == 2 &&attackType == 1) { // ここでパンチと敵の種類が合ってたら消すようにする
				enemy->OnCollision();
				enemyDefeats_++;
				seHandle_ = audio_->PlayWave(panchiSoundHandle_, false);
			} else if (cheakPanchi != 0 &&attackType == 2) { // パンチしてはいけないときプレイヤーのライフを減らす
				enemy->OnCollision();
				playerLife_--;
				seHandle_ = audio_->PlayWave(explosionSeHandle_, false);
			} 
		} 

		//倒せなかったときの処理
		if (attackType != 2 && vecEnemy.z == -80) {
			playerLife_--;
		}
	}
	if (hitCount != 0) {
		changeHitbox = true; // ヒットボックス色切り替え
	} else {
		changeHitbox = false; // ヒットボックス色切り替え
	}
}

void GameScene::EnemySpawn() {
	Enemy* enemy = new Enemy();
	// 初期化
	enemy->Initialize(model_);
	int moveType = (rand() % 3);
	int attackType = (rand() % 3);

	enemy->SetType(moveType, attackType);

	// リストに登録
	enemys_.push_back(enemy);
}

void GameScene::RandSpawn() {
	// 敵キャラの更新
	spawnTime++;
	if (spawnTime > spawnInterval) {
		EnemySpawn();
		spawnTime = 0;
	}
}

void GameScene::SpawnInterval() {
	timer++;
	if (timer == 360) {
		spawnInterval = 110; //110
	}
	if (timer == 720) {
		spawnInterval = 100; //100
	}
	if (timer == 1080) {
		spawnInterval = 90; //90
	}
	if (timer == 1320) {
		spawnInterval = 80; // 80
	}
	if (timer == 1500) {
		spawnInterval = 70; // 70
	}
	if (timer == 1560) {
		spawnInterval = 60; // 60
	}
	if (timer == 1690) {
		spawnInterval = 50; // 50
	}
	if (timer == 2000) {
		spawnInterval = 45; // 40
	}
}

void GameScene::sceneReset() {
	//シーンの切り替えフラグ
	isSceneEnd = false;
	
	for (Enemy* enemy : enemys_) {
		enemy->OnCollision();
	}
	// プレイヤーの体力
	playerLife_ = kPLAYERLIFE_;

	// 敵を倒した数
	enemyDefeats_ = 0;

	//経過時間
	timer = 0;
	//発生の間隔
	spawnInterval = 120;
	// BGMの停止
	audio_->StopWave(bgmHandle_);
	bgmHandle_ = audio_->PlayWave(bgmDataHandle_, true, 0.15f);
}