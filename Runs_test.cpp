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

TEST(RunsTest, LongerExample)
{
    std::vector<double> RR2 = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0};
    std::vector<int> annot2 = {1, 0, 0, 0, 0, 0, 0, 0};
    RRRuns RR(RR2, annot2, true);
    auto fullRuns = RR.getFullRuns();
    EXPECT_EQ(fullRuns.dec[1], 3);
    EXPECT_EQ(fullRuns.acc[1], 3);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}