// Behaviour test for CVertexGroupBlockOptimiser::AddBlocks (0x00AD9F10): the row's search logic on plain containers, with the
// callees modelled from their landed bodies: CanInsertMoreBlocks 0x00AD8DD0, CountRemainingGroups 0x00AD8ED0,
// CalcCurrentBestTheoreticalResultNewBlock 0x00AD8FE0 (blocks + (used + remaining - 1) / MaxN), CalcCurrentBlockSegments 0x00AD9B20 /
// PaintCurrentBlockSegment 0x00AD9380, CalcCurrentBestTheoreticalResultNewBlock2 0x00AD9D10 (blocks + per segment (n + MaxN - 3) / (MaxN - 1)),
// FindBestStartingBlock 0x00AD94F0 (the least-shared group's last block), InsertBlock 0x00AD92E0, RemoveBlock (PDB AAEXKK).
// Scenarios: (A) one output block; (B) MaxN forces two output blocks -> the new-block branch (push_back / recurse / pop_back) and the
// NewBlock2 prune; (B2) nested new blocks; (D) the NewBlock2 segment bound; (C) Greedy -> the std::sort path picks the cheapest candidate first and the search stops at the first solution.
#include <cstdio>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
static int g_newBlock = 0, g_newBlock2 = 0, g_sort = 0, g_candidates = 0, g_seed = 0;
struct Block { std::vector<unsigned long> Groups; std::set<unsigned long> Has;
    bool HasGroup(unsigned long g) const { return Has.count(g) != 0; }
    void AddGroup(unsigned long g) { Groups.push_back(g); Has.insert(g); }
    void RemoveLast(unsigned long n) { while (n--) { Has.erase(Groups.back()); Groups.pop_back(); } }
    bool HasIntersection(const Block& o) const { for (unsigned long i = 0; i < o.Groups.size(); ++i) if (HasGroup(o.Groups[i])) return true; return false; }
    void AddGroupsFrom(const Block& o) { for (unsigned long i = 0; i < o.Groups.size(); ++i) if (!HasGroup(o.Groups[i])) AddGroup(o.Groups[i]); } };
