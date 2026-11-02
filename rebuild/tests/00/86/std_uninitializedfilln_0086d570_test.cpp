// test: std::uninitialized_fill_n (the three-argument front end) -- copy-construct n texture-morph maps (vector copy + Dirty byte) into raw storage, return first + n; n == 0 constructs nothing
#include <stdio.h>
#include <new>
#include <vector>
static int copies = 0;
struct CTextureMorphMap {
    std::vector<unsigned long> Data; bool Dirty;
    CTextureMorphMap() : Dirty(false) { }
    CTextureMorphMap(const CTextureMorphMap& o) : Data(o.Data), Dirty(o.Dirty) { ++copies; }
};
template <class T> T* uninitialized_fill_n(T* first, unsigned int n, const T& x)
{
    T* cur = first;
    for (; n > 0; --n, ++cur) new (cur) T(x);
    return cur;
}
int main() {
    CTextureMorphMap src; src.Data.push_back(7); src.Data.push_back(9); src.Dirty = true;
    void* raw = operator new(sizeof(CTextureMorphMap) * 4);
    CTextureMorphMap* buf = (CTextureMorphMap*)raw;
    CTextureMorphMap* end = uninitialized_fill_n(buf, 3, src);
    if (end != buf + 3 || copies != 3) { printf("0086d570_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (buf[i].Data.size() != 2 || buf[i].Data[1] != 9 || !buf[i].Dirty) { printf("0086d570_TEST FAIL\n"); return 1; }
    if (uninitialized_fill_n(buf, 0, src) != buf || copies != 3) { printf("0086d570_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) buf[i].~CTextureMorphMap();
    operator delete(raw);
    printf("0086d570_TEST PASS\n");
    return 0;
}