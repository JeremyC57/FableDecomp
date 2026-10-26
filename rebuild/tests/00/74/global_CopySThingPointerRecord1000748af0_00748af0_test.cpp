#include <cstdio>

class CBaseObject { public: int m_id; };
static int g_gets = 0, g_sets = 0;

struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { ++g_gets; return PData; }
    void SetPItem(CBaseObject* item) { ++g_sets; PData = item; }
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CThing;
struct SWordPair08 { long m_first; long m_second; };
struct SThingPointerRecord10 {
    CIntelligentPointer<CThing> PThing;
    SWordPair08                 m_values;
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static SThingPointerRecord10* Copy(const SThingPointerRecord10* first, const SThingPointerRecord10* last, SThingPointerRecord10* result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    CBaseObject objs[3];
    SThingPointerRecord10 src[3], dst[4];
    for (int i = 0; i < 3; ++i) {
        src[i].PThing.PData = &objs[i];
        src[i].m_values.m_first = 10 + i;
        src[i].m_values.m_second = 20 + i;
    }
    for (int i = 0; i < 4; ++i) { dst[i].m_values.m_first = -1; dst[i].m_values.m_second = -1; }
    random_access_iterator_tag tag;
    SThingPointerRecord10* r = Copy(src, src + 3, dst, tag, 0);
    bool ok = r == dst + 3 && g_gets == 3 && g_sets == 3;
    for (int i = 0; i < 3; ++i)
        if (dst[i].PThing.PData != &objs[i] || dst[i].m_values.m_first != 10 + i || dst[i].m_values.m_second != 20 + i) ok = false;
    if (dst[3].PThing.PData != 0 || dst[3].m_values.m_first != -1) ok = false;
    if (Copy(src, src, dst, tag, 0) != dst || g_sets != 3) ok = false;
    if (!ok) { std::printf("00748af0_TEST FAIL\n"); return 1; }
    std::printf("00748af0_TEST PASS\n");
    return 0;
}