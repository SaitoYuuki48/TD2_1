#include "compute.h"

//加算
Vector3 Add(Vector3 vector1, Vector3 vector2) {
	Vector3 result = {0};
	result.x = vector1.x + vector2.x;
	result.y = vector1.y + vector2.y;
	result.z = vector1.z + vector2.z;
	return result;
}

// 減算
Vector3 Subtract(Vector3 v1, Vector3 v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

// 内積
float Dot(Vector3 v1, Vector3 v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return result;
}

// 長さ(ノルム)
float Length(const Vector3& v) { return sqrtf(Dot(v, v)); }

// 正規化
Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	assert(length != 0.0f);
	return {v.x / length, v.y / length, v.z / length};
}

// X軸回転行列
Matrix4x4 MakeRotateXmatrix(float radian) {
	Matrix4x4 result = {0};
	result.m[0][0] = cosf(radian);
	result.m[0][2] = -sinf(radian);
	result.m[1][1] = 1;
	result.m[2][0] = sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[3][3] = 1;
	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYmatrix(float radian) {
	Matrix4x4 result = {0};
	result.m[0][0] = cosf(radian);
	result.m[0][2] = -sinf(radian);
	result.m[1][1] = 1;
	result.m[2][0] = sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[3][3] = 1;
	return result;
};

// Z軸回転行列
Matrix4x4 MakeRotateZmatrix(float radian) {
	Matrix4x4 result = {0};
	result.m[0][0] = cosf(radian);
	result.m[0][1] = sinf(radian);
	result.m[1][0] = -sinf(radian);
	result.m[1][1] = cosf(radian);
	result.m[2][2] = 1;
	result.m[3][3] = 1;
	return result;
};

// 平行移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
};

