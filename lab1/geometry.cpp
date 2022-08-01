#include "geometry.h"
#include <cmath>


Point& Point::operator=(const Point &point) = default;

bool Point::operator==(const Point &point) const {
    return (x_ == point.x_ && y_ == point.y_);
}

double Point::getX() const {
    return x_;
}

double Point::getY() const {
    return y_;
}

void Point::setX(double x) {
    x_ = x;
}

void Point::setY(double y) {
    y_ = y;
}

double Point::distanceToPoint(const Point &point) const {
    return (sqrt(squaredDistanceToPoint(point)));
}

double Point::squaredDistanceToPoint(const Point &point) const {
    return pow(getX() - point.getX(), 2) + pow(getY() - point.getY(), 2);
}

PolyLine::PolyLine(unsigned int pointsCnt, Point *p) {
    for (unsigned int i = 0; i < pointsCnt; i++) {
        points_.push_back(p[i]);
    }
}

PolyLine::PolyLine(const PolyLine& polyLine) {
    points_ = polyLine.points_;
}

PolyLine& PolyLine::operator=(const PolyLine &polyLine) = default;

unsigned int PolyLine::getPointsCnt() const {
    return points_.size();
}

Point PolyLine::getPoint(unsigned int pointNumber) const {
    return points_.at(pointNumber);
}

bool PolyLine::operator==(const PolyLine &polyLine) const {
    if (getPointsCnt() == polyLine.getPointsCnt()) {
        for (unsigned int i = 0; i < getPointsCnt(); i++) {
            if (!(getPoint(i) == polyLine.getPoint(i))) return false;
        }
        return true;
    }
    return false;
}

double PolyLine::perimeter() const {
    double perimeter = 0;
    for (unsigned int i = 0; i < getPointsCnt() - 1; i++)
    {
        perimeter += getPoint(i).distanceToPoint(getPoint(i + 1));
    }
    return perimeter;
}

double ClosedPolyLine::perimeter() const {
    return PolyLine::perimeter() + getPoint(getPointsCnt() - 1).distanceToPoint(getPoint(0));
}



double Polygon::area() const {
    double area = 0;

    for (unsigned int i = 0; i < getPointsCnt() - 1; i++) {
        area += (getPoint(i).getX() * getPoint(i + 1).getY() - getPoint(i + 1).getX() * getPoint(i).getY());
    }

    area += (getPoint(getPointsCnt() - 1).getX() * getPoint(0).getY() - getPoint(0).getX() * getPoint(getPointsCnt() - 1).getY());

    return (std::abs(area)) / 2.0;
}



bool Triangle::hasRightAngle() const {
    double aX = getPoint(1).getX() - getPoint(0).getX();
    double aY = getPoint(1).getY() - getPoint(0).getY();

    double bX = getPoint(2).getX() - getPoint(1).getX();
    double bY = getPoint(2).getY() - getPoint(1).getY();

    double cX = getPoint(0).getX() - getPoint(2).getX();
    double cY = getPoint(0).getY() - getPoint(2).getY();

    return ((aX + aY) * bY == 0 || (bX + bY) * cY == 0 || (cX + cY) * aY == 0);
}


double RegularPolygon::area() const {
    if (points_.size() > 2) {
        double squaredSide = getPoint(0).squaredDistanceToPoint(getPoint(1));

        return ((double) points_.size() * squaredSide) / (4 * tan(M_PI / (double) points_.size()));
    }
    return 0;
}

double RegularPolygon::perimeter() const {
    if (points_.size() > 1) {
        double side = getPoint(0).distanceToPoint(getPoint(1));
        return (double) points_.size() * side;
    }
    return 0;
}