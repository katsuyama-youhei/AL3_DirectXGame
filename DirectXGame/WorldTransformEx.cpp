#include"WorldTransform.h"
#include"Calculation.h"
#include"WorldTransform.h"

// 何のためかわからない
void WorldTransform::UpdateMatrix() {
	// アフィン変換
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	// 親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ =Multiply( matWorld_,parent_->matWorld_);
	}
	// 行列を変換
	TransferMatrix();
};