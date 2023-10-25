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
	textureHandle_[0] = TextureManager::Load("resources/Enemy.png");
	textureHandle_[1] = TextureManager::Load("resources/Green.png");
	textureHandle_[2] = TextureManager::Load("resources/Pink.png");
	isDead_ = false;
	

}

void Enemy::Draw(ViewProjection& viewProjection) {//描画
	if (isDead_ == false&&Straight==true) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_[0]);
	}
	if (isDead_ == false && Outer == true) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_[1]);
	}
	if (isDead_ == false && Change == true) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_[2]);
	}
}


void Enemy::Update() {//// 更新

#pragma region 移動変数
    float EnemySpeedZ = 0.0f;
	float UpMoveSpeed = 0.0f;
	float DownMoveSpeed = 0.0f;
	if (Straight==true)	
	{
	   EnemySpeedZ = -0.5f;
	   UpMoveSpeed = 0.5f;
	   DownMoveSpeed = -0.9f;
	}
	if (Outer == true)
	{
	   EnemySpeedZ = -0.5f;
	   UpMoveSpeed = 0.5f;
	   DownMoveSpeed = -0.9f;
	}
	if (Change == true) {
	   EnemySpeedZ = -0.5f;
	   UpMoveSpeed = 0.5f;
	   DownMoveSpeed = -0.9f;
	}
#pragma endregion
#pragma region 回転変数
	const float RotateSpeedX = 0.0f;
	const float RotateSpeedY = 0.0f;
#pragma endregion
	if (Straight==true) {
	//移動
	Vector3 move = {0, 0, EnemySpeedZ};
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	//回転
	Vector3 Rotate = {RotateSpeedX, RotateSpeedY,0};
	worldTransform_.rotation_ = Add(worldTransform_.translation_, Rotate);
	

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
	}

	if (Outer == true) {
	// 移動
	Vector3 move = {0, 0, EnemySpeedZ};
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// 回転
	Vector3 Rotate = {RotateSpeedX, RotateSpeedY, 0};
	worldTransform_.rotation_ = Add(worldTransform_.translation_, Rotate);

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 上に行く
	if (worldTransform_.translation_.z <= -5 && worldTransform_.translation_.z > -30) {
		worldTransform_.translation_.y = worldTransform_.translation_.y + UpMoveSpeed;
	}
	// 下に降りる
	if (worldTransform_.translation_.z < -30 && worldTransform_.translation_.y >= 0) {
		worldTransform_.translation_.y = worldTransform_.translation_.y + DownMoveSpeed;
	}
	}
	if (Change == true) {
	// 移動
	Vector3 move = {0, 0, EnemySpeedZ};
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// 回転
	Vector3 Rotate = {RotateSpeedX, RotateSpeedY, 0};
	worldTransform_.rotation_ = Add(worldTransform_.translation_, Rotate);

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 上に行く
	if (worldTransform_.translation_.z <= -5 && worldTransform_.translation_.z > -30) {
		worldTransform_.translation_.y = worldTransform_.translation_.y + UpMoveSpeed;
	}
	// 下に降りる
	if (worldTransform_.translation_.z < -30 && worldTransform_.translation_.y >= 0) {
		worldTransform_.translation_.y = worldTransform_.translation_.y + DownMoveSpeed;
	}
	}

	worldTransform_.UpdateMatrix();
	float EnemyPos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	float EnemyDebug[] = {static_cast<float>(SpawnTime), static_cast<float>(number)};


#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::SliderFloat3("EnemyPos",EnemyPos, 100.0f, -100.0f);
	ImGui::End();

	worldTransform_.translation_.x = EnemyPos[0];
	worldTransform_.translation_.y = EnemyPos[1];
	worldTransform_.translation_.z = EnemyPos[2];

#endif //_DEBUG

	

#ifdef _DEBUG
	

	ImGui::Begin("EnemySpawn");
	ImGui::Text("%f,%f", EnemyDebug,number);
	ImGui::Text("%f\n,%f", EnemyDebug,SpawnTime);
	ImGui::End();
	/*SpawnTime = static_cast<float>(EnemyDebug[0]);
	number = static_cast<float> (EnemyDebug[1]);*/

	
#endif //_DEBUG

	

	//敵撃破後のランダム生成処理
	//if (isDead_ == true)
	//{
	//	worldTransform_.translation_.z = 4;
	//	number = static_cast<float>(rand());
	//	number = static_cast<float>(rand() % 10 + 1);
	//	
	//	SpawnTime++;
	//	if (SpawnTime > 5&&number==1)
	//	{
	//		isDead_ = false;
	//		
	//		SpawnTime = 0;
	//	}
	//}
}


void Enemy::OnCollision() { isDead_ = true; }

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
