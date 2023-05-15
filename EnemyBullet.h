#pragma once
#include "Model.h"
#include"WorldTransform.h"
#include <assert.h>

class EnemyBullet {
public:
	void Initialize(Model* model,const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
