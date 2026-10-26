#include <cstdio>

// Behaviour model of __copy<CIntelligentPointer<CThing>*> (0x004AA610): n = last - first assignments through
// operator= (SetPItem(other.GetPItem())), result advanced and returned.
class CThing { public: int id; };
static int g_sets = 0;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CBaseIntelligentPointer {
    CThing* PItem;
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* item) { PItem = item; ++g_sets; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
typedef CIntelligentPointer<CThing> TPointer;
TPointer* __copy(TPointer* first, TPointer* last, TPointer* result, const random_access_iterator_tag&, int*)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    CThing a = { 1 }, b = { 2 }, c = { 3 };
    TPointer src[3], dst[5];
    src[0].PItem = &a; src[1].PItem = &b; src[2].PItem = &c;
    for (int i = 0; i < 5; ++i) dst[i].PItem = 0;
    TPointer* r = __copy(src, src + 3, dst + 1, random_access_iterator_tag(), 0);
    if (r != dst + 4 || g_sets != 3) { std::printf("004aa610_TEST FAIL\n"); return 1; }
    if (dst[0].PItem != 0 || dst[1].PItem != &a || dst[2].PItem != &b || dst[3].PItem != &c || dst[4].PItem != 0) { std::printf("004aa610_TEST FAIL\n"); return 1; }
    if (__copy(src, src, dst, random_access_iterator_tag(), 0) != dst || g_sets != 3) { std::printf("004aa610_TEST FAIL\n"); return 1; }
    std::printf("004aa610_TEST PASS\n");
    return 0;
}