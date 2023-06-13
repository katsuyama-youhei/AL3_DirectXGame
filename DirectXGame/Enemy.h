#pragma once
#include "Model.h"
#include"WorldTransform.h"
#include <assert.h>
#include"EnemyBullet.h"
#include<list>

// 前方宣言


class Player;

class Enemy {
public:
	~Enemy();
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection viewProjection);
	void Approach();
	void Leave();
	void Fire();

	static const int kFireInterval = 60;
	void ApproachInitialize();
	// 別で宣言しているplayerをこのクラス内のplayerに代入
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();
	void OnCollision();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	int32_t fireTimer = 0;
	
	//	自キャラ
	Player* player_ = nullptr;
	// ゲームシーン
	GameScene* gameScene_ = nullptr;
	
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	
	};
	Phase phase_ = Phase::Approach;
};


