#ifndef LAB1_GEOMETRY_H
#define LAB1_GEOMETRY_H

#include <vector>
#include <cmath>

class Point {
private:
    double x_, y_;

public:
    explicit Point(int x = 0, int y = 0) : x_ (x), y_ (y) {}

    Point(const Point& other) = default;

    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
    double distanceToPoint(const Point& point) const;
    double squaredDistanceToPoint(const Point& point) const;

    Point& operator=(const Point &point);

    bool operator==(const Point& point) const;
};

class PolyLine {
protected:
    std::vector<Point> points_;
public:
    explicit PolyLine(unsigned int pointsCnt = 0, Point *p = {});

    PolyLine(const PolyLine& polyLine);

    virtual ~PolyLine() = default;

    unsigned int getPointsCnt() const;
    Point getPoint(unsigned int pointNumber) const;
    virtual double perimeter() const;

    PolyLine& operator=(const PolyLine& polyLine);
    bool operator==(const PolyLine& polyLine) const;
};

class ClosedPolyLine : public PolyLine {
public:
    using PolyLine::PolyLine;

    double perimeter() const override;
};

class Polygon : public ClosedPolyLine {
public:
    using ClosedPolyLine::ClosedPolyLine;

    virtual double area() const;
};

class Triangle : public Polygon {
public:
    using Polygon::Polygon;

    bool hasRightAngle() const;
};

class Trapezoid : public Polygon
{
    using Polygon::Polygon;
};

class RegularPolygon : public Polygon {
public:
    using Polygon::Polygon;

    double perimeter() const override;
    double area() const override;
};

#endif //LAB1_GEOMETRY_H
