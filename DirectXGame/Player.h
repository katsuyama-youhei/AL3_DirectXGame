#pragma once
#include "Model.h"
#include"WorldTransform.h"
#include <assert.h>
#include"Input.h"
#include"PlayerBullet.h"
#include<list>

class Player {
public:
	void Initialize(Model* model,uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection viewProjection);
	void Rotate();
	void Attack();
	~Player();
	// ワールド座礁を取得
	Vector3 GetWorldPosition();
	void OnCollision();
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};
	std::list<PlayerBullet*> bullets_;

};
