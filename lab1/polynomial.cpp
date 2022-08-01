#include "polynomial.h"
#include <algorithm>

Polynomial::Polynomial() : lowestDegree_(0), highestDegree_(0) {
    coefficients_ = new int[1]{0};
}

Polynomial::Polynomial(int lowestDegree, int highestDegree, int data[]) {
    lowestDegree_ = lowestDegree;
    highestDegree_ = highestDegree;
    int arraySize = highestDegree - lowestDegree + 1;
    coefficients_ = new int[arraySize];
    std::copy(data, data + arraySize, coefficients_);
    refactor();
}

Polynomial::Polynomial(const Polynomial &polynomial) : lowestDegree_(polynomial.lowestDegree_) , highestDegree_(polynomial.highestDegree_) {
    int size = highestDegree_ - lowestDegree_ + 1;
    coefficients_ = new int [size];
    std::copy(polynomial.coefficients_, polynomial.coefficients_ + size, coefficients_);
    refactor();
}

Polynomial& Polynomial::operator=(const Polynomial &polynomial) {
    if (this == &polynomial) return *this;
    lowestDegree_ = polynomial.lowestDegree_;
    highestDegree_ = polynomial.highestDegree_;
    int arraySize = highestDegree_ - lowestDegree_ + 1;
    coefficients_ = new int[arraySize];
    std::copy(polynomial.coefficients_, polynomial.coefficients_ + arraySize, coefficients_);
    refactor();
    return *this;
}

void Polynomial::refactor() const {
    auto *buffer = const_cast<Polynomial*>(this);
    int currentMin = lowestDegree_;

    while (currentMin <= highestDegree_ && coefficients_[currentMin - lowestDegree_] == 0)
        currentMin++;
    if (currentMin > highestDegree_) {
        delete [] buffer->coefficients_;
        buffer->coefficients_ = new int[1]{0};
        buffer->lowestDegree_ = 0;
        buffer->highestDegree_ = 0;
    }

    int currentMax = highestDegree_;

    while (currentMax >= currentMin && coefficients_[currentMax - lowestDegree_] == 0)
        currentMax--;

    int* buf = new int[currentMax - currentMin + 1];

    for (int i = currentMin; i <= currentMax; i++)
        buf[i - currentMin] = coefficients_[i - lowestDegree_];

    delete[] buffer->coefficients_;
    buffer->coefficients_ = new int[currentMax - currentMin + 1];
    std::copy(buf, buf + currentMax - currentMin + 1, coefficients_);
    delete [] buf;
    buffer->highestDegree_ = currentMax;
    buffer->lowestDegree_ = currentMin;
}

Polynomial & Polynomial::sum(const Polynomial &polynomial, int option) {
    int minDegree = (lowestDegree_ < polynomial.lowestDegree_) ? lowestDegree_ : polynomial.lowestDegree_;

    int maxDegree = (highestDegree_ > polynomial.highestDegree_) ? highestDegree_ : polynomial.highestDegree_;

    int *buf = new int[maxDegree - minDegree + 1];

    int *poly = buf;

    int *factors = coefficients_;
    int *othersFactors = polynomial.coefficients_;
    for (int i = minDegree; i <= maxDegree; i++) {
        *poly = 0;
        if (i >= lowestDegree_ and i <= highestDegree_) {
            *poly += *factors;
            factors++;
        }

        if (i >= polynomial.lowestDegree_ and i <= polynomial.highestDegree_) {
            *poly += *othersFactors * option;
            othersFactors++;
        }
        poly++;
    }

    delete [] coefficients_;

    coefficients_ = new int[maxDegree - minDegree + 1];

    std::copy(buf, buf + maxDegree - minDegree + 1, coefficients_);

    lowestDegree_ = minDegree;
    highestDegree_ = maxDegree;

    refactor();

    return *this;
}

Polynomial& Polynomial::operator+=(const Polynomial &polynomial) {
    sum(polynomial, 1);
    return *this;
}


Polynomial& Polynomial::operator-=(const Polynomial &polynomial)
{
    sum(polynomial, -1);
    return *this;
}

Polynomial& Polynomial::operator*=(int num) {
    std::for_each(coefficients_, coefficients_ + highestDegree_ - lowestDegree_ + 1, [num] (int &v) { v *= num;});
    refactor();
    return *this;
}

Polynomial& Polynomial::operator/=(int num) {
    std::for_each(coefficients_, coefficients_ + highestDegree_ - lowestDegree_ + 1, [num] (int &v) { v /= num;});
    refactor();
    return *this;
}

int Polynomial::operator[](int degree) const {
    if (degree < lowestDegree_ || degree > highestDegree_)
        return 0;
    return coefficients_[degree - lowestDegree_];
}

