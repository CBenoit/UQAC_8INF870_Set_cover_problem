//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_EXHAUSTIVE_HPP
#define SCPSOLVER_EXHAUSTIVE_HPP

#include "Problem.hpp"
#include "Solution.hpp"

namespace scp::exhaustive
{
	Solution solve(const Problem& problem);
};

#endif //SCPSOLVER_EXHAUSTIVE_HPP