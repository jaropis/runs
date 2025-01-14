#include "Runs.h"
#include <gtest/gtest.h>

TEST(RunsTest, SimpleExample)
{
    std::vector<double> RR1 = {1.0, 0.0, 1.0, 0.0};
    std::vector<int> annot1 = {1, 0, 0, 0};
    RRRuns RR(RR1, annot1, true);

    auto fullRuns = RR.getFullRuns();
    EXPECT_EQ(fullRuns.dec[1], 1);
}

TEST(RunsTest, LongerExample1)
{
    std::vector<double> RR2 = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0};
    std::vector<int> annot2 = {1, 0, 0, 0, 0, 0, 0, 0};
    RRRuns RR(RR2, annot2, true);
    auto fullRuns = RR.getFullRuns();
    EXPECT_EQ(fullRuns.dec[1], 3);
    EXPECT_EQ(fullRuns.acc[1], 3);
}

TEST(RunsTest, LongerExample2)
{
    std::vector<double> RR_data = {1.0, 2.0, 3.0, 0.0, 1.0, 2.0, 3.0, 0.0};
    std::vector<int> annot_data = {0, 0, 0, 0, 0, 0, 0, 0};
    RRRuns RR(RR_data, annot_data, true);
    auto fullRuns = RR.getFullRuns();
    EXPECT_EQ(fullRuns.dec[1], 0);
    EXPECT_EQ(fullRuns.dec[2], 1);
    EXPECT_EQ(fullRuns.dec[3], 1);
    EXPECT_EQ(fullRuns.acc[1], 2);
    EXPECT_EQ(fullRuns.acc[2], 0);
    EXPECT_EQ(fullRuns.dec[3], 0);
    std::cout << "LongerTest2" << std::endl;
    RR.printRuns();
}

TEST(RunsTest, LongerExample3)
{ // clang-format off
    std::vector<double> RR_data = {1.0, 2.0, 3.0, 0.0, 1.0, 2.0, 3.0, 0.0};
    std::vector<int> annot_data = {0,   0,   1,   0,   0,   1,   0,   0};
    // clang-format on
    RRRuns RR(RR_data, annot_data, true);
    auto fullRuns = RR.getFullRuns();
    EXPECT_EQ(fullRuns.dec[1], 2);
    EXPECT_EQ(fullRuns.dec[2], 0);
    EXPECT_EQ(fullRuns.acc[1], 1);
    EXPECT_EQ(fullRuns.acc[2], 0);
    EXPECT_EQ(fullRuns.dec[3], 0);
    std::cout << "LongerTest3" << std::endl;
    RR.printRuns();
}
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}