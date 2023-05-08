#include"WorldTransform.h"
#include"Calculation.h"
#include"WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = Calculation::MakeAfineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
};