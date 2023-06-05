#include"WorldTransform.h"
#include"Calculation.h"
#include"WorldTransform.h"

// 何のためかわからない
void WorldTransform::UpdateMatrix() {
	// アフィン変換
	matWorld_ = MakeAfineMatrix(scale_, rotation_, translation_);
	// 行列を変換
	TransferMatrix();
};