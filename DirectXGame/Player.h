#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include <assert.h>
#include <list>

class Player {
public:
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);
	// 更新
	void Update(ViewProjection viewProjection);
	// 描画
	void Draw(ViewProjection viewProjection);
	// 回転
	void Rotate();
	// 弾の発射
	void Attack();
	// デストラクト
	~Player();
	// ワールド座礁を取得
	Vector3 GetWorldPosition();
	// 衝突時にする行動
	void OnCollision();
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	// 回転量の取得
	Vector3 GetRotate() { return worldTransform_.rotation_; }
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent);

	// 3DReticleのワールド座標を取得
	Vector3 Get3DReticleWorldPosition();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};
	std::list<PlayerBullet*> bullets_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
};
