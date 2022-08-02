#include <iostream>
#include "poly.hpp"
#include "gtest/gtest.h"

TEST(compTimePoly, test1) {
    int val = Polynomial<1, 2, 3>::calculate<3>::value;
    ASSERT_EQ(val, 34);
}

TEST(compTimePoly, test2) {
    int val = Polynomial<0, 0, 0, 0>::calculate<54>::value;
    ASSERT_EQ(val, 0);
}

TEST(compTimePoly, test3) {
    int val = Polynomial<2, 5, 4, 7>::calculate<4>::value;
    ASSERT_EQ(val, 534);
}

TEST(compTimePoly, test4) {
    int val = Polynomial<1>::calculate<9999>::value;
    ASSERT_EQ(val, 1);
}

int main() {
    testing::InitGoogleTest();
    int status = RUN_ALL_TESTS();
    std::cout << status << std::endl;

    Polynomial<1, 2, 3, 4, 5, 6, 7, 8, 9> polynomial;
    std::cout << polynomial << std::endl;
    return 0;
}
