// behaviour test for 0x00659C7E CTCVillage::AddCrateTasksFromAndTo(const vector<long>& from, const vector<long>& to):
// stacks whose type is a destination get (2 - crates) put-down tasks, then stacks whose type is a source get one
// pick-up task per crate they hold.
#include <cstdio>
#include <vector>
struct Stack { long type; long crates; long crateType; };
struct Task { int stack; bool pickUp; long crateType; };
static bool Contains(const std::vector<long>& v, long value)
{
    for (size_t i = 0; i < v.size(); ++i) if (v[i] == value) return true;
    return false;
}
static std::vector<Task> AddCrateTasks(const std::vector<Stack>& stacks, const std::vector<long>& from, const std::vector<long>& to)
{
    std::vector<Task> tasks;
    for (int s = 0; s < (int)stacks.size(); ++s)
        if (Contains(to, stacks[s].type))
            for (long i = 0; i < 2 - stacks[s].crates; ++i) { Task t = { s, false, stacks[s].crateType }; tasks.push_back(t); }
    for (int s = 0; s < (int)stacks.size(); ++s)
        if (Contains(from, stacks[s].type))
            for (long i = 0; i < stacks[s].crates; ++i) { Task t = { s, true, stacks[s].crateType }; tasks.push_back(t); }
    return tasks;
}
int main()
{
    std::vector<Stack> stacks;
    Stack a = { 1, 0, 7 }, b = { 2, 2, 8 }, c = { 3, 1, 9 };
    stacks.push_back(a); stacks.push_back(b); stacks.push_back(c);
    std::vector<long> from, to;
    from.push_back(2); from.push_back(3); to.push_back(1); to.push_back(3);
    std::vector<Task> t = AddCrateTasks(stacks, from, to);
    // to: stack0 (type 1, 0 crates) -> 2 put-downs, stack2 (type 3, 1 crate) -> 1; from: stack1 -> 2 pick-ups, stack2 -> 1
    bool ok = t.size() == 6 && !t[0].pickUp && t[0].stack == 0 && t[1].stack == 0 && t[2].stack == 2 && !t[2].pickUp
        && t[3].pickUp && t[3].stack == 1 && t[3].crateType == 8 && t[4].stack == 1 && t[5].stack == 2 && t[5].pickUp;
    ok = ok && AddCrateTasks(stacks, std::vector<long>(), std::vector<long>()).empty();
    if (!ok) { std::printf("00659c7e_TEST FAIL\n"); return 1; }
    std::printf("00659c7e_TEST PASS\n");
    return 0;
}
