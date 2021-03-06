//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "exhaustive.hpp"
#include "permutations.hpp"
#include "logger.hpp"

scp::Solution scp::exhaustive::solve_ram(const scp::Problem& problem)
{
	const auto start = std::chrono::system_clock::now();

	Solution best_solution(problem);
	Solution current_solution(problem);
	std::vector<std::vector<dynamic_bitset<>>> permutations =
	  generate_permutations(problem.subsets_costs.size());
	for(auto& i_bits_permutations: permutations)
	{
		for(const dynamic_bitset<>& permutation: i_bits_permutations)
		{
			current_solution.selected_subsets = permutation;
			current_solution.compute_cover();
			if((!best_solution.cover_all_points) || (current_solution.cost < best_solution.cost))
			{
				current_solution.compute_cost();
				if(current_solution.cost < best_solution.cost)
				{
					best_solution = current_solution;
				}
			}
		}
	}

	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	SPDLOG_LOGGER_DEBUG(
	  LOGGER,
	  "found optimal solution by (ram expensive) exhaustive search with {} subsets in {}s",
	  best_solution.selected_subsets.count(),
	  elapsed_seconds.count());
	return best_solution;
}
scp::Solution scp::exhaustive::solve_cpu(const scp::Problem& problem)
{
	const auto start = std::chrono::system_clock::now();
	Solution best_solution(problem);
	Solution current_solution(problem);

	for(size_t bits_on = 0; bits_on <= problem.subsets_costs.size(); ++bits_on)
	{
		PermutationsGenerator generator(problem.subsets_costs.size(), bits_on);
		while(!generator.finished())
		{
			current_solution.selected_subsets = generator.next();
			current_solution.compute_cover();
			if((!best_solution.cover_all_points) || (current_solution.cost < best_solution.cost))
			{
				current_solution.compute_cost();
				if(current_solution.cost < best_solution.cost)
				{
					best_solution = current_solution;
				}
			}
		}
	}

	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	SPDLOG_LOGGER_DEBUG(
	  LOGGER,
	  "found optimal solution by (cpu expensive) exhaustive search with {} subsets in {}s",
	  best_solution.selected_subsets.count(),
	  elapsed_seconds.count());
	return best_solution;
}

scp::Solution scp::exhaustive::solve_counter(const scp::Problem& problem)
{
	const auto start = std::chrono::system_clock::now();
	Solution best_solution(problem);
	Solution current_solution(problem);

	do
	{
		increment(current_solution.selected_subsets);
		current_solution.compute_cover();
		if(current_solution.cover_all_points)
		{
			current_solution.compute_cost();
			if((!best_solution.cover_all_points) || (current_solution.cost < best_solution.cost))
			{
				best_solution = current_solution;
			}
		}
	} while(!current_solution.selected_subsets.none());

	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	SPDLOG_LOGGER_DEBUG(
	  LOGGER,
	  "found optimal solution by (counter) exhaustive search with {} subsets in {}s",
	  best_solution.selected_subsets.count(),
	  elapsed_seconds.count());
	return best_solution;
}
