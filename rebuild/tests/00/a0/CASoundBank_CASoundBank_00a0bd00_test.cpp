// behaviour test for 0x00A0BD00 CASoundBank::CASoundBank() (model copy of the source; the base ctor/dtor are local stubs)
#include <cstdio>
#include <cstring>
static int g_base = 0;
#include <new>
class CBaseClass {                                       // Ego_r.pdb / RTTI: the polymorphic engine root
public:
    CBaseClass() { ++g_base; }
    virtual ~CBaseClass() { }
};
class CBaseClassNonCopyable : public CBaseClass {        // RTTI base of the class below
protected:
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { }
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};
class CCRCSymbolMap;

class CASoundBank : public CBaseClassNonCopyable {
public:
    CASoundBank();
    virtual ~CASoundBank();
    CCountedPointer<CCRCSymbolMap> PSymbolMap;
};
CASoundBank::CASoundBank() { }
CASoundBank::~CASoundBank() { }
int main()
{
    unsigned char raw[sizeof(CASoundBank)];
    std::memset(raw, 0xCD, sizeof(raw));
    CASoundBank* p = new (raw) CASoundBank;
    int ok = g_base == 1 && p->PSymbolMap.Data == 0 && p->PSymbolMap.Info == 0 && (void*)p == (void*)raw;
    if (!ok) { std::printf("00a0bd00_TEST FAIL\n"); return 1; }
    std::printf("00a0bd00_TEST PASS\n");
    return 0;
}