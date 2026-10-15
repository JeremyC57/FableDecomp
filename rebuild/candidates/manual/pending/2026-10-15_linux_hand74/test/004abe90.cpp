// Behaviour test for CScriptThing::CScriptThing(const CScriptThing&) (0x004ABE90): copies both PImp words, bumps the count when present.
#include <cstdio>

struct CCPPointerInfo { long m_references; };
struct CScriptThing;
struct CCountedPointer_CScriptThing {
    CScriptThing* Data;
    CCPPointerInfo* Info;
    CCountedPointer_CScriptThing(CScriptThing* d, CCPPointerInfo* i) : Data(d), Info(i) {}
    CCountedPointer_CScriptThing(const CCountedPointer_CScriptThing& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};
struct CScriptThing {
    CCountedPointer_CScriptThing PImp;
    CScriptThing(CScriptThing* d, CCPPointerInfo* i) : PImp(d, i) {}
    CScriptThing(const CScriptThing& other) : PImp(other.PImp) {}
    virtual ~CScriptThing() {}
};

int main()
{
    CCPPointerInfo info = { 3 };
    CScriptThing impl(0, 0);
    CScriptThing a(&impl, &info);
    CScriptThing b(a);
    if (b.PImp.Data != &impl || b.PImp.Info != &info || info.m_references != 4) { std::printf("004abe90_TEST FAIL\n"); return 1; }
    CScriptThing n(0, 0);
    CScriptThing m(n);
    if (m.PImp.Data != 0 || m.PImp.Info != 0) { std::printf("004abe90_TEST FAIL\n"); return 1; }
    std::printf("004abe90_TEST PASS\n");
    return 0;
}
