/**
 * @file Runs.h
 * @brief Header file defining the RRRuns class and related structures for processing RR intervals.
 */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

/**
 * @enum RunType
 * @brief Enumeration for different types of runs.
 */
enum RunType
{
    DEC = 1, ///< Deceleration run
    NEU = 0, ///< Neutral run
    ACC = -1 ///< Acceleration run
};

/**
 * @struct RunsAccumulator
 * @brief Holds statistics and addresses for different types of runs.
 */
struct RunsAccumulator
{
    std::vector<int> dec;                         ///< Accumulates statistics for deceleration runs.
    std::vector<int> acc;                         ///< Accumulates statistics for acceleration runs.
    std::vector<int> neu;                         ///< Accumulates statistics for neutral runs.
    std::vector<std::vector<int>> runs_addresses; ///< Stores addresses of runs: [end address, length, type].
};

/**
 * @class RRRuns
 * @brief Analyzes and accumulates statistics for RR interval runs.
 */
class RRRuns
{
public:
    /**
     * @brief Constructor for RRRuns.
     * @param RR Vector of RR interval data.
     * @param annot Vector of annotations corresponding to the RR data.
     * @param writeLastRun Flag to indicate whether to write the last run.
     */
    RRRuns(std::vector<double> RR, std::vector<int> annot, bool writeLastRun);

    /**
     * @brief Retrieves the accumulated statistics for all runs.
     * @return A RunsAccumulator object containing the statistics.
     */
    RunsAccumulator getFullRuns();

    /**
     * @brief Prints the run statistics in a formatted manner.
     */
    void printRuns();

    /**
     * @brief Prints addresses of runs based on type and length.
     * @param runType Type of run to filter out (DEC, NEU, ACC).
     * @param runLength Length of run to filter out.
     * @param referenceBeat Whether to include the reference beat.
     */
    void printAddresses(RunType runType, int runLength, bool referenceBeat);

private:
    std::vector<double> rrIntervals; ///< Vector storing RR interval data.
    std::vector<int> annotations;    ///< Vector storing annotations corresponding to RR data.
    bool writeLastRun;               ///< Flag to indicate whether to write the last run.

    RunsAccumulator accumulator; ///< Accumulator object for storing run statistics.
    int getNonzeroLength(std::vector<int> vec);
    /**
     * @brief Updates the run addresses in the accumulator.
     * @param new_entry A vector containing [end address, length, type].
     */
    void updateRunsAddresses(std::vector<int> new_entry);

    /**
     * @brief Analyzes the RR intervals and updates run statistics.
     */
    void analyzeRuns();

    bool analyzed_ = false; ///< Flag to indicate whether the data has been analyzed.
};
