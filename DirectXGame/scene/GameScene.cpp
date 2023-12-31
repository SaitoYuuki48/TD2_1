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

	//ゲームオーバーの画像追加
	uint32_t textureGameOver = TextureManager::Load("resources/overEnter.png");
	spriteGameOver_ =
	    Sprite::Create(textureGameOver, {0.0f, -140.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	//倒した敵の画像
	uint32_t textureKillEnemy = TextureManager::Load("resources/teki.png");
	spriteKillEnemy_=
	    Sprite::Create(textureKillEnemy, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	//ライフの画像
	//  テクスチャ
	uint32_t textureLife = TextureManager::Load("resources/Heart.png");

	//ハートの画像
	spriteLife_ = Sprite::Create(textureLife, {60.0f, 60.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	//スコアの画像
	//テクスチャ(0～9)
	uint32_t textureScore0 = TextureManager::Load("resources/0.png");
	uint32_t textureScore1 = TextureManager::Load("resources/1.png");
	uint32_t textureScore2 = TextureManager::Load("resources/2.png");
	uint32_t textureScore3 = TextureManager::Load("resources/3.png");
	uint32_t textureScore4 = TextureManager::Load("resources/4.png");
	uint32_t textureScore5 = TextureManager::Load("resources/5.png");
	uint32_t textureScore6 = TextureManager::Load("resources/6.png");
	uint32_t textureScore7 = TextureManager::Load("resources/7.png");
	uint32_t textureScore8 = TextureManager::Load("resources/8.png");
	uint32_t textureScore9 = TextureManager::Load("resources/9.png");

	spriteScore1_[0] =
	    Sprite::Create(textureScore0, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[1] =
	    Sprite::Create(textureScore1, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[2] =
	    Sprite::Create(textureScore2, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[3] =
	    Sprite::Create(textureScore3, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[4] =
	    Sprite::Create(textureScore4, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[5] =
	    Sprite::Create(textureScore5, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[6] =
	    Sprite::Create(textureScore6, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[7] =
	    Sprite::Create(textureScore7, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[8] =
	    Sprite::Create(textureScore8, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore1_[9] =
	    Sprite::Create(textureScore9, {100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	spriteScore2_[0] =
	    Sprite::Create(textureScore0, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[1] =
	    Sprite::Create(textureScore1, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[2] =
	    Sprite::Create(textureScore2, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[3] =
	    Sprite::Create(textureScore3, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[4] =
	    Sprite::Create(textureScore4, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[5] =
	    Sprite::Create(textureScore5, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[6] =
	    Sprite::Create(textureScore6, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[7] =
	    Sprite::Create(textureScore7, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[8] =
	    Sprite::Create(textureScore8, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore2_[9] =
	    Sprite::Create(textureScore9, {50.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	spriteScore3_[0] =
	    Sprite::Create(textureScore0, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[1] =
	    Sprite::Create(textureScore1, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[2] =
	    Sprite::Create(textureScore2, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[3] =
	    Sprite::Create(textureScore3, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[4] =
	    Sprite::Create(textureScore4, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[5] =
	    Sprite::Create(textureScore5, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[6] =
	    Sprite::Create(textureScore6, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[7] =
	    Sprite::Create(textureScore7, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[8] =
	    Sprite::Create(textureScore8, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteScore3_[9] =
	    Sprite::Create(textureScore9, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	//リザルト
	spriteResult1_[0] =
	    Sprite::Create(textureScore0, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[1] =
	    Sprite::Create(textureScore1, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[2] =
	    Sprite::Create(textureScore2, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[3] =
	    Sprite::Create(textureScore3, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[4] =
	    Sprite::Create(textureScore4, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[5] =
	    Sprite::Create(textureScore5, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[6] =
	    Sprite::Create(textureScore6, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[7] =
	    Sprite::Create(textureScore7, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[8] =
	    Sprite::Create(textureScore8, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult1_[9] =
	    Sprite::Create(textureScore9, {660.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	spriteResult2_[0] =
	    Sprite::Create(textureScore0, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[1] =
	    Sprite::Create(textureScore1, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[2] =
	    Sprite::Create(textureScore2, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[3] =
	    Sprite::Create(textureScore3, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[4] =
	    Sprite::Create(textureScore4, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[5] =
	    Sprite::Create(textureScore5, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[6] =
	    Sprite::Create(textureScore6, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[7] =
	    Sprite::Create(textureScore7, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[8] =
	    Sprite::Create(textureScore8, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult2_[9] =
	    Sprite::Create(textureScore9, {610.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	spriteResult3_[0] =
	    Sprite::Create(textureScore0, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[1] =
	    Sprite::Create(textureScore1, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[2] =
	    Sprite::Create(textureScore2, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[3] =
	    Sprite::Create(textureScore3, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[4] =
	    Sprite::Create(textureScore4, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[5] =
	    Sprite::Create(textureScore5, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[6] =
	    Sprite::Create(textureScore6, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[7] =
	    Sprite::Create(textureScore7, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[8] =
	    Sprite::Create(textureScore8, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	spriteResult3_[9] =
	    Sprite::Create(textureScore9, {560.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});


	//パンチのSE
	panchiSoundHandle_ = audio_->LoadWave("se/panchi.mp3");
	panchiAiSoundHandle_ = audio_->LoadWave("se/panchi2.mp3");

	//爆発のSE
	explosionSeHandle_ = audio_->LoadWave("se/explosion.mp3");

	// BGM
	bgmDataHandle_ = audio_->LoadWave("BGM/BGM.mp3");
	bgmHandle_ = audio_->PlayWave(bgmDataHandle_, true, 0.15f);

	// ペースアップテクスチャ
	PaseUptextureHandle_ = TextureManager::Load("resources/SpeedUp.png");
	isPaseUp = false;

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

	ScoreUpdate();

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

	//ペースアップ
	PaseUpTimer++;

	

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
		isGameOver = true;

	} else {
		// 敵の発生間隔
		SpawnInterval();
	
		//ランダムな種類の敵の発生
		RandSpawn();
	}

#ifdef _DEBUG
	ImGui::Begin("Player");

	ImGui::Text("%d", playerLife_);
	ImGui::Text("%d", enemyDefeats_);
	ImGui::Text("%f", spawnInterval);

	ImGui::End();
#endif // _DEBUG
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

	// ペースアップの描画
	if (isPaseUp == true&&PaseUpTimer>=30) {
		PaseUpsprite_->Draw();
	}
	
	// スプライトの生成
	PaseUpsprite_ = Sprite::Create(PaseUptextureHandle_, {0, -85});

	
	if (isGameOver == false) {
		// ハートの描画
		//spriteLife_->Draw();

		// スコアの画像
		spriteScore1_[score1]->Draw();
		spriteScore2_[score2]->Draw();
		spriteScore3_[score3]->Draw();
	} else {
		//ここにゲームオーバー時の画像描画を追加する
		spriteGameOver_->Draw();
		spriteKillEnemy_->Draw();
		spriteResult1_[result1]->Draw();
		spriteResult2_[result2]->Draw();
		spriteResult3_[result3]->Draw();
	}

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
				score++;
				seHandle_ = audio_->PlayWave(panchiSoundHandle_, false, 1.0f);
			} else if (cheakPanchi == 2 &&attackType == 1) { // ここでパンチと敵の種類が合ってたら消すようにする
				enemy->OnCollision();
				enemyDefeats_++;
				score++;
				seHandle_ = audio_->PlayWave(panchiAiSoundHandle_, false, 1.0f);
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

void GameScene::ScoreUpdate() { 
	score1 = (score / 1) % 10;//1の桁
	score2 = (score / 10) % 10;//10の桁
	score3 = (score / 100) % 10;//100の桁

	result1 = (score / 1) % 10;   // 1の桁
	result2 = (score / 10) % 10;  // 10の桁
	result3 = (score / 100) % 10; // 100の桁
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
		isPaseUp = true;
		
	}
	if (timer == 1690) {
		spawnInterval = 50; // 50
	}
	if (timer == 2000) {
		spawnInterval = 40; // 40
	}
	if (timer == 2400) {
		spawnInterval = 36; // 40
	}
	if (timer == 2800) {
		spawnInterval = 34; // 40
	}
}

void GameScene::sceneReset() {
	//シーンの切り替えフラグ
	isSceneEnd = false;
	
	for (Enemy* enemy : enemys_) {
		enemy->OnCollision();
	}
	//ゲームオーバーのフラグを初期化
	isGameOver = false; 
	
	// プレイヤーの体力
	playerLife_ = kPLAYERLIFE_;



	// 敵を倒した数
	enemyDefeats_ = 0;
	score = 0;

	isPaseUp = false;
	PaseUpTimer = 0;

	//経過時間
	timer = 0;
	//発生の間隔
	spawnInterval = 120;
	// BGMの停止
	audio_->StopWave(bgmHandle_);
	bgmHandle_ = audio_->PlayWave(bgmDataHandle_, true, 0.15f);
}