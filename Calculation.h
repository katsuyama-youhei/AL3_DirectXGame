#pragma once

class Calculation {
public:
	// 加算
	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
	// 減算
	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
	// 積
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	// 逆行列
	static Matrix4x4 Inverse(const Matrix4x4& m);
	// 転置行列
	static Matrix4x4 Transpose(const Matrix4x4& m);
	// 単位行列の作成
	static Matrix4x4 MakeIdentity4x4();

	// 加算
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	// 減算
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	// スカラー倍
	static Vector3 Multiply(float scalar, const Vector3& v);
	// 内積
	static float Dot(const Vector3& v1, const Vector3& v2);
	// 長さ（ノルム）
	static float Length(const Vector3& v);
	// 正規化
	static Vector3 Normlize(const Vector3& v);

	// 回転行列
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);

	//
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	// アフィン変換行列
	static Matrix4x4
	    MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
};

//Vector3 operator+(const Vector3& v1, const Vector3& v2);