// Link test for 0x00A84FF0: the source object's CVectorMap_T_00a84ff0::Sort hands [First, Last) and the map's comparator
// to the sort (0x00A84CF0, supplied here) and clears Dirty.
#include <cstdio>
struct SElement12_00a84cf0 { unsigned char _body[12]; };
struct CCompare_00a84ff0 { };
static int g_sorts = 0;
static const void* g_first = 0;
static const void* g_last = 0;
void __fastcall Sort_00a84cf0(SElement12_00a84cf0* first, SElement12_00a84cf0* last, CCompare_00a84ff0) { ++g_sorts; g_first = first; g_last = last; }
struct CVectorMap_T_00a84ff0 {
    SElement12_00a84cf0* First;
    SElement12_00a84cf0* Last;
    SElement12_00a84cf0* EndOfStorage;
    CCompare_00a84ff0    Compare;
    bool                 Dirty;
    void Sort();                                   // from the source object
};
int main()
{
    SElement12_00a84cf0 e[3];
    CVectorMap_T_00a84ff0 m;
    m.First = e; m.Last = e + 3; m.EndOfStorage = e + 3; m.Dirty = true;
    m.Sort();
    int ok = sizeof(SElement12_00a84cf0) == 12 && g_sorts == 1 && g_first == e && g_last == e + 3 && !m.Dirty;
    if (!ok) { std::printf("00a84ff0_TEST FAIL\n"); return 1; }
    std::printf("00a84ff0_TEST PASS\n");
    return 0;
}