struct Info { unsigned long BlockIndex; long Cost; long Benefit; bool operator<(const Info& o) const { return Cost < o.Cost; } };
struct Opt {
    std::vector<Block> Input; std::list<Block> Optimal, Blocks; Block* Current; std::vector<bool> Remaining;
    std::vector<Block> SegGroups; std::vector<long> SegIndices; unsigned long SegCount;
    unsigned long MaxGroupIndex, MaxN, MaxIterations, Iterations; bool Greedy;
    bool HasMore() const { for (unsigned long i = 0; i < Input.size(); ++i) if (Remaining[i]) return true; return false; }
    bool CanInsertMoreBlocks() const {
        unsigned long space = MaxN - Current->Groups.size();
        for (unsigned long i = 0; i < Input.size(); ++i) if (Remaining[i]) {
            unsigned long newGroups = 0;
            for (unsigned long j = 0; j < Input[i].Groups.size(); ++j) if (!Current->HasGroup(Input[i].Groups[j])) ++newGroups;
            if (newGroups <= space) return true; }
        return false; }
    unsigned long CountRemainingGroups() const {
        std::vector<bool> used(MaxGroupIndex + 1, false);
        for (unsigned long i = 0; i < Input.size(); ++i) if (Remaining[i]) for (unsigned long j = 0; j < Input[i].Groups.size(); ++j) used[Input[i].Groups[j]] = true;
        unsigned long count = 0;
        for (unsigned long i = 0; i <= MaxGroupIndex; ++i) if (used[i] && !Current->HasGroup(i)) ++count;
        return count; }
    unsigned long CalcCurrentBestTheoreticalResultNewBlock() {
        unsigned long remaining = CountRemainingGroups(); unsigned long freeSlots = MaxN - Current->Groups.size();
        return Blocks.size() + (MaxN - freeSlots + remaining - 1) / MaxN; }
    void PaintCurrentBlockSegment(unsigned long index) {
        SegIndices[index] = (long)SegCount; SegGroups.back().AddGroupsFrom(Input[index]);
        for (unsigned long i = 0; i < SegIndices.size(); ++i)
            if (Remaining[i] && SegIndices[i] == -1 && SegGroups.back().HasIntersection(Input[i])) PaintCurrentBlockSegment(i); }
    void CalcCurrentBlockSegments() {
        SegIndices.assign(Input.size(), -1); SegCount = 0; SegGroups.clear();
        for (unsigned long i = 0; i < SegIndices.size(); ++i)
            if (Remaining[i] && SegIndices[i] == -1) { SegGroups.push_back(Block()); PaintCurrentBlockSegment(i); ++SegCount; } }
    unsigned long CalcCurrentBestTheoreticalResultNewBlock2() {
        ++g_newBlock2; CalcCurrentBlockSegments(); unsigned long result = Blocks.size();
        for (unsigned long i = 0; i < SegCount; ++i) result += (SegGroups[i].Groups.size() + MaxN - 3) / (MaxN - 1);
        return result; }
    unsigned long FindBestStartingBlock() {
        std::map<unsigned long, std::pair<unsigned long, unsigned long> > counts;   // group -> (count, last block)
        for (unsigned long i = 0; i < Input.size(); ++i) if (Remaining[i])
            for (unsigned long j = 0; j < Input[i].Groups.size(); ++j) { std::pair<unsigned long, unsigned long>& c = counts[Input[i].Groups[j]]; ++c.first; c.second = i; }
        unsigned long bestBlock = (unsigned long)-1, bestCount = (unsigned long)-1;
        for (std::map<unsigned long, std::pair<unsigned long, unsigned long> >::iterator it = counts.begin(); it != counts.end(); ++it)
            if (it->second.first < bestCount) { bestBlock = it->second.second; bestCount = it->second.first; }
        return bestBlock; }
    void InsertBlock(unsigned long index, unsigned long& added) {
        Remaining[index] = false; added = 0;
        for (unsigned long i = 0; i < Input[index].Groups.size(); ++i) { unsigned long g = Input[index].Groups[i];
            if (!Current->HasGroup(g)) { Current->AddGroup(g); ++added; } } }
    void RemoveBlock(unsigned long index, unsigned long count) { Remaining[index] = true; Current->RemoveLast(count); }
    void AddBlocks(unsigned long start) {
        ++Iterations;
        if (!Optimal.empty()) {
            if (Iterations > MaxIterations || Greedy) return;
            if (CalcCurrentBestTheoreticalResultNewBlock() >= Optimal.size()) return; }
        if (!HasMore()) { Optimal = Blocks; return; }
        unsigned long space = MaxN - Current->Groups.size();
        if (Current->Groups.size() == 0) {
            ++g_seed; unsigned long best = FindBestStartingBlock();
            unsigned long added; InsertBlock(best, added); AddBlocks(0); RemoveBlock(best, added); return; }
        std::vector<Info> cand;
        for (unsigned long j = start; j < Input.size(); ++j) if (Remaining[j]) {
            unsigned long count = Input[j].Groups.size(), newGroups = 0;
            for (unsigned long k = 0; k < count; ++k) if (!Current->HasGroup(Input[j].Groups[k])) ++newGroups;
            if (newGroups <= space) { Info in = { j, (long)newGroups, (long)count }; cand.push_back(in); } }
        if (Greedy) { ++g_sort; std::sort(cand.begin(), cand.end()); }
        if (cand.empty()) {
            if (!CanInsertMoreBlocks()) {
                if (!Optimal.empty()) {
                    if (Blocks.size() >= Optimal.size()) return;
                    if (CalcCurrentBestTheoreticalResultNewBlock2() >= Optimal.size()) return; }
                ++g_newBlock;
                Block* saved = Current; Blocks.push_back(Block()); Current = &Blocks.back();
                AddBlocks(0); Blocks.pop_back(); Current = saved; }
        } else {
            ++g_candidates;
            unsigned long n = cand.size();
            for (unsigned long c = 0; c < n; ++c) { unsigned long added; InsertBlock(cand[c].BlockIndex, added);
                unsigned long next = 0; if (!Greedy) next = cand[c].BlockIndex + 1;
                AddBlocks(next); RemoveBlock(cand[c].BlockIndex, added); } }
    }
    void Setup(unsigned long maxN, bool greedy, const unsigned long (*g)[2], int n) {
        MaxN = maxN; Greedy = greedy; MaxIterations = 100000; Iterations = 0; MaxGroupIndex = 0;
        for (int i = 0; i < n; ++i) { Block b; b.AddGroup(g[i][0]); b.AddGroup(g[i][1]); Input.push_back(b); Remaining.push_back(true);
            if (g[i][0] > MaxGroupIndex) MaxGroupIndex = g[i][0]; if (g[i][1] > MaxGroupIndex) MaxGroupIndex = g[i][1]; }
        Blocks.push_back(Block()); Current = &Blocks.back(); }
    bool Clean() const { for (unsigned long i = 0; i < Remaining.size(); ++i) if (!Remaining[i]) return false;
        return Blocks.size() == 1 && Current->Groups.empty(); }
    bool Covers() const {   // every input block fits wholly in some optimal block, and no block exceeds MaxN
        for (std::list<Block>::const_iterator it = Optimal.begin(); it != Optimal.end(); ++it) if (it->Groups.size() > MaxN) return false;
        for (unsigned long i = 0; i < Input.size(); ++i) { bool found = false;
            for (std::list<Block>::const_iterator it = Optimal.begin(); it != Optimal.end(); ++it)
                if (it->HasGroup(Input[i].Groups[0]) && it->HasGroup(Input[i].Groups[1])) found = true;
            if (!found) return false; }
        return true; }
};
static int fail(const char* what) { std::printf("00ad9f10_TEST FAIL %s\n", what); return 1; }
int main() {
    {   // A: MaxN 4, {1,2}{3,4}{1,2}{3,4} -> one block {1,2,3,4}; no new block needed
        static const unsigned long g[4][2] = { {1,2}, {3,4}, {1,2}, {3,4} };
        Opt o; o.Setup(4, false, g, 4); g_newBlock = 0; g_sort = 0; o.AddBlocks(0);
        if (!(o.Optimal.size() == 1 && o.Optimal.front().Groups.size() == 4 && o.Covers() && o.Clean() && g_newBlock == 0 && g_sort == 0)) return fail("A");
    }
    {   // B: MaxN 4, {1,2}{3,4}{5,6}{1,5} -> no single block holds 6 groups; the best is two blocks, reached through the new-block branch
        static const unsigned long g[4][2] = { {1,2}, {3,4}, {5,6}, {1,5} };
        Opt o; o.Setup(4, false, g, 4); g_newBlock = 0; g_newBlock2 = 0; g_sort = 0; o.AddBlocks(0);
        if (!(o.Optimal.size() == 2 && o.Covers() && o.Clean())) return fail("B result");
        if (!(g_newBlock > 0 && g_sort == 0)) return fail("B paths");
    }
    {   // B2: MaxN 2, {1,2}{3,4}{5,6} -> three blocks: every block is full, so each step opens a new block (nested push_back / pop_back)
        static const unsigned long g[3][2] = { {1,2}, {3,4}, {5,6} };
        Opt o; o.Setup(2, false, g, 3); g_newBlock = 0; o.AddBlocks(0);
        if (!(o.Optimal.size() == 3 && o.Covers() && o.Clean() && g_newBlock >= 2)) return fail("B2");
    }
    {   // D: MaxN 3, {0,4}{3,6}{4,2}{4,5} -> three blocks; after the first solution the search opens a new block again with fewer blocks
        //    than the best so far, so the CalcCurrentBestTheoreticalResultNewBlock2 (segment) bound is evaluated before the push_back
        static const unsigned long g[4][2] = { {0,4}, {3,6}, {4,2}, {4,5} };
        Opt o; o.Setup(3, false, g, 4); g_newBlock = 0; g_newBlock2 = 0; o.AddBlocks(0);
        if (!(o.Optimal.size() == 3 && o.Covers() && o.Clean() && g_newBlock >= 2 && g_newBlock2 > 0)) return fail("D");
    }
    {   // C: Greedy, MaxN 4, {1,2}{3,4}{2,6}{7,8}: seed {1,2}; candidates {3,4} cost 2, {2,6} cost 1, {7,8} cost 2; the sort puts {2,6} first
        //    -> first block {1,2,6}, then a new block {3,4,7,8}; greedy stops at the first solution (unsorted order would give {1,2,3,4} first)
        static const unsigned long g[4][2] = { {1,2}, {3,4}, {2,6}, {7,8} };
        Opt o; o.Setup(4, true, g, 4); g_newBlock = 0; g_sort = 0; o.AddBlocks(0);
        if (!(o.Optimal.size() == 2 && o.Covers() && o.Clean() && g_sort > 0 && g_newBlock > 0)) return fail("C result");
        const Block& first = o.Optimal.front(); const Block& second = o.Optimal.back();
        if (!(first.Groups.size() == 3 && first.HasGroup(1) && first.HasGroup(2) && first.HasGroup(6))) return fail("C first block");
        if (!(second.Groups.size() == 4 && second.HasGroup(3) && second.HasGroup(4) && second.HasGroup(7) && second.HasGroup(8))) return fail("C second block");
    }
    std::printf("00ad9f10_TEST PASS\n"); return 0;
}