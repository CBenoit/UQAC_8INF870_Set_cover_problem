//
// Created by Benoit Cortier on 3/12/19.
//
#include "descent.hpp"
#include "neighbouring.hpp"

#include <cmath>
#include <memory>

std::ostream& scp::descent::operator<<(std::ostream& os, const scp::descent::Config& conf)
{
	os << "scp::descent::Config{\n";
	os << "\tnumber of iterations = " << conf.iterations_number << ",\n";
	os << "\tinitial temperature = " << conf.initial_temperature << ",\n";
	os << "\tfinal temperature = " << conf.final_temperature << ",\n";
	os << "}\n";
	return os;
}

scp::Solution scp::descent::improve_by_annealing(const Solution& initial_solution,
                                                 std::default_random_engine& generator,
                                                 const Config& conf)
{
	assert(conf.initial_temperature > conf.final_temperature);
	assert(initial_solution.cover_all_points);

	const double initial_to_final_range = conf.final_temperature - conf.initial_temperature;
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	Solution best = initial_solution;
	best.compute_cost();
	Solution solution = initial_solution;

	for(size_t iter = 0; iter < conf.iterations_number; ++iter)
	{
		solution.compute_cost();
		const size_t previous_cost = solution.cost;

		size_t flipped_bit = neighbouring::flip_bit_safe(solution, generator);
		solution.compute_cost();

		if(previous_cost < solution.cost)
		{
			double temperature =
			  conf.initial_temperature
			  + (static_cast<double>(iter) / static_cast<double>(conf.iterations_number))
			      * initial_to_final_range;
			size_t cost_delta = solution.cost - previous_cost;
			double probability = std::exp(-static_cast<double>(cost_delta) / temperature);

			if(dist(generator) <= probability)
			{
				// keep the new (worst) solution
			}
			else
			{
				// revert to the old (better) solution
				solution.selected_subsets.flip(flipped_bit);
			}
		}
		else if(solution.cost < best.cost)
		{
			best = solution;
		}
	}

	return best;
}
