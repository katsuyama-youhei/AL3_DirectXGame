#include "Enemy.h"
#include "Calculation.h"
#include "GameScene.h"
#include "Player.h"

// staticで宣言したメンバ関数ポインタテーブルの実体
void (Enemy::*Enemy::pFunc[static_cast<size_t>(Phase::Leave) + 1])() = {
    &Enemy::Approach, &Enemy::Leave};

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {10.0f, 2.0f, 32.0f};
	velocity_ = {0.0f, 0.0f, 0.0f};
	ApproachInitialize();
};

void Enemy::Update() {

	// 現在のフェーズの関数を実行
	(this->*pFunc[static_cast<size_t>(phase_)])();
	// worldTransform_.translation_ = Calculation::Subtract(worldTransform_.translation_,
	// velocity_);
	worldTransform_.UpdateMatrix();

	if (player_->GetWorldPosition().z > GetWorldPosition().z) {
		fireTimer = kFireInterval;
		if (player_->GetWorldPosition().z > GetWorldPosition().z + 20.0f) {
			isDead_ = true;
		}
	}
	if (hp <= 0) {
		isDead_ = true;
	}
};

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};

void Enemy::Approach() {
	velocity_ = {0.0f, 0.0f, -0.1f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	fireTimer--;
	if (fireTimer <= 0) {
		Fire();
		fireTimer = kFireInterval;
	}
	// z座標が０よりも小さくなったら離脱フェーズに移行
	if (worldTransform_.translation_.z < player_->GetWorldPosition().z + 10.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	Vector3 pos = GetWorldPosition();
	if (pos.x < 0) {
		velocity_.x = -0.2f;
	} else {
		velocity_.x = 0.2f;
	}

	if (pos.y < 0) {
		velocity_.y = -0.2f;
	} else {

		velocity_.y = 0.2f;
	}
	// velocity_ = {0.0f, 0.2f, 0.0f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}

void Enemy::Fire() {
	assert(player_);
	const float kBulletSpeed = 0.5f;

	// 自機のワールド座標
	Vector3 playerWorldPos = player_->GetWorldPosition();
	// 敵のワールド座標
	Vector3 enemyWorldPos = GetWorldPosition();
	// 自機から敵までの距離
	Vector3 differenceVector = Subtract(playerWorldPos, enemyWorldPos);

	differenceVector = Normlize(differenceVector);

	Vector3 velocity = Multiply(kBulletSpeed, differenceVector);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, enemyWorldPos, velocity);
	// bullets_.push_back(newBullet);
	gameScene_->AddEnemyBullet(newBullet);
};

void Enemy::ApproachInitialize() { fireTimer = 240; }

// ワールド座標を取得
Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
};

// 当たっても何も起きない
void Enemy::OnCollision() {
	Hit();
	HitParticle();
}

void Enemy::HitParticle() {
	// const float kBulletSpeed = 0.5f;
	Vector3 velocity{0.5f, 0.5f, -0.2f};

	// 敵のワールド座標
	Vector3 enemyWorldPos = GetWorldPosition();
	int max = 6;
	for (int i = 0; i < max; ++i) {
		EnemyParticle* newBullet = new EnemyParticle();
		newBullet->Initialize(model_, enemyWorldPos, velocity);
		
		gameScene_->AddEnemyParticle(newBullet);
	}

	//EnemyParticle* newBullet = new EnemyParticle();
	//newBullet->Initialize(model_, enemyWorldPos, velocity);
	//// bullets_.push_back(newBullet);
	//gameScene_->AddEnemyParticle(newBullet);
};