#pragma once
#include "Model.h"
#include"WorldTransform.h"
#include <assert.h>
#include"EnemyBullet.h"
#include<list>

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
		Approach, // ê⁄ãﬂÇ∑ÇÈ
		Leave,    // ó£íEÇ∑ÇÈ
	
	};
	Phase phase_ = Phase::Approach;
};


