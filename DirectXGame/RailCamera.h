#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 
/// </summary>
class RailCamera {
public:
	void Initialize(const Vector3& position,const Vector3& radian);
	void Update();
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 移動速度
	Vector3 translationVelocity = {0.0f, 0.0f, 0.1f};
	// 回転速度
	Vector3 rotationVelocity = {0.0f, 0.003f, 0.0f};
};
