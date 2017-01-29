/*
 * Copyright (c) 2016-2017, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 * \brief Rose build: code for constructing literal tables.
 */

#ifndef ROSE_BUILD_MATCHERS_H
#define ROSE_BUILD_MATCHERS_H

#include "rose_build_impl.h"
#include "rose_build_lit_accel.h"
#include "hwlm/hwlm_literal.h"

#include <map>
#include <vector>

struct HWLM;

namespace ue2 {

/** \brief Prototype for literal matcher construction. */
struct MatcherProto {
    /** \brief Literal fragments used to construct the literal matcher. */
    std::vector<hwlmLiteral> lits;

    /** \brief Longer literals used for acceleration analysis. */
    std::vector<AccelString> accel_lits;

    /** \brief The history required by the literal matcher. */
    size_t history_required = 0;

    /** \brief Insert the contents of another MatcherProto. */
    void insert(const MatcherProto &a);
};

/**
 * \brief Build up a vector of literals (and associated other data) for the
 * given table.
 *
 * If max_offset is specified (and not ROSE_BOUND_INF), then literals that can
 * only lead to a pattern match after max_offset may be excluded.
 */
MatcherProto
makeMatcherProto(const RoseBuildImpl &build,
                 const std::map<u32, LitFragment> &final_to_frag_map,
                 rose_literal_table table, bool delay_rebuild, size_t max_len,
                 u32 max_offset = ROSE_BOUND_INF);

aligned_unique_ptr<HWLM>
buildFloatingMatcher(const RoseBuildImpl &build, size_t longLitLengthThreshold,
                     const std::map<u32, LitFragment> &final_to_frag_map,
                     rose_group *fgroups, size_t *fsize,
                     size_t *historyRequired);

aligned_unique_ptr<HWLM> buildDelayRebuildMatcher(
    const RoseBuildImpl &build, size_t longLitLengthThreshold,
    const std::map<u32, LitFragment> &final_to_frag_map, size_t *drsize);

aligned_unique_ptr<HWLM>
buildSmallBlockMatcher(const RoseBuildImpl &build,
                       const std::map<u32, LitFragment> &final_to_frag_map,
                       size_t *sbsize);

aligned_unique_ptr<HWLM>
buildEodAnchoredMatcher(const RoseBuildImpl &build,
                        const std::map<u32, LitFragment> &final_to_frag_map,
                        size_t *esize);

void findMoreLiteralMasks(RoseBuildImpl &build);

} // namespace ue2

#endif // ROSE_BUILD_MATCHERS_H
