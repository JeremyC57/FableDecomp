// Behaviour test: vector<CScriptedMapBrush::CScriptedMapCell>(n) -- n value-initialised (all-zero) cells, capacity exactly n
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct CScriptedMapCell { float Height; unsigned int Theme[3]; unsigned char ThemeBlend[2]; };
struct CCellVector {
    CScriptedMapCell* _M_start;
    CScriptedMapCell* _M_finish;
    CScriptedMapCell* _M_end_of_storage;
    explicit CCellVector(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n ? (CScriptedMapCell*)std::malloc(n * sizeof(CScriptedMapCell)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        CScriptedMapCell value = CScriptedMapCell();     // _Tp(): zero-initialised
        CScriptedMapCell* cur = _M_start;
        for (unsigned int k = n; k > 0; --k, ++cur)      // __uninitialized_fill_n 0x0082F240
            new (cur) CScriptedMapCell(value);
        _M_finish = cur;
    }
};

int main()
{
    void* scratch = std::malloc(16 * sizeof(CScriptedMapCell));
    std::memset(scratch, 0x5a, 16 * sizeof(CScriptedMapCell));   // dirty heap: the zeros must come from the fill
    std::free(scratch);
    CCellVector v(12);
    if (v._M_finish - v._M_start != 12 || v._M_end_of_storage != v._M_finish)
        { std::printf("0082f630_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 12; ++i)
        if (v._M_start[i].Height != 0.0f || v._M_start[i].Theme[0] != 0 || v._M_start[i].Theme[2] != 0 || v._M_start[i].ThemeBlend[1] != 0)
            { std::printf("0082f630_TEST FAIL\n"); return 1; }
    CCellVector e(0);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage != 0)
        { std::printf("0082f630_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    std::printf("0082f630_TEST PASS\n");
    return 0;
}