// Behaviour test for std::uninitialized_fill_n over CCountedPointer slots (00a95890): n copies of x, each bumping the shared refcount, returns the end
#include <cstdio>
#include <new>
class CCPPointerInfo { public: unsigned long RefCount; void* DeleteFunc; void* Data; };
struct CCountedPointer {
    void* Data; CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer(void* d, CCPPointerInfo* i) : Data(d), Info(i) {}
};
inline void* PlacementNew(void* p) { return p; }
static CCountedPointer* uninitialized_fill_n(CCountedPointer* first, unsigned int n, const CCountedPointer& x)
{
    CCountedPointer* cur = first;
    for (; n > 0; --n, ++cur) {
        void* slot = PlacementNew(cur);
        if (slot) new (slot) CCountedPointer(x);
    }
    return cur;
}
int main()
{
    int payload = 5;
    CCPPointerInfo info = { 1, 0, &payload };
    CCountedPointer x(&payload, &info);
    unsigned char raw[4 * sizeof(CCountedPointer)];
    CCountedPointer* slots = (CCountedPointer*)raw;
    CCountedPointer* end = uninitialized_fill_n(slots, 4, x);
    if (end != slots + 4 || info.RefCount != 5) { std::printf("00a95890_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (slots[i].Data != &payload || slots[i].Info != &info) { std::printf("00a95890_TEST FAIL\n"); return 1; }
    if (uninitialized_fill_n(slots, 0, x) != slots || info.RefCount != 5) { std::printf("00a95890_TEST FAIL\n"); return 1; }
    CCountedPointer nul((void*)0, (CCPPointerInfo*)0);
    if (uninitialized_fill_n(slots, 2, nul) != slots + 2 || slots[1].Info != 0) { std::printf("00a95890_TEST FAIL\n"); return 1; }
    std::printf("00a95890_TEST PASS\n");
    return 0;
}