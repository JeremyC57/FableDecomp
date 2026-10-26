// Behaviour test for 0x008AC180 UninitFillN_CScriptThing_008ac180: fill n CScriptThing handles (shared implementation +1 reference each).
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
class CCPPointerInfo { public: long m_references; CCPPointerInfo() : m_references(1) { } };
struct CBase {
    CBase() { }
    virtual ~CBase() { }
};
class CScriptThing;
struct CCountedPointer_CScriptThing {
    CScriptThing*   Data;
    CCPPointerInfo* Info;
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
    CCountedPointer_CScriptThing(const CCountedPointer_CScriptThing& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_references; }
};
class CScriptThing : public CBase { public:
    CCountedPointer_CScriptThing PImp;
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBase(), PImp(other.PImp) { }
    virtual ~CScriptThing() { }
    virtual int Kind() const { return 11; }
};

static CScriptThing* UninitFillN(CScriptThing* first, unsigned int n, const CScriptThing& x, const __false_type&)
{
    CScriptThing* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CScriptThing(x);
    return cur;
}

int main()
{
    CCPPointerInfo info;
    CScriptThing implementation;
    CScriptThing x;
    x.PImp.Data = &implementation; x.PImp.Info = &info;
    static double storage[3 * sizeof(CScriptThing) / sizeof(double) + 1];
    CScriptThing* dst = (CScriptThing*)storage;
    __false_type tag;
    CScriptThing* r = UninitFillN(dst, 3, x, tag);
    bool ok = r == dst + 3 && info.m_references == 4;
    for (int k = 0; k < 3; ++k)
        if (dst[k].PImp.Data != &implementation || dst[k].PImp.Info != &info || dst[k].Kind() != 11) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || info.m_references != 4) ok = false;
    if (!ok) { std::printf("008ac180_TEST FAIL\n"); return 1; }
    std::printf("008ac180_TEST PASS\n");
    return 0;
}