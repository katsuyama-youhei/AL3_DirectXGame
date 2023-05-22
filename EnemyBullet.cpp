#include "EnemyBullet.h"
#include "Calculation.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	textureHandle_ = TextureManager::Load("Jade.png");
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
};

void EnemyBullet::Update() {
	worldTransform_.translation_ = Calculation::Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
};

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};