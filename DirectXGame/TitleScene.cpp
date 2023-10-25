#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() { delete spriteTitle_; }

void TitleScene::Initialize() 
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャ
	uint32_t textureTitle = TextureManager::Load("resources/Title.png");
	// ルールテクスチャ
	uint32_t Luletexture = TextureManager::Load("resources/Lule.png");

	// スプライト生成
	spriteTitle_ =
	    Sprite::Create(textureTitle, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	// スプライト生成
	spriteLule_ = Sprite::Create(Luletexture, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		isLule = true;
	}
	if (input_->TriggerKey(DIK_RETURN)&& isLule == true) {
		isLule = false;
		isSceneEnd = true;
	}
}

void TitleScene::Draw()
{
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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	spriteTitle_->Draw();
	if (isLule == true)
	{
		spriteLule_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void TitleScene::sceneReset() { isSceneEnd = false; }
