#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	// 初期化
	void Initialize(const Vector3& position,const Vector3& radian);
	// 更新
	void Update();
	// ビュープロジェクトの取得
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	// ワールドトランスフォームの取得
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 移動速度
	Vector3 translationVelocity = {0.0f, 0.0f, 0.1f};
	// 回転速度
	Vector3 rotationVelocity = {0.0f, 0.0f, 0.0f};
};
