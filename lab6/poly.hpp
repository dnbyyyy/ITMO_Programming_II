#ifndef LAB6_POLY_HPP
#define LAB6_POLY_HPP

template<int p, int q>
struct Power {
public:
    static const int value = p * Power<p, q - 1>::value;
};

template<int p>
struct Power<p, 0> {
public:
    static const int value = 1;
};

template <int... coefficients>
struct Polynomial {
private:
    template<int x, int p, int coefficient, int... others>
    struct step {
        static constexpr int value = Power<x, p>::value * coefficient + step<x, p + 1, others...>::value;
    };

    template<int x, int p, int coefficient>
    struct step<x, p, coefficient> {
        static constexpr int value = Power<x, p>::value * coefficient;
    };

public:
    template<int x>
    struct calculate {
        static constexpr int value = step<x, 0, coefficients...>::value;
    };
};

template<int p, int a, int... coefficients>
void toString(std::ostream& os, Polynomial<a, coefficients...>) {
    os << (a < 0 ? " - " : " + ");
    if (a != -1 && a != 1) {
        os << (a > 0 ? a : a * -1);
    }
    os << "X";
    if (p > 1) os << "^" << p;
    toString<p + 1, coefficients...>(os, Polynomial<coefficients...>());
}

template<int p, int a, int... coefficients>
void toString(std::ostream& os, Polynomial<0, coefficients...>) {
    toString<p + 1, coefficients...>(os, Polynomial<coefficients...>());
}

template<int p, int a, int... coefficients>
void toString(std::ostream& os, Polynomial<0, a>) {
    os << (a < 0 ? " - " : " + ");
    if (a != -1 && a != 1) os << (a > 0 ? a : a * -1);
    os << "X^" << p;
}

template<int p>
void toString(std::ostream& os, Polynomial<>) {}


template<int a, int... coefficients>
std::ostream& operator<<(std::ostream& os, Polynomial<a, coefficients...> polynomial) {
    os << (a < 0 ? "-" : "") << a;
    toString<1, coefficients...>(os, Polynomial<coefficients...>());
    return os;
}
#endif //LAB6_POLY_HPP
