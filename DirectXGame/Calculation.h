#pragma once

// 加算
 Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 減算
 Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// 積
 Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 逆行列
 Matrix4x4 Inverse(const Matrix4x4& m);
// 転置行列
 Matrix4x4 Transpose(const Matrix4x4& m);
// 単位行列の作成
 Matrix4x4 MakeIdentity4x4();

// 加算
 Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
 Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
 Vector3 Multiply(float scalar, const Vector3& v);
// 内積
 float Dot(const Vector3& v1, const Vector3& v2);
// 長さ（ノルム）
 float Length(const Vector3& v);
// 正規化
 Vector3 Normlize(const Vector3& v);

// 回転行列
 Matrix4x4 MakeRotateXMatrix(float radian);
 Matrix4x4 MakeRotateYMatrix(float radian);
 Matrix4x4 MakeRotateZMatrix(float radian);

//
 Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

 Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// アフィン変換行列
 Matrix4x4
    MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// ベクトル変換
 Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// Vector3 operator+(const Vector3& v1, const Vector3& v2);