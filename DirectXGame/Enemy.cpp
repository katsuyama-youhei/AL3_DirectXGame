#include "Enemy.h"
#include"Calculation.h"
#include"Player.h"

Enemy::~Enemy() {
	
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
	
	//worldTransform_.translation_ = Calculation::Subtract(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	
};

void Enemy::Draw(ViewProjection viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};

void Enemy::Approach() {
	velocity_ = {0.0f, 0.0f, -0.2f};
	//worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
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
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}

void Enemy::Fire() {
	assert(player_);
	const float kBulletSpeed = 1.0f;

	Vector3 playerWorldPos = player_->GetWorldPosition();
	Vector3 enemyWorldPos = GetWorldPosition();
	Vector3 differenceVector = Subtract(playerWorldPos, enemyWorldPos);
	
	differenceVector = Normlize(differenceVector);
	
	Vector3 velocity(
	    differenceVector.x * kBulletSpeed, 
		differenceVector.y * kBulletSpeed,
	    differenceVector.z * kBulletSpeed
	);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	bullets_.push_back(newBullet);
};

void Enemy::ApproachInitialize() { fireTimer = 60; }

// ワールド座標を取得
Vector3 Enemy::GetWorldPosition(){

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
	};

// 当たっても何も起きない
void Enemy::OnCollision() {

}