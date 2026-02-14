export module Maths:Vector;
import <utility>;
import <stdexcept>;
import <algorithm>;

namespace Maths
{
	export template<typename T, size_t N>
	struct Vector
	{
	public:
		static_assert(N > 0, "Must have at least one dimension to use mathematical vectors");

		using Type = T;
		static constexpr size_t Length = N;
		static constexpr size_t TypeSize = sizeof(T);
		static constexpr size_t TotalBytes = Length * TypeSize;
		
		constexpr Vector() noexcept
			= default;

		template< typename U, size_t C>
		constexpr Vector(const Vector<U, C>& other) noexcept
		{
			*this = other;
		}

		template< typename U, size_t C>
		constexpr Vector(Vector<U, C>&& other) noexcept
		{
			*this = std::move(other);
		}

		explicit constexpr Vector(const T (&values)[N])
		{
			for (auto i = 0; i < N; ++i)
				Dimensions[i] = values[i];
		}

		~Vector() noexcept
			= default;

		template<typename U, size_t C>
		[[nodiscard]] constexpr Type DotProduct(const Vector<U, C>& other) const noexcept
		{
			constexpr auto size = (N < C) ? N : C;
			auto dp = Type();
			for (auto i = 0; i < size; ++i)
				dp += Dimensions[i] * static_cast<Type>(other[i]);
			return dp;
		}

		// Returns vector times by -1 - does not reassign values (except w element)
		[[nodiscard]] constexpr Vector Reverse() const noexcept
		{
			Type copy[N]{ Type() };
			for (auto i = 0; i < N; ++i)
				copy[i] = Dimensions[i] * -1;
			return Vector(copy);
		}

		[[nodiscard]] constexpr Vector Inverse() const noexcept
		{
			if constexpr(std::is_integral_v<Type>)
				return *this;
			else
			{
				T copy[N]{ T() };
				for (auto i = 0; i < N; ++i)
					copy[i] = T(1) / Dimensions[i];
				return Vector(copy);
			}
		}

		// Sets all values of the vector to zero
		constexpr void Zero() noexcept
		{
			for (auto i = 0; i < N; ++i)
				Dimensions[i] = Type();
		}

		[[nodiscard]] constexpr bool IsZero() const noexcept
		{
			for (auto val : Dimensions)
				if (val != Type())
					return false;
			return true;
		}

		// Compilers earlier than C++20 will not work in constexpr
		[[nodiscard]] constexpr const Type* AsPtr() const
		{
			return std::addressof(Dimensions[0]);
		}
		
		[[nodiscard]] constexpr Type* AsPtr()
		{
			return std::addressof(Dimensions[0]);
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
			if (index >= N) throw std::out_of_range("Index must be between 0 and size of vector - 1!");
			return Dimensions[index];
		}

		// Gives a const reference
		[[nodiscard]] constexpr const Type& operator[](const size_t index) const
		{
			if (index >= N) throw std::out_of_range("Index must be between 0 and size of vector - 1!");
			return Dimensions[index];
		}

		[[nodiscard]] constexpr Vector operator-() const noexcept requires !std::is_unsigned_v<T>
		{
			return Reverse();
		}

		template<typename U, size_t C>
		[[nodiscard]] constexpr Vector operator+(const Vector<U, C>& other) const noexcept
		{
			T copy[N]{};
			const auto loops = std::min(N, C);
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
			Type copy[N]{};
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
			Type copy[N]{};
			constexpr auto loops = std::min(Length, C);
			for (size_t i = 0; i < loops; ++i)
			{
				copy[i] = static_cast<Type>(Dimensions[i] * other[i]);
			}
			std::ranges::fill(Dimensions + loops, std::ranges::end(Dimensions), T(0));
			return Vector(copy);
		}

		template<typename U, size_t C>
		[[nodiscard]] constexpr Vector operator/(const Vector<U, C>& other) const noexcept
		{
			Type copy[N]{};
			constexpr auto loops = std::min(Length, C);
			for (size_t i = 0; i < loops; ++i)
			{
				copy[i] = static_cast<Type>(Dimensions[i] / other[i]);
			}
			std::ranges::fill(Dimensions + loops, std::ranges::end(Dimensions), T(0));
			return Vector(copy);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		[[nodiscard]] constexpr Vector operator*(const U scalar) const noexcept
		{
			Type copy[N]{};
			for (auto i = size_t(0); i < N; ++i)
				copy[i] = static_cast<Type>(Dimensions[i] * scalar);
			return Vector(copy);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		[[nodiscard]] constexpr Vector operator/(const U scalar) const noexcept
		{
			Type copy[N]{};
			for (auto i = size_t(0); i < N; ++i)
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
			for (size_t i = 0; i < N; ++i)
				if (Dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator == returns true if both Vector values are equal
		[[nodiscard]] constexpr bool operator==(Vector&& v) const
		{
			for (size_t i = 0; i < N; ++i)
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
			constexpr auto size = (N < C) ? N : C;

			for (auto i = 0; i < size; ++i)
				Dimensions[i] = static_cast<Type>(other[i]);

			return *this;
		}

		template<typename U, size_t C>
		constexpr Vector& operator=(Vector<U, C>&& other) noexcept // Move
		{
			if (this == &other)
				return *this;
			
			constexpr auto size = (N < C) ? N : C;

			for (auto i = 0; i < size; ++i)
				Dimensions[i] = std::move(other[i]);

			return *this;
		}

	private:
		Type Dimensions[N]{};
	};
}
