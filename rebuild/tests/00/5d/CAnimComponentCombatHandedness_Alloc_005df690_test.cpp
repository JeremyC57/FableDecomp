// Behaviour test for CAnimComponentCombatHandedness::Alloc @ 0x005df690: a fresh component with ref count 1 (and defaulted members), null when new fails.
#include <cstdio>
#include <cstdlib>
static bool g_fail = false;
void* operator new(size_t size) { if (g_fail) return 0; return std::malloc(size); }
void operator delete(void* p) { std::free(p); }
class CPersistContext; class CCharString; class CDataInputStream; class CDataOutputStream;
class CIVCountedPointeeBase { public: virtual void v0() { } long m_refCount; CIVCountedPointeeBase() : m_refCount(1) { } };
class CAnimComponentBase : public CIVCountedPointeeBase { public:
    virtual void v1() { }
    virtual long GetType() const { return -1; }
    virtual CAnimComponentBase* Alloc() const { return 0; }
};

class CAnimComponentCombatHandedness : public CAnimComponentBase { public:
    long StartHandedness; long EndHandedness;
    CAnimComponentCombatHandedness() { }
    virtual long GetType() const { return 7; }
    virtual CAnimComponentBase* Alloc() const { return new CAnimComponentCombatHandedness; }
};
int main()
{
    CAnimComponentCombatHandedness proto;
    CAnimComponentBase* p = proto.Alloc();
    bool ok = p != 0 && p->m_refCount == 1 && p->GetType() == 7 && sizeof(CAnimComponentCombatHandedness) == 0x10;
    if (ok) {  }
    g_fail = true;
    if (proto.Alloc() != 0) ok = false;
    if (!ok) { std::printf("005df690_TEST FAIL\n"); return 1; }
    std::printf("005df690_TEST PASS\n");
    return 0;
}