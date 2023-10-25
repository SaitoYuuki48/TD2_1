#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	
}

void Skydome::Update() { 
	worldTransform_.UpdateMatrix(); 

	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.01f;

	//回転
	//worldTransform_.rotation_.x -= kRotSpeed;
	//worldTransform_.rotation_.y -= kRotSpeed;
	worldTransform_.rotation_.z -= kRotSpeed;
}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}