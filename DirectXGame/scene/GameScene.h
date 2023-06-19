#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <sstream>

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	// csvファイルのロード
	void LoadEnemyPopData();
	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();
	// 敵発生
	void EnemyOccurrence(Model* model, uint32_t textureHandle,Vector3& vector);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 自機のテクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 自機のモデル
	Model* model_ = nullptr;
	// 　敵のテクスチャハンドル
	uint32_t enemyTextureHandle_ = 0;
	// 　敵のモデル
	Model* enemyModel_ = nullptr;
	// 天球のモデル
	Model* modelSkydome_ = nullptr;

	// WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	// 自機
	Player* player_ = nullptr;
	// 敵
	// Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;
	std::stringstream enemyPopCommands;
	// 敵の発生コマンドの待機中フラグ
	bool commandIsWait = false;
	// 敵の発生コマンドの待機中タイマー(フレーム)
	size_t commandWaitTimer = 0;
	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;
	// 天球
	Skydome* skydome_ = nullptr;
	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// デバッグカメラ
	bool isDebygCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
