export module Maths:Vector2;
import :Vector;
import <utility>;
import <stdexcept>;
import <algorithm>;

#pragma warning(push)
#pragma warning(disable: 4201)

namespace Maths
{
	export template<typename T>
	struct Vector<T, 2>
	{
	public:
		using Type = T;
		static constexpr size_t Length = 2;
		static constexpr size_t TypeSize = sizeof(T);
		static constexpr size_t TotalBytes = Length * TypeSize;

		constexpr Vector() noexcept
			= default;

		constexpr Vector(Type x, Type y) noexcept
			: X(x), Y(y)
		{}

		constexpr Vector(Type(&a)[2]) noexcept
			: X(a[0]), Y(a[1])
		{}

		template<typename U, size_t C>
		constexpr Vector(const Vector<U, C>& other) noexcept
		{
			*this = other;
		}

		template< typename U, size_t C>
		constexpr Vector(Vector<U, C>&& other) noexcept
		{
			*this = std::move(other);
		}

		~Vector() noexcept
			= default;

		// Returns vector times by -1 - does not reassign values (except w element)
		[[nodiscard]] constexpr Vector Reverse() const noexcept
		{
			return Vector{ -X, -Y };
		}

		[[nodiscard]] constexpr Vector Inverse() const noexcept
		{
			if constexpr (std::floating_point<Type>)
				return Vector(T(1) / X, T(1) / Y);
			else
				return *this;
		}

		// Sets all values of the vector to zero
		constexpr void Zero() noexcept
		{
			X = Y = T();
		}

		[[nodiscard]] constexpr bool IsZero() const noexcept
		{
			return X == 0 && Y == 0;
		}

		// Compilers earlier than C++20 will not work in constexpr
		[[nodiscard]] constexpr const Type* AsPtr() const
		{
			return Dimensions;
		}

		[[nodiscard]] constexpr Type* AsPtr()
		{
			return Dimensions;
		}

		[[nodiscard]] constexpr auto GetTotalBytes() const noexcept
		{
			return TotalBytes;
		}

		[[nodiscard]] constexpr auto GetLength() const noexcept
		{
			return Length;
		}

		// Gives a copy
		[[nodiscard]] constexpr Type At(const size_t index) const noexcept
		{
			return operator[](index);
		}

		// Gives a reference
		[[nodiscard]] constexpr Type& operator[](const size_t index)
		{
			if (index >= Length) throw std::out_of_range("Index must be between 0 and size of vector - 1!");
			return Dimensions[index];
		}

		// Gives a const reference
		[[nodiscard]] constexpr const Type& operator[](const size_t index) const
		{
			if (index >= Length) throw std::out_of_range("Index must be between 0 and size of vector - 1!");
			return Dimensions[index];
		}

		[[nodiscard]] constexpr Vector operator-() const noexcept requires !std::is_unsigned_v<T>
		{
			return Reverse();
		}

		template<typename U, size_t C>
		[[nodiscard]] constexpr Vector operator+(const Vector<U, C>& other) const noexcept
		{
			T copy[Length]{};
			constexpr auto loops = std::min(Length, C);
			for (size_t i = 0; i < loops; ++i)
			{
				copy[i] = static_cast<Type>(Dimensions[i] + other[i]);
			}
			std::ranges::copy(Dimensions + loops, std::ranges::end(Dimensions), copy + loops);
			return Vector(copy);
		}

		template<typename U, size_t C>
		[[nodiscard]] constexpr Vector operator-(const Vector<U, C>& other) const noexcept
		{
			Type copy[Length]{};
			constexpr auto loops = std::min(Length, C);
			for (size_t i = 0; i < loops; ++i)
			{
				copy[i] = static_cast<Type>(Dimensions[i] - other[i]);
			}
			std::ranges::copy(Dimensions + loops, std::ranges::end(Dimensions), copy + loops);
			return Vector(copy);
		}

