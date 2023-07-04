#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete enemyModel_;
	delete player_;

	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete railCamera_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// 自機のテクスチャ
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 自機のモデル
	model_ = Model::Create();

	// 敵のテクスチャ
	enemyTextureHandle_ = TextureManager::Load("sentouki.png");
	// 敵のモデル
	enemyModel_ = Model::Create();

	viewProjection_.Initialize();

	// 自機の生成
	player_ = new Player();
	Vector3 playerPosition(0, 0, 25);
	player_->Initialize(model_, textureHandle_, playerPosition);

	debugCamera_ = new DebugCamera(int(1280.0f), int(720.0f));

	// 敵の生成
	Enemy* enemy = new Enemy();
	enemy->Initialize(enemyModel_, enemyTextureHandle_);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);

	enemies_.push_back(enemy);

	LoadEnemyPopData();

	// 天球の生成
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

		// レティクルのテクスチャ
	TextureManager::Load("./Resources/point.png");
}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		isDebygCameraActive_ = true;
		// enemy_ = nullptr;
	}
#endif
	if (isDebygCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		// viewProjection_.UpdateMatrix();
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	player_->Update(viewProjection_);

	UpdateEnemyPopCommands();
	
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		// bullet->SetPlayer(player_);
		bullet->Update();
	}

	CheckAllCollisions();

	//	skydome_->Update();

	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

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
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
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

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB, radiusA, radiusB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region
	// 　自機と敵弾の衝突判定
	posA = player_->GetWorldPosition();
	radiusA = {3.0f, 3.0f, 3.0f};
	for (EnemyBullet* bullet : enemyBullets_) {
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
	// 敵と自弾の衝突判定
	for (Enemy* enemy : enemies_) {
		posA = enemy->GetWorldPosition();
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
				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region
	// 自弾と敵弾の衝突判定
	for (PlayerBullet* bullet_ : playerBullets) {
		posA = bullet_->GetWorldPosition();
		radiusA = {3.0f, 3.0f, 3.0f};
		for (EnemyBullet* bullet : enemyBullets_) {
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
				//bullet_->OnCollision();
			}
		}
	}

#pragma endregion
};

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); };

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
};

void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (commandIsWait) {
		commandWaitTimer--;
		if (commandWaitTimer <= 0) {
			// 待機完了
			commandIsWait = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;
	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の未次烈をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行に先頭文字列を取得
		getline(line_stream, word, ',');
		// "//"から始まる行はコマンド
		if (word.find("//") == 0) {
			// コメント行は飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			// 敵を発生させる
			Vector3 tmp = {x, y, z};
			EnemyOccurrence(model_, enemyTextureHandle_, tmp);
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			commandIsWait = true;
			commandWaitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
};

void GameScene::EnemyOccurrence(Model* model, uint32_t textureHandle, Vector3& vector) {
	// 敵キャラの生成
	Enemy* enemy = new Enemy();
	// 敵キャラの初期化
	enemy->Initialize(model, textureHandle);
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	// 敵キャラにゲームシーンのアドレスを渡す
	enemy->SetGameScene(this);
	// 敵キャラに座標を渡す
	enemy->SetWorldTransformTranslation(vector);
	enemies_.push_back(enemy);
}