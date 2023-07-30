#pragma once
#include "Model.h"
#include"WorldTransform.h"
#include <assert.h>

class EnemyParticle {
public:
	// 初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	// 更新
	void Update();
	// 描画
	void Draw(const ViewProjection& viewProjection);
	// デスフラグ
	bool IsDead() const { return isDead_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 5 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
