// -*- coding: utf-16 -*-
#pragma once

/*! @file include/fractions.hpp
 *  This is a C++ Library header.
 */

// #include <boost/operators.hpp>
// #include <cmath>
#include <numeric>
#include <type_traits>
#include <utility>

#include "common_concepts.h"

namespace fun {

    /**
     * @brief absolute (for unsigned)
     *
     * @tparam T
     * @param a
     * @return T
     */
    template <typename T>
    requires std::is_unsigned_v<T>
    inline constexpr auto abs(const T& a) -> T { return a; }

    /**
     * @brief absolute
     *
     * @tparam T
     */
    template <typename T>
    requires(!std::is_unsigned_v<T> && ordered_ring<T>) inline constexpr auto abs(const T& a) -> T {
        return (a < T(0)) ? -a : a;
    }

    /*!
     * @brief Greatest common divider
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <Integral _Mn> inline constexpr auto gcd_recur(_Mn __m, _Mn __n) -> _Mn {
        if (__n == 0) {
            return abs(__m);
        }
        return gcd_recur(__n, __m % __n);
    }

    /*!
     * @brief Greatest common divider
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <Integral _Mn> inline constexpr auto gcd(_Mn __m, _Mn __n) -> _Mn {
        if (__m == 0) {
            return abs(__n);
        }
        return gcd_recur(__m, __n);
    }

    /*!
     * @brief Least common multiple
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <Integral _Mn> inline constexpr auto lcm(_Mn __m, _Mn __n) -> _Mn {
        if (__m == 0 || __n == 0) {
            return 0;
        }
        return (abs(__m) / gcd(__m, __n)) * abs(__n);
    }

    template <Integral Z> struct Fraction {
        Z _num;
        Z _den;

        /*!
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         * @param[in] den
         */
        constexpr Fraction(Z num, Z den) : _num{std::move(num)}, _den{std::move(den)} {
            this->normalize();
        }

        constexpr void normalize() {
            Z common = gcd(this->_num, this->_den);
            if (common == Z(1) || common == Z(0)) {
                return;
            }
            if (this->_den < Z(0)) {
                common = -common;
            }
            this->_num /= common;
            this->_den /= common;
        }

        /*!
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr explicit Fraction(Z&& num) : _num{std::move(num)}, _den(Z(1)) {}

        /*!
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr explicit Fraction(const Z& num) : _num{num}, _den(1) {}

        /*!
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr Fraction() : _num(0), _den(1) {}

        /*!
         * @brief
         *
         * @return const Z&
         */
        [[nodiscard]] constexpr auto num() const noexcept -> const Z& { return _num; }

        /*!
         * @brief
         *
         * @return const Z&
         */
        [[nodiscard]] constexpr auto den() const noexcept -> const Z& { return _den; }

        /** @name Comparison operators
         *  ==, !=, <, >, <=, >= etc.
         */
        ///@{

        /**
         * @brief
         *
         * @tparam U
         * @param rhs
         * @return true
         * @return false
         */
        template <typename U>  //
        constexpr auto operator==(const Fraction<U>& rhs) const -> bool {
            if (this->_den == rhs._den) {
                return this->_num == rhs._num;
            }
            return this->_num * rhs._den == this->_den * rhs._num;
        }

        /**
         * @brief
         *
         * @tparam U
         * @param rhs
         * @return true
         * @return false
         */
        template <typename U>  //
        constexpr auto operator<(const Fraction<U>& rhs) const -> bool {
            if (this->_den == rhs._den) {
                return this->_num < rhs._num;
            }
            return this->_num * rhs._den < this->_den * rhs._num;
        }

        /**
         * @brief
         *
         * @tparam U
         * @param rhs
         * @return true
         * @return false
         */
        template <typename U>  //
        constexpr auto operator!=(const Fraction<U>& rhs) const -> bool {
            return !(*this == rhs);
        }

        /**
         * @brief Greater than
         *
         * @tparam U
         * @param rhs
         * @return true
         * @return false
         */
        template <typename U>  //
        constexpr auto operator>(const Fraction<U>& rhs) const -> bool {
            return !(rhs < *this);
        }

        /**
         * @brief
         *
         * @tparam U
         * @param rhs
         * @return true
         * @return false
         */
        template <typename U>  //
        constexpr auto operator>=(const Fraction<U>& rhs) const -> bool {
            return !(*this < rhs);
        }

        /**
         * @brief
         *
         * @tparam U
         * @param rhs
         * @return true
         * @return false
         */
        template <typename U>  //
        constexpr auto operator<=(const Fraction<U>& rhs) const -> bool {
            return !(rhs > *this);
        }

        /**
         * @brief
         *
         * @param rhs
         * @return true
         * @return false
         */
        constexpr auto operator==(const Z& rhs) const -> bool {
            return this->_num == this->_den * rhs;
        }

        /**
         * @brief Less than
         *
         * @param rhs
         * @return true
         * @return false
         */
        constexpr auto operator<(const Z& rhs) const -> bool {
            return this->_num < this->_den * rhs;
        }

        /**
         * @brief
         *
         * @param rhs
         * @return true
         * @return false
         */
        constexpr auto operator>(const Z& rhs) const -> bool { return !(*this < rhs); }

        /**
         * @brief
         *
         * @param rhs
         * @return true
         * @return false
         */
        constexpr auto operator<=(const Z& rhs) const -> bool { return !(rhs < *this); }

