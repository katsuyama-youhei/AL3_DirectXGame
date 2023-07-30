#include "EnemyParticle.h"
#include "Calculation.h"

	// 初期化
void EnemyParticle::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	    assert(model);
	    textureHandle_ = TextureManager::Load("tama_rakugaki.png");
	    model_ = model;
	    worldTransform_.Initialize();
	    worldTransform_.translation_ = position;
	    velocity_ = Multiply(float( 0.1f),velocity);
    };
// 更新
void EnemyParticle::Update(){
	    worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	    worldTransform_.UpdateMatrix();
	    if (--deathTimer_ <= 0) {
		    isDead_ = true;
	    }
    };
// 描画
void EnemyParticle::Draw(const ViewProjection& viewProjection){
	    model_->Draw(worldTransform_, viewProjection, textureHandle_);
    };