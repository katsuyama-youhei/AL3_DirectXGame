#include "Player.h"
#include "Calculation.h"
#include "ImGuiManager.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	// モデルのnullチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
	input_ = Input::GetInstance();
	// 3Dレティクル用ワールドトランスフォームの初期化
	worldTransform3DReticle_.Initialize();
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("./Resources/point.png");
	// スプライト生成
	sprite2DReticle_ = Sprite::Create(
	    textureReticle, Vector2(0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f));
}

void Player::Update(ViewProjection viewProjection) {
	worldTransform_.TransferMatrix();

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	Rotate();

	//  移動ベクトル
	Vector3 move = {0, 0, 0};
	// 　移動の速さ
	const float kCharacterSpeed = 0.2f;
	// 推した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	worldTransform_.UpdateMatrix();

	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 35.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {
	    0,
	    0,
	    1.0f,
	};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える　
	offset = Multiply(kDistancePlayerTo3DReticle, Normlize(offset));
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(offset, worldTransform_.translation_);
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標の計算より後に行う

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	// 3Dレティクルのワールド座標を取得
	 Vector3 positionReticle = Get3DReticleWorldPosition();
	
	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列,ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

	// ワールドスクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	// デバッグテキストの表示
	ImGui::Begin("a");
	inputFloat3[0] = worldTransform_.translation_.x;
	inputFloat3[1] = worldTransform_.translation_.y;
	inputFloat3[2] = worldTransform_.translation_.z;
	ImGui::SliderFloat3("Player", inputFloat3, -30.0f, 30.0f);
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];
	ImGui::End();

	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 18.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::Rotate() {
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = Subtract(Get3DReticleWorldPosition(), GetWorldPosition());
		// 速度ベクトルを自機の向きに合わせて回転させる
		// velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		velocity = Multiply(kBulletSpeed, Normlize(velocity));

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		// リストに登録
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::Get3DReticleWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision() {
	// 何もしない
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

void Player::DrawUI() { sprite2DReticle_->Draw(); };