        /**
         * @brief
         *
         * @param rhs
         * @return true
         * @return false
         */
        constexpr auto operator>=(const Z& rhs) const -> bool { return !(*this < rhs); }

        ///@}

        /*!
         * @brief
         *
         * @return Fraction
         */
        [[nodiscard]] constexpr auto abs() const -> Fraction {
            return Fraction(abs(this->_num), abs(this->_den));
        }

        /*!
         * @brief
         *
         */
        constexpr void reciprocal() noexcept(std::is_nothrow_swappable_v<Z>) {
            std::swap(this->_num, this->_den);
        }

        /*!
         * @brief
         *
         * @return Fraction
         */
        constexpr auto operator-() const -> Fraction {
            auto res = Fraction(*this);
            res._num = -res._num;
            return res;
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator+(const Fraction& frac) const -> Fraction {
            if (this->_den == frac._den) {
                return Fraction(this->_num + frac._num, this->_den);
            }
            auto d = this->_den * frac._den;
            auto n = frac._den * this->_num + this->_den * frac._num;
            return Fraction(std::move(n), std::move(d));
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator-(const Fraction& frac) const -> Fraction { return *this + (-frac); }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         * @todo gcd first
         */
        constexpr auto operator*(const Fraction& frac) const -> Fraction {
            auto n = this->_num * frac._num;
            auto d = this->_den * frac._den;
            return Fraction(std::move(n), std::move(d));
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator/(Fraction frac) const -> Fraction {
            frac.reciprocal();
            return *this * frac;
        }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator+(const Z& i) const -> Fraction {
            auto n = this->_num + this->_den * i;
            return Fraction(std::move(n), this->_den);
        }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator-(const Z& i) const -> Fraction { return *this + (-i); }

        // /*!
        //  * @brief
        //  *
        //  * @param[in] i
        //  * @return Fraction
        //  */
        // constexpr Fraction operator*(const Z& i) const noexcept
        // {
        //     auto n = _num * i;
        //     return Fraction(n, _den);
        // }

        // /*!
        //  * @brief
        //  *
        //  * @param[in] i
        //  * @return Fraction
        //  */
        // constexpr Fraction operator/(const Z& i) const noexcept
        // {
        //     auto d = _den * i;
        //     return Fraction(_num, d);
        // }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator+=(const Fraction& frac) -> Fraction& {
            return *this = *this + frac;
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator-=(const Fraction& frac) -> Fraction& {
            return *this = *this - frac;
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator*=(const Fraction& frac) -> Fraction& {
            return *this = *this * frac;
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        constexpr auto operator/=(const Fraction& frac) -> Fraction& {
            return *this = *this / frac;
        }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator+=(const Z& i) -> Fraction& { return *this = *this + i; }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator-=(const Z& i) -> Fraction& { return *this = *this - i; }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator*=(const Z& i) -> Fraction& {
            const auto common = gcd(i, this->_den);
            if (common == Z(1)) {
                this->_num *= i;
            }
            // else if (common == Z(0)) [[unlikely]] // both i and den are zero
            // {
            //     this->_num = Z(0);
            // }
            else {
                this->_num *= (i / common);
                this->_den /= common;
            }
            return *this;
        }

        /*!
         * @brief
         *
         * @param[in] frac
         * @return Fraction
         */
        friend constexpr auto operator*(Fraction f, const Z& i) -> Fraction { return f *= i; }

        /*!
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator/=(const Z& i) -> Fraction& {
            const auto common = gcd(this->_num, i);
            if (common == Z(1)) {
                this->_den *= i;
            }
            // else if (common == Z(0)) [[unlikely]] // both i and num are zero
            // {
            //     this->_den = Z(0);
            // }
            else {
                this->_den *= (i / common);
                this->_num /= common;
            }
            return *this;
        }

        // /*!
        //  * @brief
        //  *
        //  * @return double
        //  */
        // constexpr explicit operator double())
        // {
        //     return double(_num) / _den;
        // }
    };

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator+(const Z& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return frac + c;
    }

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator-(const Z& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return c + (-frac);
    }

    // /*!
    //  * @brief
    //  *
    //  * @param[in] c
    //  * @param[in] frac
    //  * @return Fraction<Z>
    //  */
    // template <typename Z>
    // constexpr Fraction<Z> operator*(const Z& c, const Fraction<Z>& frac)
    // {
    //     return frac * c;
    // }

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator+(int&& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return frac + Z(c);
    }

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator-(int&& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return (-frac) + Z(c);
    }

    /*!
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    template <typename Z> constexpr auto operator*(int&& c, const Fraction<Z>& frac)
        -> Fraction<Z> {
        return frac * Z(c);
    }

    /*!
     * @brief
     *
     * @tparam _Stream
     * @tparam Z
     * @param[in] os
     * @param[in] frac
     * @return _Stream&
     */
    // template <typename _Stream, typename Z> auto operator<<(_Stream& os, const Fraction<Z>& frac)
    //     -> _Stream& {
    //     os << frac.num() << "/" << frac.den();
    //     return os;
    // }

    // For template deduction
    // Integral{Z} Fraction(const Z &, const Z &) noexcept -> Fraction<Z>;

}  // namespace fun
