#ifndef LAB1_POLYNOMIAL_H
#define LAB1_POLYNOMIAL_H

#include <iostream>
#include <sstream>
#include <cmath>

class Polynomial {
private:
    int lowestDegree_;
    int highestDegree_;
    int* coefficients_;

public:
    Polynomial();

    Polynomial(int, int, int []);

    Polynomial(const Polynomial &);

    Polynomial& operator=(const Polynomial &);

    Polynomial& sum(const Polynomial &polynomial, int option);

    Polynomial& operator+=(const Polynomial &);

    Polynomial& operator-=(const Polynomial &);

    Polynomial& operator*=(int);

    Polynomial& operator/=(int);

    Polynomial operator-();

    int operator[](int) const;

    int &operator[](int);

    std::string toString() const;

    std::string power(int) const;

    void refactor() const;

    double calculate(int n) const;

    friend Polynomial operator+(const Polynomial &, const Polynomial &);
    friend Polynomial operator-(const Polynomial &, const Polynomial &);
    friend Polynomial operator*(const Polynomial &, int);
    friend Polynomial operator*(int, const Polynomial &);
    friend Polynomial operator*(const Polynomial &, const Polynomial &);
    friend Polynomial operator/(const Polynomial &, int);
    friend bool operator==(const Polynomial &, const Polynomial &);
    friend std::ostream &operator<<(std::ostream &, const Polynomial &);
};


#endif //LAB1_POLYNOMIAL_H
