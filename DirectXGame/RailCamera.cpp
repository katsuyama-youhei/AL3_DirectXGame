#include "RailCamera.h"
#include"Calculation.h"
#include"ImGuiManager.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& radian){
	worldTransform_.translation_=position;
	worldTransform_.rotation_ = radian;
	viewProjection_.Initialize();
};

void RailCamera::Update() {
	// ワールドトランスフォームの座標の数値を加算したりする(移動)
	worldTransform_.translation_ = Add(worldTransform_.translation_, translationVelocity);
	// ワールドトランスフォームの角度の数値を加算したりする(回転)
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotationVelocity);
	
	// カメラの座標を画面表示する
	ImGui::Begin("Camera");
	// translationを表示
	float inputFloat3[3] = {0, 0, 0};
	inputFloat3[0] = worldTransform_.translation_.x;
	inputFloat3[1] = worldTransform_.translation_.y;
	inputFloat3[2] = worldTransform_.translation_.z;
	ImGui::SliderFloat3("transform", inputFloat3, 0.0f, 1.0f);
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];
	// rotationを表示
	inputFloat3[0] = worldTransform_.rotation_.x;
	inputFloat3[1] = worldTransform_.rotation_.y;
	inputFloat3[2] = worldTransform_.rotation_.z;
	ImGui::SliderFloat3("rotation", inputFloat3, 0.0f, 1.0f);
	worldTransform_.rotation_.x = inputFloat3[0];
	worldTransform_.rotation_.y = inputFloat3[1];
	worldTransform_.rotation_.z = inputFloat3[2];

	ImGui::End();

	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// ビュープロジェクションを転送
	viewProjection_.TransferMatrix();
};