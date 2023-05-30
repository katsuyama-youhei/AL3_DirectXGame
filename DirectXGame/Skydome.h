#pragma once
#include "Model.h"
#include"WorldTransform.h"
#include <assert.h>

class Skydome {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	// モデルがでかいと楽
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};
