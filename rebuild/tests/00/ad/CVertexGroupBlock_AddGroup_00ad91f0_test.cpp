// Behaviour test for 0x00AD91F0 CVertexGroupBlock::AddGroup (STLport push_back + map insert of group -> index).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

static int g_overflows = 0;
struct SVector {
    unsigned long* Start; unsigned long* Finish; unsigned long* End;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    // stand-in for the out-of-line _M_insert_overflow 0x00464ACE (fill_len 1, at_end true)
    void InsertOverflow(unsigned long* pos, const unsigned long& x)
    {
        ++g_overflows;
        unsigned int old = size();
        unsigned int len = old + (old > 1u ? old : 1u);
        unsigned long* ns = (unsigned long*)std::malloc(len * sizeof(unsigned long));
        if (old) std::memmove(ns, Start, (char*)pos - (char*)Start);
        unsigned long* nf = ns + (pos - Start);
        *nf++ = x;
        if (Start) std::free(Start);
        Start = ns; Finish = nf; End = ns + len;
    }
    void push_back(const unsigned long& x)
    {
        if (Finish != End) { *Finish = x; ++Finish; }
        else InsertOverflow(Finish, x);
    }
};
class CVertexGroupBlock { public:
    SVector Groups; std::map<unsigned long, long> GroupToIndexMap;
    CVertexGroupBlock() { Groups.Start = Groups.Finish = Groups.End = 0; }
    // reconstructed body
    void AddGroup(unsigned long group)
    {
        Groups.push_back(group);
        GroupToIndexMap.insert(std::map<unsigned long, long>::value_type(group, (long)(Groups.size() - 1)));
    }
};

int main()
{
    CVertexGroupBlock b;
    b.AddGroup(7); b.AddGroup(3); b.AddGroup(11);
    if (b.Groups.size() != 3 || b.Groups.Start[0] != 7 || b.Groups.Start[1] != 3 || b.Groups.Start[2] != 11) { std::printf("00ad91f0_TEST FAIL\n"); return 1; }
    if (b.GroupToIndexMap.size() != 3 || b.GroupToIndexMap[7] != 0 || b.GroupToIndexMap[3] != 1 || b.GroupToIndexMap[11] != 2) { std::printf("00ad91f0_TEST FAIL\n"); return 1; }
    if (g_overflows != 3) { std::printf("00ad91f0_TEST FAIL\n"); return 1; }   // capacities 1, 2, 4
    b.AddGroup(9);   // fits in capacity 4: no reallocation
    if (g_overflows != 3 || b.GroupToIndexMap[9] != 3) { std::printf("00ad91f0_TEST FAIL\n"); return 1; }
    // a duplicate group is appended but keeps its first index (insert_unique does not overwrite)
    b.AddGroup(3);
    if (b.Groups.size() != 5 || b.GroupToIndexMap.size() != 4 || b.GroupToIndexMap[3] != 1) { std::printf("00ad91f0_TEST FAIL\n"); return 1; }
    std::free(b.Groups.Start);
    std::printf("00ad91f0_TEST PASS\n");
    return 0;
}