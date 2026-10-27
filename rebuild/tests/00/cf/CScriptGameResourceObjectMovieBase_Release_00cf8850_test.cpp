#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
static int g_deleted;
template <class T> class CCountedPointer {
public:
    T* Data; int* Count;
    void Reset(T* data) { if (Count && --*Count == 0) { ++g_deleted; } Count = 0; Data = data; }
};
class CScriptGameResourceObjectMovieBase {
public:
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;
    void Release() { PImp.Reset(0); }
};
int main()
{
    int ok = 1;
    int count = 1; CScriptGameResourceObjectMovieBase impl, h; h.PImp.Data = &impl; h.PImp.Count = &count;
    h.Release();
    ok = h.PImp.Data == 0 && h.PImp.Count == 0 && count == 0 && g_deleted == 1;
    if (!ok) { std::printf("00cf8850_TEST FAIL\n"); return 1; }
    std::printf("00cf8850_TEST PASS\n");
    return 0;
}