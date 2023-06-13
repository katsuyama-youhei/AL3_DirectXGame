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
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	std::list<EnemyBullet*> bullets_;
	int32_t fireTimer = 0;
	Player* player_ = nullptr;
	
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	
	};
	Phase phase_ = Phase::Approach;

	// メンバ関数ポインタ
	static void (Enemy::*pFunc[static_cast<size_t>(Phase::Leave) + 1])();
};


