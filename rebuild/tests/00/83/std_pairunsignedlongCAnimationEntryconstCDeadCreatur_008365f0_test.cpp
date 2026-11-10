// Behaviour test for 0x008365F0 std::for_each<const pair<unsigned long,CAnimationEntry>*, CDeadCreatureAnimPreloader>: every
// entry whose animation name (through the def string table) is "STANDARD_DEAD" gets its bank animation's BeginLoading(false);
// the functor is returned by value.
#include <stdio.h>
#include <string.h>

static const char* GTable[4] = { "STANDARD_IDLE", "STANDARD_DEAD", "STANDARD_DEAD_2", "STANDARD_DEAD" };
struct CDefStringTable { const char* GetStringAsCharArray(long pos) const { return GTable[pos]; } };
static CDefStringTable GDefStringTable;
class CDefString { public: long TablePos; const char* GetAsCharArray() const { return GDefStringTable.GetStringAsCharArray(TablePos); } };
class CAnimationEntry { public: long BankIndex; CDefString AnimName; CDefString GroupName; long Components; };
struct SEntry { unsigned long first; CAnimationEntry second; };

static int GLoads[8]; static int GLoadArgTrue;
class C3DAnimationInfo { public: int Index; void BeginLoading(bool flag) const { ++GLoads[Index]; if (flag) ++GLoadArgTrue; } };
static C3DAnimationInfo GInfos[8] = { {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7} };
class CMeshDataBank { public: const C3DAnimationInfo* PeekAnimation(unsigned long index) const { return &GInfos[index]; } };

class CDeadCreatureAnimPreloader {
public:
    CDeadCreatureAnimPreloader(const CMeshDataBank& bank) : MeshBank(&bank) {}
    void operator()(const SEntry& entry) const
    {
        CDefString name = entry.second.AnimName;
        if (strcmp(name.GetAsCharArray(), "STANDARD_DEAD") == 0)
            MeshBank->PeekAnimation(entry.second.BankIndex)->BeginLoading(false);
    }
    const CMeshDataBank* MeshBank;
};

template <class It, class F> F ForEach(It first, It last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

int main()
{
    CMeshDataBank bank;
    SEntry entries[4];
    for (int i = 0; i < 4; ++i) {
        entries[i].first = i;
        entries[i].second.BankIndex = 4 + i;
        entries[i].second.AnimName.TablePos = i;
        entries[i].second.GroupName.TablePos = 0;
        entries[i].second.Components = 0;
    }
    CDeadCreatureAnimPreloader result = ForEach(entries, entries + 4, CDeadCreatureAnimPreloader(bank));
    int ok = result.MeshBank == &bank;
    ok = ok && GLoads[4] == 0 && GLoads[5] == 1 && GLoads[6] == 0 && GLoads[7] == 1 && GLoadArgTrue == 0;
    CDeadCreatureAnimPreloader empty = ForEach(entries, entries, CDeadCreatureAnimPreloader(bank));
    ok = ok && empty.MeshBank == &bank && GLoads[5] == 1;
    printf(ok ? "008365f0_TEST PASS\n" : "008365f0_TEST FAIL\n");
    return ok ? 0 : 1;
}