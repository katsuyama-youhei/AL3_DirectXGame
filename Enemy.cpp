#include "Enemy.h"
#include"Calculation.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle){
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 2.0f, 32.0f};
	velocity_ = {0.0f, 0.0f, 0.2f};
};

void Enemy::Update(){ 
	worldTransform_.translation_ = Calculation::Subtract(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
};

void Enemy::Draw(ViewProjection viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};