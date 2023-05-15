#include "Enemy.h"
#include"Calculation.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle){
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {10.0f, 2.0f, 32.0f};
	velocity_ = {0.0f, 0.0f, 0.0f};
	ApproachInitialize();
};

void Enemy::Update(){ 
	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}
	//worldTransform_.translation_ = Calculation::Subtract(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
};

void Enemy::Draw(ViewProjection viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
};

void Enemy::Approach() {
	velocity_ = {0.0f, 0.0f, -0.2f};
	worldTransform_.translation_ = Calculation::Add(worldTransform_.translation_, velocity_);
	fireTimer--;
	if (fireTimer <= 0) {
		Fire();
		fireTimer = kFireInterval;
	}
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	velocity_ = {-0.2f, 0.1f, -0.2f};
	worldTransform_.translation_ = Calculation::Add(worldTransform_.translation_, velocity_);
}

void Enemy::Fire() {
	EnemyBullet* newBullet = new EnemyBullet(); 
	newBullet->Initialize(model_, worldTransform_.translation_);
	bullets_.push_back(newBullet);
};

void Enemy::ApproachInitialize() { 
	fireTimer = 0;
}