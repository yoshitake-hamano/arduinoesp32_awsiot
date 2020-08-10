#include <unity.h>
#include <iostream>

void test_example()
{
    TEST_ASSERT_EQUAL(0, 0);
}

int main(int argc, char **argv)
{
    std::cout << "Hello platformio UT" << std::endl;
    UNITY_BEGIN();
    RUN_TEST(test_example);
    return UNITY_END();
}
