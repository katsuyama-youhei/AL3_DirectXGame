#pragma once
#include "Model.h"
#include"WorldTransform.h"
#include <assert.h>
#include"EnemyBullet.h"
#include<list>

// 前方宣言
class GameScene;

class Player;

class Enemy {
public:
	// デストラクト
	~Enemy();
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	// 更新
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection);
	// 接近フェーズの処理
	void Approach();
	// 離脱フェーズの処理
	void Leave();
	// 弾の発射
	void Fire();

	static const int kFireInterval = 60;
	void ApproachInitialize();
	// 別で宣言しているplayerをこのクラス内のplayerに代入
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();
	void OnCollision();
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	bool IsDead() const { return isDead_; }
	void SetWorldTransformTranslation(Vector3 worldTransformTranslation) {
		worldTransform_.translation_ = worldTransformTranslation;
	}

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	//　モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	
	int32_t fireTimer = 0;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;
	// デスフラグ
	bool isDead_ = false;

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	
	};
	Phase phase_ = Phase::Approach;

	// メンバ関数ポインタ
	static void (Enemy::*pFunc[static_cast<size_t>(Phase::Leave) + 1])();
};


