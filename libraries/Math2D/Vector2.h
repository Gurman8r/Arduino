#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include <inttypes.h>

template <typename T>
class Vector2
{
private:
	T data[2];
	
public:
	typedef Vector2<T> self_type;

public:
	Vector2() {
		data[0] = T(0);
		data[1] = T(0);
	}
	Vector2(const T & xy) {
		data[0] = xy;
		data[1] = xy;
	}
	Vector2(const T & xx, const T & yy) {
		data[0] = xx;
		data[1] = yy;
	}
	~Vector2() {
	}
	
	template<typename U>
	explicit Vector2(const Vector2<U>& copy) {
		(*this)[0] = static_cast<T>(copy[0]);
		(*this)[1] = static_cast<T>(copy[1]);
	};

	inline const T * ptr() const {
		return &data[0];
	};
	inline const T & operator[](unsigned index) const {
		return data[index];
	}
	inline T & operator[](unsigned index) { 
		return data[index];
	}

	inline const T & x() const { return (*this)[0]; }
	inline const T & y() const { return (*this)[1]; }
	inline T & x() { return (*this)[0]; }
	inline T & y() { return (*this)[1]; }


	// Functions
	inline T magnitude() const {
		return sqrt(sqrMagnitude());
	};
	inline T sqrMagnitude() const {
		T value = 0;
		for (unsigned i = 0; i < self_type::Size; i++) {
			value += (*this)[i] * (*this)[i];
		}
		return value;
	};
	inline self_type & normalize() {
		return ((*this) = (T(1) / sqrt(sqrMagnitude())));
	};
	inline self_type normal() const {
		return self_type(*this).normalize();
	};
	

	// Static Functions
	inline static float angle(const self_type& from, const self_type& to) {
		return angle(to - from);
	};
	inline static float angle(const self_type& dir) {
		return atan2(dir[0], dir[1]);
	};
	inline static float det(const self_type& a, const self_type& b) {
		return (a[0] * b[1]) - (a[1] * b[0]);
	};
	inline static float distance(const self_type& a, const self_type& b) {
		return Vector2<float>(a - b).magnitude();
	};
	inline static float dot(const self_type& a, const self_type& b) {
		float value = 0;
		for (unsigned i = 0; i < self_type::Size; i++) {
			value += a[i] * b[i];
		}
		return value;
	};
	
	inline static self_type angle(float angle) {
		return self_type(Vector2<float>(cos(angle), sin(angle)));
	};
	inline static self_type direction(const self_type& from, const self_type& to) {
		return self_type(Vector2<float>(to - from)).normal();
	};
	inline static self_type lengthDir(float length, float dir) {
		return self_type(Vector2<float>::angle(dir) * length);
	};
	inline static self_type lerp(const self_type& a, const self_type& b, float t) {
		return self_type(Vector2<float>(a * t + b * (T(1) - t)));
	};
	inline static self_type reflect(const self_type& direction, const self_type& normal) {
		return (normal - direction) * (2 * dot(direction, normal));
	};

	inline friend bool operator==(const self_type& lhs, const self_type& rhs) {
		return (lhs[0] == rhs[0]) && (lhs[1] == rhs[1]);
	};
	inline friend bool operator!=(const self_type& lhs, const self_type& rhs) {
		return !(lhs == rhs);
	};
	
	inline friend self_type operator+(const self_type& lhs, const self_type& rhs) {
		return self_type(lhs[0] + rhs[0], lhs[1] + rhs[1]);
	};
	inline friend self_type operator-(const self_type& lhs, const self_type& rhs) {
		return self_type(lhs[0] - rhs[0], lhs[1] - rhs[1]);
	};
	inline friend self_type operator*(const self_type& lhs, const self_type& rhs) {
		return self_type(lhs[0] * rhs[0], lhs[1] * rhs[1]);
	};
	inline friend self_type operator/(const self_type& lhs, const self_type& rhs) {
		return self_type(lhs[0] / rhs[0], lhs[1] / rhs[1]);
	};
	inline friend self_type operator*(const self_type& lhs, const T& rhs) {
		return self_type(lhs[0] * rhs, lhs[1] * rhs);
	};
	inline friend self_type operator/(const self_type& lhs, const T& rhs) {
		return self_type(lhs[0] / rhs, lhs[1] / rhs);
	};
	inline friend self_type & operator+=(self_type& lhs, const self_type& rhs) {
		return (lhs = (lhs + rhs));
	};
	inline friend self_type & operator-=(self_type& lhs, const self_type& rhs) {
		return (lhs = (lhs - rhs));
	};
	inline friend self_type & operator*=(self_type& lhs, const self_type& rhs) {
		return (lhs = (lhs * rhs));
	};
	inline friend self_type & operator/=(self_type& lhs, const self_type& rhs) {
		return (lhs = (lhs / rhs));
	};
	inline friend self_type & operator*=(self_type& lhs, const T& rhs) {
		return (lhs = (lhs * rhs));
	};
	inline friend self_type & operator/=(self_type& lhs, const T& rhs) {
		return (lhs = (lhs / rhs));
	};
	inline friend self_type operator-(const self_type& rhs) {
		return rhs * static_cast<T>(-1);
	}

	// Constants
	static const self_type Zero;
	static const self_type One;
	static const self_type Up;
	static const self_type Down;
	static const self_type Left;
	static const self_type Right;
};


// Constants
template<typename T> const Vector2<T> Vector2<T>::Zero	= Vector2<T>( 0,  0);
template<typename T> const Vector2<T> Vector2<T>::One	= Vector2<T>( 1,  1);
template<typename T> const Vector2<T> Vector2<T>::Up	= Vector2<T>( 0,  1);
template<typename T> const Vector2<T> Vector2<T>::Down	= Vector2<T>( 0, -1);
template<typename T> const Vector2<T> Vector2<T>::Left	= Vector2<T>(-1,  0);
template<typename T> const Vector2<T> Vector2<T>::Right = Vector2<T>( 1,  0);


// Typedefs
using vec2f = Vector2<float>;
using vec2i = Vector2<int>;
using vec2d = Vector2<double>;
using vec2u = Vector2<unsigned>;

template <typename T>
using vec2 = Vector2<T>;

#endif