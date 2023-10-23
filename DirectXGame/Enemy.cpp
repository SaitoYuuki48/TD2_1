#include"Enemy.h"
#include <cassert>
#include "compute.h"
#include "ImGuiManager.h"


void Enemy::Initialize(Model* model) { // 初期化
	// NULLポインタチェック
	assert(model);

	// 引数からデータを受け取る
	model_ = model;
	

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// スケール
	Vector3 Scale = {4, 4, 4};
	worldTransform_.scale_ = Scale;
	textureHandle1_ = TextureManager::Load("resources/Enemy.png");//真っ直ぐくるやつ
	textureHandle2_ = TextureManager::Load("resources/Red.png");//外回りするやつ
	textureHandle3_ = TextureManager::Load("resources/Green.png");//速度変化するやつ
	isDead_ = false;
	StayingTime = 0;
	
	// 乱数の初期化(シード値の設定)
	Time = static_cast<unsigned int>(time(nullptr));
	srand((unsigned)time(NULL));

}

void Enemy::Draw(ViewProjection& viewProjection) {//描画
	if (isDead_ == false) {
		if (isStraight == true)
		{
			model_->Draw(worldTransform_, viewProjection, textureHandle1_);//真っ直ぐくるやつ
		}
		if (isOuter == true) {
			model_->Draw(worldTransform_, viewProjection, textureHandle2_);//外回りするやつ
		}
		if (isChange == true) {
			model_->Draw(worldTransform_, viewProjection, textureHandle3_);//速度変化するやつ
		}
	}
}


void Enemy::Update() {//// 更新
	isStraight = true;
#pragma region 移動変数
	float UpMoveSpeed = 0.5f;
	float DownMoveSpeed = -0.9f;
#pragma endregion

	if (isStraight == true) {
		//滞在時間
		StayingTime++;
		//移動
		Vector3 move = {0, 0, -0.5f};
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		//回転
		Vector3 Rotate = {0.5f, 0.5f,0.3f};
		worldTransform_.rotation_ = Add(worldTransform_.rotation_, Rotate);
		

		worldTransform_.matWorld_ = MakeAffineMatrix(
		    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);


		// 上に行く
		if (worldTransform_.translation_.z <= -5 && worldTransform_.translation_.z > -30) {
			worldTransform_.translation_.y = worldTransform_.translation_.y+UpMoveSpeed;
		}
		//下に降りる
		if (worldTransform_.translation_.z < -30 && worldTransform_.translation_.y >= 0) {
			worldTransform_.translation_.y = worldTransform_.translation_.y + DownMoveSpeed;
		}
		worldTransform_.TransferMatrix();


	}

	if (isOuter == true)
	{
		// 滞在時間
		StayingTime++;
		// 移動
		Vector3 move = {0, 0, -0.5f};
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		
		// 回転
		Vector3 Rotate = {0.5f, 0.5f, 0.3f};
		worldTransform_.rotation_ = Add(worldTransform_.rotation_, Rotate);

		worldTransform_.matWorld_ = MakeAffineMatrix(
		    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);


		// 右に行く
		if (worldTransform_.translation_.z <= -5 && worldTransform_.translation_.z > -30) {
			worldTransform_.translation_.y = worldTransform_.translation_.y + UpMoveSpeed;
		}
		//左へいく

		

		worldTransform_.TransferMatrix();
	}

	if (isChange == true)
	{
		// 滞在時間
		StayingTime++;
		// 移動
		Vector3 move = {0, 0, -0.5f};
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);

		// 回転
		Vector3 Rotate = {0.5f, 0.5f, 0.3f};
		worldTransform_.rotation_ = Add(worldTransform_.rotation_, Rotate);

		worldTransform_.matWorld_ = MakeAffineMatrix(
		    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

		// 右に行く
		if (worldTransform_.translation_.z <= -5 && worldTransform_.translation_.z > -30) {
			worldTransform_.translation_.y = worldTransform_.translation_.y + UpMoveSpeed;
		}
		// 左へいく

		worldTransform_.TransferMatrix();
	}



	
	float EnemyDebug[] = {
	    static_cast<float>(SpawnTime), static_cast<float>(Spawnnumber)};
	float EnemyPos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::SliderFloat3("EnemyPos", EnemyPos, 100.0f, -100.0f);
	ImGui::End();

	worldTransform_.translation_.x = EnemyPos[0];
	worldTransform_.translation_.y = EnemyPos[1];
	worldTransform_.translation_.z = EnemyPos[2];

#endif //_DEBUG

	

#ifdef _DEBUG
	

	ImGui::Begin("EnemySpawn");
	ImGui::Text("%f,%f", EnemyDebug,Spawnnumber);
	ImGui::Text("%f\n,%f", EnemyDebug,SpawnTime);
	ImGui::End();
	SpawnTime = static_cast<float>(EnemyDebug[0]);
	Spawnnumber = static_cast<float> (EnemyDebug[1]);
	

	
#endif //_DEBUG
	//リスポーン処理
	if (StayingTime>=150) {
		isDead_ = true;
		StayingTime = 0;
	}


	//敵のランダム生成処理
	if (isDead_ == true)
	{
		worldTransform_.translation_.z = 4;
		worldTransform_.translation_.y = 0;
		Spawnnumber = static_cast<float>(rand());
		Spawnnumber = static_cast<float>(rand() % 3 + 1); 
		
		//待機時間
		SpawnTime++;
		if (SpawnTime > 120 && Spawnnumber == 1)
		{
			isStraight = true;
			isDead_ = false;
			SpawnTime = 0;
		}
		if (SpawnTime > 120 && Spawnnumber == 2) {
			isOuter = true;
			isDead_ = false;
			SpawnTime = 0;
		}
		if (SpawnTime > 120 && Spawnnumber == 3) {
			isChange = true;
			isDead_ = false;
			SpawnTime = 0;
		}
	}


}


void Enemy::OnCollision() { 
	isDead_ = true;
	StayingTime = 0;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
