#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete enemyModel_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	enemyTextureHandle_ = TextureManager::Load("sentouki.png");
	enemyModel_ = Model::Create();
	viewProjection_.Initialize();
	player_ = new Player();
	Vector3 playerPosition(0, 0, 25);
	player_->Initialize(model_, textureHandle_,playerPosition);
	debugCamera_ = new DebugCamera(int(1280.0f), int(720.0f));
	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, enemyTextureHandle_);
	enemy_->SetPlayer(player_);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, -50.0f}, player_->GetRotate());
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	player_->Update();
	if (enemy_) {
		enemy_->Update();
	}
	CheckAllCollisions();
//	skydome_->Update();
	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		isDebygCameraActive_ = true;
		//enemy_ = nullptr;
	}
#endif
	if (isDebygCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		//viewProjection_.UpdateMatrix();
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	debugCamera_->Update();
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

	player_->Draw(viewProjection_);
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}
	skydome_->Draw(viewProjection_);
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

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB, radiusA, radiusB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region
	posA = player_->GetWorldPosition();
	radiusA = {3.0f, 3.0f, 3.0f};
	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();
		radiusB = {3.0f, 3.0f, 3.0f};
		Vector3 a2b = {
		    (posA.x - posB.x) * (posA.x - posB.x), (posA.y - posB.y) * (posA.y - posB.y),
		    (posA.z - posB.z) * (posA.z - posB.z)};
		Vector3 a2br = {
		    (radiusA.x + radiusB.x) * (radiusA.x + radiusB.x),
		    (radiusA.y + radiusB.y) * (radiusA.y + radiusB.y),
		    (radiusA.z + radiusB.z) * (radiusA.z + radiusB.z)};
		if (a2b.x + a2b.y + a2b.z <= a2br.x || a2b.x + a2b.y + a2b.z <= a2br.y ||
		    a2b.x + a2b.y + a2b.z <= a2br.z) {
			bullet->OnCollision();
			player_->OnCollision();
		}
	}

#pragma endregion

#pragma region
	posA = enemy_->GetWorldPosition();
	radiusA = {3.0f, 3.0f, 3.0f};
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		radiusB = {3.0f, 3.0f, 3.0f};
		Vector3 a2b = {
		    (posA.x - posB.x) * (posA.x - posB.x), (posA.y - posB.y) * (posA.y - posB.y),
		    (posA.z - posB.z) * (posA.z - posB.z)};
		Vector3 a2br = {
		    (radiusA.x + radiusB.x) * (radiusA.x + radiusB.x),
		    (radiusA.y + radiusB.y) * (radiusA.y + radiusB.y),
		    (radiusA.z + radiusB.z) * (radiusA.z + radiusB.z)};
		if (a2b.x + a2b.y + a2b.z <= a2br.x || a2b.x + a2b.y + a2b.z <= a2br.y ||
		    a2b.x + a2b.y + a2b.z <= a2br.z) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region
	
	for (PlayerBullet* bullet_ : playerBullets) {
		posA=bullet_->GetWorldPosition();
		radiusA = {3.0f, 3.0f, 3.0f};
		for (EnemyBullet* bullet : enemyBullets) {
			posB=bullet->GetWorldPosition();
			radiusB = {3.0f, 3.0f, 3.0f};
			Vector3 a2b = {
			    (posA.x - posB.x) * (posA.x - posB.x), (posA.y - posB.y) * (posA.y - posB.y),
			    (posA.z - posB.z) * (posA.z - posB.z)};
			Vector3 a2br = {
			    (radiusA.x + radiusB.x) * (radiusA.x + radiusB.x),
			    (radiusA.y + radiusB.y) * (radiusA.y + radiusB.y),
			    (radiusA.z + radiusB.z) * (radiusA.z + radiusB.z)};
			if (a2b.x + a2b.y + a2b.z <= a2br.x || a2b.x + a2b.y + a2b.z <= a2br.y ||
			    a2b.x + a2b.y + a2b.z <= a2br.z) {
				bullet->OnCollision();
				bullet_->OnCollision();
			}
		}
	}

#pragma endregion
};