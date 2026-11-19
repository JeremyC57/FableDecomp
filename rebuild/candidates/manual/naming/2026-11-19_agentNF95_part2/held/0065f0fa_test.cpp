// Test for CIsHornTaskWithThisHorn::CIsHornTaskWithThisHorn(CIntelligentPointer<CThing>) @ 0x0065F0FA (NF95 2026-11-19, q11276).
// Portable model of the constructor's contract: PHorn is copy-constructed from the by-value argument (the out-of-line
// CIntelligentPointer<CThing> copy 0x004A9730: base ctor, then SetPItem(other.GetPItem())), and the argument is destroyed
// by the callee (~CBaseIntelligentPointer 0x00A01C10) after the member copy.
#include <cstdio>
static int gLog[8]; static int gN = 0;
class CThing { public: int id; };
struct CBaseIntelligentPointer {
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    ~CBaseIntelligentPointer() { gLog[gN++] = 2; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { PItem = item; }
    CIntelligentPointer(const CIntelligentPointer& other) { PItem = other.PItem; gLog[gN++] = 1; }
    T* Get() const { return PItem; }
};
class CIsHornTaskWithThisHorn {
public:
    CIsHornTaskWithThisHorn(CIntelligentPointer<CThing> p0) : PHorn(p0) { }
    CIntelligentPointer<CThing> PHorn;
};
int main()
{
    CThing horn; horn.id = 7;
    int fails = 0;
    {
        CIntelligentPointer<CThing> src(&horn);
        gN = 0;
        CIsHornTaskWithThisHorn pred(src);       // copy into the by-value argument, copy into PHorn, destroy the argument
        if (pred.PHorn.Get() != &horn) fails |= 1;
        if (src.Get() != &horn) fails |= 2;
        if (gN < 3 || gLog[gN - 2] != 1 || gLog[gN - 1] != 2) fails |= 4;   // the member copy precedes the argument's destruction
    }
    if (fails) { std::printf("0065f0fa_TEST FAIL %d\n", fails); return 1; }
    std::printf("0065f0fa_TEST PASS\n");
    return 0;
}
