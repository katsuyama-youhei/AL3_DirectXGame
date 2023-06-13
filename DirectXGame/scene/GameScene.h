#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Player.h"
#include"Enemy.h"
#include"DebugCamera.h"
#include"Skydome.h"
#include"RailCamera.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 自機のテクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 自機のモデル
	Model* model_ = nullptr;
	//　敵のテクスチャハンドル
	uint32_t enemyTextureHandle_ = 0;
	//　敵のモデル
	Model* enemyModel_ = nullptr;
	// 天球のモデル
	Model* modelSkydome_ = nullptr;

	//WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	// 自機
	Player* player_ = nullptr;
	// 敵
	Enemy* enemy_ = nullptr;
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