int &Polynomial::operator[](int i) {
    if (i > highestDegree_) {
        int *array = new int[i - lowestDegree_ + 1];
        for (int *j = array; j <= array + i; j++)
            *j = 0;
        int *p = array;
        for (int *j = coefficients_; j <= coefficients_ + highestDegree_ - lowestDegree_; j++, p++)
            *p = *j;
        delete[] coefficients_;
        highestDegree_ = i;
        coefficients_ = array;
    }
    if (i < lowestDegree_) {
        int *array = new int[highestDegree_ - i + 1];
        for (int *j = array; j <= array + highestDegree_ - i; j++)
            *j = 0;
        int *p = array - lowestDegree_;
        for (int *j = coefficients_; j <= coefficients_ + highestDegree_ - lowestDegree_; j++, p++)
            *p = *j;
        delete[] coefficients_;
        lowestDegree_= i;
        coefficients_ = array;
    }
    return coefficients_[i - lowestDegree_];
}



Polynomial operator+(const Polynomial& a, const Polynomial& b) {
    Polynomial buf(a);
    buf += b;
    buf.refactor();
    return buf;
}

Polynomial operator-(const Polynomial& a, const Polynomial& b) {
    Polynomial buf(a);
    buf -= b;
    buf.refactor();
    return buf;
}

Polynomial operator*(const Polynomial &polynomial, int num) {
    Polynomial buf(polynomial);
    buf *= num;
    buf.refactor();
    return buf;
}

Polynomial operator*(int num, const Polynomial &polynomial) {
    Polynomial buf(polynomial);
    buf *= num;
    buf.refactor();
    return buf;
}

Polynomial Polynomial::operator-() {
    Polynomial buf(lowestDegree_, highestDegree_, coefficients_);
    for (int i = 0; i < highestDegree_ - lowestDegree_ + 1; i++)
        buf.coefficients_[i] *= -1;
    buf.refactor();
    return buf;
}

Polynomial operator*(const Polynomial &a, const Polynomial &b) {
    if (a.lowestDegree_ == a.highestDegree_ and a.lowestDegree_ == 0)
        return {};
    if (b.lowestDegree_ == b.highestDegree_ and b.lowestDegree_ == 0)
        return {};
    int max = a.highestDegree_ + b.highestDegree_;
    int min = a.lowestDegree_ + b.lowestDegree_;
    int *array = new int[max - min + 1];
    for (int *i = array; i <= array + max - min; i++)
        *i = 0;
    for (int *i = a.coefficients_; i <= a.coefficients_ + a.highestDegree_ - a.lowestDegree_; i++)
        for (int *j = b.coefficients_; j <= b.coefficients_ + b.highestDegree_ - b.lowestDegree_; j++)
            *(array + (i - a.coefficients_) + (j - b.coefficients_)) += *i * *j;
    Polynomial poly(min, max, array);
    poly.refactor();
    return poly;
}

bool operator==(const Polynomial &a, const Polynomial &b) {
    Polynomial bufA(a);
    Polynomial bufB(b);

    bufA.refactor();
    bufB.refactor();

    if (bufA.lowestDegree_ != bufB.lowestDegree_ || bufA.highestDegree_ != bufB.highestDegree_)
        return false;
    else {
        for (int i = 0; i < bufA.highestDegree_ - bufA.lowestDegree_ + 1; i++)
            if (bufA.coefficients_[i] != bufB.coefficients_[i])
                return false;
    }
    return true;
}

Polynomial operator/(const Polynomial &p, int num) {
    Polynomial buf(p);
    buf/=num;
    buf.refactor();
    return buf;
}

double Polynomial::calculate(int x) const {
    int notNegativeDegree = 0;
    if (highestDegree_ > 0) {
        int minNotNegativeDegree = (lowestDegree_ > 0) ? lowestDegree_: 0;
        for (int i = highestDegree_; i >= minNotNegativeDegree; i--) {
            notNegativeDegree *= x;
            notNegativeDegree += coefficients_[i - lowestDegree_];
        }
    }

    double negativeDegree = 0;
    if (lowestDegree_ < 0) {
        int maxNegativeDegree = (highestDegree_ < 0) ? highestDegree_ : -1;

        for (int i = lowestDegree_; i <= maxNegativeDegree; i++) {
            negativeDegree += coefficients_[i - lowestDegree_];
            negativeDegree /= x;
        }
    }
    return notNegativeDegree + negativeDegree;
}


std::ostream &operator<<(std::ostream &out, const Polynomial &p) {
    out << p.toString();
    return out;
}


std::string Polynomial::power(int num) const {
    if (num > highestDegree_ or num < lowestDegree_)
        return "";
    if (coefficients_[num - lowestDegree_] == 0)
        return "";

    std::string answer;

    if (num != highestDegree_ and coefficients_[num - lowestDegree_] > 0)
        answer += "+";
    if (num == 0) {
        answer += std::to_string(coefficients_[num - lowestDegree_]);
        return answer;
    }
    else {
        switch (coefficients_[num - lowestDegree_]) {
            case 1:
                break;
            case -1:
                answer += "-";
                break;
            default:
                answer += std::to_string(coefficients_[num - lowestDegree_]);
                break;
        }
        answer += "x";
        if (num != 1)
            answer += "^" + std::to_string(num);
    }
    return answer;
}


std::string Polynomial::toString() const {
    refactor();
    std::string result;

    for (int i = highestDegree_; i >= lowestDegree_; i--)
        result += power(i);


    if (result.empty())
        return "0";
    return result;
}