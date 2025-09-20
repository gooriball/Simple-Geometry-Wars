#ifndef VEC2_H
# define VEC2_H

# include <spdlog/spdlog.h>
# include <SFML/System/Vector2.hpp>

# include <cmath>

template<typename T, typename ReturnT = float>
class Vec2
{
	public:
		T	x_ = 0;
		T	y_ = 0;

		Vec2() = default;

		Vec2(const T x, const T y) :
			x_{x}, y_{y} {}

		operator sf::Vector2f () const
		{
			return (sf::Vector2f{x_, y_});
		}

		operator sf::Vector2i () const
		{
			return (sf::Vector2i{x_, y_});
		}

		operator Vec2<float>() const
		{
			return (Vec2<float>{static_cast<float>(x_), static_cast<float>(y_)});
		}

		operator Vec2<int>() const
		{
			return (Vec2<int>{static_cast<int>(x_), static_cast<int>(y_)});
		}
		
		Vec2<T>		operator + (const Vec2<T>& rhs) const
		{
			return (Vec2<T>{x_ + rhs.x_, y_ + rhs.y_});
		}

		Vec2<T>		operator - (const Vec2<T>& rhs) const
		{
			return (Vec2<T>{x_ - rhs.x_, y_ - rhs.y_});
		}

		Vec2<T>	operator * (const T val)
		{
			return (Vec2<T>{x_ * val, y_ * val});
		}

		Vec2<T>	operator / (const T val)
		{
			if (val == 0)
			{
				SPDLOG_ERROR("val must be non-zero!");
				return (Vec2<T>());
			}
			return (Vec2<T>{x_ / val, y_ / val});
		}

		Vec2<T>&	operator += (const Vec2<T>& rhs)
		{
			x_ += rhs.x_;
			y_ += rhs.y_;

			return (*this);
		}

		Vec2<T>&	operator -= (const Vec2<T>& rhs)
		{
			x_ -= rhs.x_;
			y_ -= rhs.y_;

			return (*this);
		}

		Vec2<T>&	operator *= (const T val)
		{
			x_ *= val;
			y_ *= val;

			return (*this);
		}

		Vec2<T>&	operator /= (const T val)
		{
			if (val == 0)
			{
				SPDLOG_ERROR("val must be non-zero!");
				*this = Vec2<T>();
				return (*this);
			}
			x_ /= val;
			y_ /= val;

			return (*this);
		}

		ReturnT		length() const
		{
			return (std::sqrt(x_ * x_ + y_ * y_));
		}

		ReturnT		lengthSquared() const
		{
			return (x_ * x_ + y_ * y_);
		}

		Vec2<T>&	normalize()
		{
			ReturnT len = length();
			if (len == 0)
			{
				SPDLOG_INFO("length must be non-zero!");
				*this = Vec2<T>();
				return (*this);
			}
			x_ /= len;
			y_ /= len;

			return (*this);
		}

		Vec2<T>		normalized() const
		{
			ReturnT len = length();
			if (len == 0)
			{
				SPDLOG_INFO("length must be non-zero!");
				return (Vec2<T>());
			}

			return (Vec2<T>{x_ / len, y_ / len});
		}

		ReturnT		dist(const Vec2<T>& rhs) const
		{
			float dx = (rhs.x_ - x_);
			float dy = (rhs.y_ - y_);

			return (std::sqrt(dx * dx + dy * dy));
		}

		ReturnT		distSquared(const Vec2<T>& rhs) const
		{
			float dx = (rhs.x_ - x_);
			float dy = (rhs.y_ - y_);

			return (dx * dx + dy * dy);
		}

		void		print() const
		{
			SPDLOG_INFO("({}, {})", x_, y_);
		}

};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;

#endif