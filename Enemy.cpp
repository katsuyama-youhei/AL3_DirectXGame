#include "Enemy.h"
#include"Calculation.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle){
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 2.0f, 32.0f};
	velocity_ = {0.0f, 0.0f, 0.0f};
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
};

void Enemy::Draw(ViewProjection viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};

void Enemy::Approach() {
	velocity_ = {0.0f, 0.0f, -0.2f};
	worldTransform_.translation_ = Calculation::Add(worldTransform_.translation_, velocity_);
	if (worldTransform_.translation_.z < 2.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	velocity_ = {-0.2f, 0.1f, -0.2f};
	worldTransform_.translation_ = Calculation::Add(worldTransform_.translation_, velocity_);
}