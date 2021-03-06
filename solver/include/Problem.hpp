//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_PROBLEM_HPP
#define SCPSOLVER_PROBLEM_HPP

#include <dynamic_bitset.hpp>
#include <spdlog/fmt/ostr.h> // for operator<<

#include <filesystem>
#include <ostream>
#include <random>

namespace scp
{
	struct Problem final
	{
		dynamic_bitset<> full_set; // all bits to 1
		std::vector<dynamic_bitset<>> subsets_points;
		std::vector<size_t> subsets_costs;

		Problem() = default;
		Problem(const Problem&) = default;
		Problem(Problem&&) noexcept = default;
		Problem& operator=(const Problem&) = default;
		Problem& operator=(Problem&&) noexcept = default;
	};
	std::ostream& operator<<(std::ostream& os, const Problem& problem);

	bool read_problem(const std::filesystem::path& path, Problem& problem);

	bool write_problem(const Problem& problem,
	                   const std::filesystem::path& path,
	                   bool override_file = false);

	Problem generate_problem(size_t points_number,
	                         size_t subsets_number,
	                         std::default_random_engine& generator,
	                         size_t min_covering_subsets,
	                         size_t max_covering_subset,
	                         size_t min_subsets_cost,
	                         size_t max_subsets_cost);

	Problem generate_problem(size_t points_number,
	                         size_t subsets_number,
	                         std::default_random_engine& generator);
} // namespace scp

#endif //SCPSOLVER_PROBLEM_HPP