// 拡大縮小
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;

	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float Term = 0;
			for (int k = 0; k < 4; k++) {
				Term = Term + m1.m[i][k] * m2.m[k][j];
				result.m[i][j] = Term;
			}
		}
	}
	return result;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	Matrix4x4 result;
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXmatrix(rot.x);
	Matrix4x4 rotateYMatrix = MakeRotateYmatrix(rot.y);
	Matrix4x4 rotateZMatrix = MakeRotateZmatrix(rot.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	return result;
}

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m1) {
	Matrix4x4 result;

	float deterninant = m1.m[0][0] * m1.m[1][1] * m1.m[2][2] * m1.m[3][3] +
	                    m1.m[0][0] * m1.m[1][2] * m1.m[2][3] * m1.m[3][1] +
	                    m1.m[0][0] * m1.m[1][3] * m1.m[2][1] * m1.m[3][2] -

	                    m1.m[0][0] * m1.m[1][3] * m1.m[2][2] * m1.m[3][1] -
	                    m1.m[0][0] * m1.m[1][2] * m1.m[2][1] * m1.m[3][3] -
	                    m1.m[0][0] * m1.m[1][1] * m1.m[2][3] * m1.m[3][2] -

	                    m1.m[0][1] * m1.m[1][0] * m1.m[2][2] * m1.m[3][3] -
	                    m1.m[0][2] * m1.m[1][0] * m1.m[2][3] * m1.m[3][1] -
	                    m1.m[0][3] * m1.m[1][0] * m1.m[2][1] * m1.m[3][2] +

	                    m1.m[0][3] * m1.m[1][0] * m1.m[2][2] * m1.m[3][1] +
	                    m1.m[0][2] * m1.m[1][0] * m1.m[2][1] * m1.m[3][3] +
	                    m1.m[0][1] * m1.m[1][0] * m1.m[2][3] * m1.m[3][2] +

	                    m1.m[0][1] * m1.m[1][2] * m1.m[2][0] * m1.m[3][3] +
	                    m1.m[0][2] * m1.m[1][3] * m1.m[2][0] * m1.m[3][1] +
	                    m1.m[0][3] * m1.m[1][1] * m1.m[2][0] * m1.m[3][2] -

	                    m1.m[0][3] * m1.m[1][2] * m1.m[2][0] * m1.m[3][1] -
	                    m1.m[0][2] * m1.m[1][1] * m1.m[2][0] * m1.m[3][3] -
	                    m1.m[0][1] * m1.m[1][3] * m1.m[2][0] * m1.m[3][2] -

	                    m1.m[0][1] * m1.m[1][2] * m1.m[2][3] * m1.m[3][0] -
	                    m1.m[0][2] * m1.m[1][3] * m1.m[2][1] * m1.m[3][0] -
	                    m1.m[0][3] * m1.m[1][1] * m1.m[2][2] * m1.m[3][0] +

	                    m1.m[0][3] * m1.m[1][2] * m1.m[2][1] * m1.m[3][0] +
	                    m1.m[0][2] * m1.m[1][1] * m1.m[2][3] * m1.m[3][0] +
	                    m1.m[0][1] * m1.m[1][3] * m1.m[2][2] * m1.m[3][0];

	assert(deterninant != 0.0f);
	float deterninantRect = 1.0f / deterninant;

	result.m[0][0] = (m1.m[1][1] * m1.m[2][2] * m1.m[3][3] + m1.m[1][2] * m1.m[2][3] * m1.m[3][1] +
	                  m1.m[1][3] * m1.m[2][1] * m1.m[3][2] - m1.m[1][3] * m1.m[2][2] * m1.m[3][1] -
	                  m1.m[1][2] * m1.m[2][1] * m1.m[3][3] - m1.m[1][1] * m1.m[2][3] * m1.m[3][2]) *
	                 deterninantRect;
	result.m[0][1] = (-m1.m[0][1] * m1.m[2][2] * m1.m[3][3] - m1.m[0][2] * m1.m[2][3] * m1.m[3][1] -
	                  m1.m[0][3] * m1.m[2][1] * m1.m[3][2] + m1.m[0][3] * m1.m[2][2] * m1.m[3][1] +
	                  m1.m[0][2] * m1.m[2][1] * m1.m[3][3] + m1.m[0][1] * m1.m[2][3] * m1.m[3][2]) *
	                 deterninantRect;
	result.m[0][2] = (m1.m[0][1] * m1.m[1][2] * m1.m[3][3] + m1.m[0][2] * m1.m[1][3] * m1.m[3][1] +
	                  m1.m[0][3] * m1.m[1][1] * m1.m[3][2] - m1.m[0][3] * m1.m[1][2] * m1.m[3][1] -
	                  m1.m[0][2] * m1.m[1][1] * m1.m[3][3] - m1.m[0][1] * m1.m[1][3] * m1.m[3][2]) *
	                 deterninantRect;
	result.m[0][3] = (-m1.m[0][1] * m1.m[1][2] * m1.m[2][3] - m1.m[0][2] * m1.m[1][3] * m1.m[2][1] -
	                  m1.m[0][3] * m1.m[1][1] * m1.m[2][2] + m1.m[0][3] * m1.m[1][2] * m1.m[2][1] +
	                  m1.m[0][2] * m1.m[1][1] * m1.m[2][3] + m1.m[0][1] * m1.m[1][3] * m1.m[2][2]) *
	                 deterninantRect;

	result.m[1][0] = (-m1.m[1][0] * m1.m[2][2] * m1.m[3][3] - m1.m[1][2] * m1.m[2][3] * m1.m[3][0] -
	                  m1.m[1][3] * m1.m[2][0] * m1.m[3][2] + m1.m[1][3] * m1.m[2][2] * m1.m[3][0] +
	                  m1.m[1][2] * m1.m[2][0] * m1.m[3][3] + m1.m[1][0] * m1.m[2][3] * m1.m[3][2]) *
	                 deterninantRect;
	result.m[1][1] = (m1.m[0][0] * m1.m[2][2] * m1.m[3][3] + m1.m[0][2] * m1.m[2][3] * m1.m[3][0] +
	                  m1.m[0][3] * m1.m[2][0] * m1.m[3][2] - m1.m[0][3] * m1.m[2][2] * m1.m[3][0] -
	                  m1.m[0][2] * m1.m[2][0] * m1.m[3][3] - m1.m[0][0] * m1.m[2][3] * m1.m[3][2]) *
	                 deterninantRect;
	result.m[1][2] = (-m1.m[0][0] * m1.m[1][2] * m1.m[3][3] - m1.m[0][2] * m1.m[1][3] * m1.m[3][0] -
	                  m1.m[0][3] * m1.m[1][0] * m1.m[3][2] + m1.m[0][3] * m1.m[1][2] * m1.m[3][0] +
	                  m1.m[0][2] * m1.m[1][0] * m1.m[3][3] + m1.m[0][0] * m1.m[1][3] * m1.m[3][2]) *
	                 deterninantRect;
	result.m[1][3] = (m1.m[0][0] * m1.m[1][2] * m1.m[2][3] + m1.m[0][2] * m1.m[1][3] * m1.m[2][0] +
	                  m1.m[0][3] * m1.m[1][0] * m1.m[2][2] - m1.m[0][3] * m1.m[1][2] * m1.m[2][0] -
	                  m1.m[0][2] * m1.m[1][0] * m1.m[2][3] - m1.m[0][0] * m1.m[1][3] * m1.m[2][2]) *
	                 deterninantRect;

	result.m[2][0] = (m1.m[1][0] * m1.m[2][1] * m1.m[3][3] + m1.m[1][1] * m1.m[2][3] * m1.m[3][0] +
	                  m1.m[1][3] * m1.m[2][0] * m1.m[3][1] - m1.m[1][3] * m1.m[2][1] * m1.m[3][0] -
	                  m1.m[1][1] * m1.m[2][0] * m1.m[3][3] - m1.m[1][0] * m1.m[2][3] * m1.m[3][1]) *
	                 deterninantRect;
	result.m[2][1] = (-m1.m[0][0] * m1.m[2][1] * m1.m[3][3] - m1.m[0][1] * m1.m[2][3] * m1.m[3][0] -
	                  m1.m[0][3] * m1.m[2][0] * m1.m[3][1] + m1.m[0][3] * m1.m[2][1] * m1.m[3][0] +
	                  m1.m[0][1] * m1.m[2][0] * m1.m[3][3] + m1.m[0][0] * m1.m[2][3] * m1.m[3][1]) *
	                 deterninantRect;
	result.m[2][2] = (m1.m[0][0] * m1.m[1][1] * m1.m[3][3] + m1.m[0][1] * m1.m[1][3] * m1.m[3][0] +
	                  m1.m[0][3] * m1.m[1][0] * m1.m[3][1] - m1.m[0][3] * m1.m[1][1] * m1.m[3][0] -
	                  m1.m[0][1] * m1.m[1][0] * m1.m[3][3] - m1.m[0][0] * m1.m[1][3] * m1.m[3][1]) *
	                 deterninantRect;
	result.m[2][3] = (-m1.m[0][0] * m1.m[1][1] * m1.m[2][3] - m1.m[0][1] * m1.m[1][3] * m1.m[2][0] -
	                  m1.m[0][3] * m1.m[1][0] * m1.m[2][1] + m1.m[0][3] * m1.m[1][1] * m1.m[2][0] +
	                  m1.m[0][1] * m1.m[1][0] * m1.m[2][3] + m1.m[0][0] * m1.m[1][3] * m1.m[2][1]) *
	                 deterninantRect;

	result.m[3][0] = (-m1.m[1][0] * m1.m[2][1] * m1.m[3][2] - m1.m[1][1] * m1.m[2][2] * m1.m[3][0] -
	                  m1.m[1][2] * m1.m[2][0] * m1.m[3][1] + m1.m[1][2] * m1.m[2][1] * m1.m[3][0] +
	                  m1.m[1][1] * m1.m[2][0] * m1.m[3][2] + m1.m[1][0] * m1.m[2][2] * m1.m[3][1]) *
	                 deterninantRect;
	result.m[3][1] = (m1.m[0][0] * m1.m[2][1] * m1.m[3][2] + m1.m[0][1] * m1.m[2][2] * m1.m[3][0] +
	                  m1.m[0][2] * m1.m[2][0] * m1.m[3][1] - m1.m[0][2] * m1.m[2][1] * m1.m[3][0] -
	                  m1.m[0][1] * m1.m[2][0] * m1.m[3][2] - m1.m[0][0] * m1.m[2][2] * m1.m[3][1]) *
	                 deterninantRect;
	result.m[3][2] = (-m1.m[0][0] * m1.m[1][1] * m1.m[3][2] - m1.m[0][1] * m1.m[1][2] * m1.m[3][0] -
	                  m1.m[0][2] * m1.m[1][0] * m1.m[3][1] + m1.m[0][2] * m1.m[1][1] * m1.m[3][0] +
	                  m1.m[0][1] * m1.m[1][0] * m1.m[3][2] + m1.m[0][0] * m1.m[1][2] * m1.m[3][1]) *
	                 deterninantRect;
	result.m[3][3] = (m1.m[0][0] * m1.m[1][1] * m1.m[2][2] + m1.m[0][1] * m1.m[1][2] * m1.m[2][0] +
	                  m1.m[0][2] * m1.m[1][0] * m1.m[2][1] - m1.m[0][2] * m1.m[1][1] * m1.m[2][0] -
	                  m1.m[0][1] * m1.m[1][0] * m1.m[2][2] - m1.m[0][0] * m1.m[1][2] * m1.m[2][1]) *
	                 deterninantRect;

	return result;
}