		template<typename U, size_t C>
		[[nodiscard]] constexpr Vector operator*(const Vector<U, C>& other) const noexcept
		{
			Type copy[Length]{};
			constexpr auto loops = std::min(Length, C);
			for (size_t i = 0; i < loops; ++i)
			{
				copy[i] = static_cast<Type>(Dimensions[i] * other[i]);
			}
			std::ranges::fill(copy + loops, std::ranges::end(copy), 0);
			return Vector(copy);
		}

		template<typename U, size_t C>
		[[nodiscard]] constexpr Vector operator/(const Vector<U, C>& other) const noexcept
		{
			Type copy[Length]{};
			constexpr auto loops = std::min(Length, C);
			for (size_t i = 0; i < loops; ++i)
			{
				copy[i] = static_cast<Type>(Dimensions[i] / other[i]);
			}
			std::ranges::copy(Dimensions + loops, std::ranges::end(Dimensions), copy + loops);
			return Vector(copy);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		[[nodiscard]] constexpr Vector operator*(const U scalar) const noexcept
		{
			Type copy[Length]{};
			for (auto i = size_t(0); i < Length; ++i)
				copy[i] = static_cast<Type>(Dimensions[i] * scalar);
			return Vector(copy);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		[[nodiscard]] constexpr Vector operator/(const U scalar) const noexcept
		{
			Type copy[Length]{};
			for (auto i = size_t(0); i < Length; ++i)
				copy[i] = static_cast<Type>(Dimensions[i] / scalar);
			return Vector(copy);
		}

		template<typename U, size_t C>
		constexpr Vector& operator+=(const Vector<U, C>& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		template<typename U, size_t C>
		constexpr Vector operator-=(const Vector<U, C>& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		template<typename U>
		constexpr Vector operator*=(const U scalar) noexcept
		{
			*this = *this * scalar;
			return *this;
		}

		template<typename U, size_t C>
		constexpr Vector operator*=(const Vector<U, C>& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		template<typename U>
		constexpr Vector operator/=(const U scalar) noexcept
		{
			*this = *this / scalar;
			return *this;
		}

		template<typename U, size_t C>
		constexpr Vector operator/=(const Vector<U, C>& other) noexcept
		{
			*this = *this / other;
			return *this;
		}

		// bool operator == returns true if both Vector values are equal
		[[nodiscard]] constexpr bool operator==(const Vector& v) const
		{
			for (size_t i = 0; i < Length; ++i)
				if (Dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator == returns true if both Vector values are equal
		[[nodiscard]] constexpr bool operator==(Vector&& v) const
		{
			for (size_t i = 0; i < Length; ++i)
				if (Dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator != returns true if both Vector values are NOT equal
		[[nodiscard]] constexpr bool operator!=(const Vector& v) const
		{
			return !(*this == v);
		}

		// bool operator == returns true if both Vector values are equal
		template<typename U>
		[[nodiscard]] constexpr bool operator!=(const U scalar) const
		{
			return !(*this == scalar);
		}

		template<typename U, size_t C>
		constexpr Vector& operator=(const Vector<U, C>& other) noexcept // Copy
		{
			constexpr auto size = (Length < C) ? Length : C;

			for (auto i = 0; i < size; ++i)
				Dimensions[i] = static_cast<Type>(other[i]);

			return *this;
		}

		template<typename U, size_t C>
		constexpr Vector& operator=(Vector<U, C>&& other) noexcept // Move
		{
			if (this == &other)
				return *this;

			constexpr auto size = (Length < C) ? Length : C;

			for (auto i = 0; i < size; ++i)
				Dimensions[i] = std::move(other[i]);

			return *this;
		}

	public:
		union
		{
			struct
			{
				Type X, Y;
			};
			Type Dimensions[2];
		};
	};

	export template<typename T>
	using Vector2 = Vector<T, 2>;
}

#pragma warning(pop)
