// CTCWound::AddPossibleWoundGivenStrikePoint(const C3DVector&) -- a 5% roll adds a wound at the body location the strike height picks
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757F90, 111 bytes. __fastcall, `ret 4`, returns bool.
// Callees: CTCBase::GetWorld() 0x00686A70; CWorld random-seed accessor 0x0049D810 (returns &seed; catalog
//          "CVertexBufferWin32::DoSizeof" is a byte-twin label); GFROR13(unsigned long) 0x00497890 (__fastcall);
//          CTCWound::AddWound(EBodyLocation) 0x00757B30. Float constant 1.55f at 0x01269954.
// Shape: location 1 when the strike point's Z is above 1.55, else 6 (`fcomp ; test ah,41h ; je` over `mov ebx,6`);
//        the world's LCG seed is advanced in place (`seed = seed*0x24A1 + 0x24DF`, stored, then `seed = GFROR13(seed)`),
//        and an unsigned `% 100` compared signed (`jl`) against 95.
// Layout: CTCWound ParentThing +0x04 (CTCBase).
#include "fable_compiler.h"
#include "engine/C3DVector.h"
long __fastcall GFROR13(unsigned long value);    // 0x00497890
class CWorld { public:
    unsigned long* GetWorldSeed() const;              // 0x0049D810
};
FABLE_FORCEINLINE long GetRandomBelow(CWorld* world, unsigned long n)
{
    unsigned long* seed = world->GetWorldSeed();
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed % n;
}
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCWound : public CTCBase { public:
    void* ParentThing;                           // +0x04
    void AddWound(long location);                // 0x00757B30
    bool AddPossibleWoundGivenStrikePoint(const C3DVector& point);
};
bool CTCWound::AddPossibleWoundGivenStrikePoint(const C3DVector& point)
{
    long location;
    if (point.Z > 1.55f)
        location = 1;
    else
        location = 6;
    if (GetRandomBelow((&GetWorld()), 100) >= 95) {
        AddWound(location);
        return true;
    }
    return false;
}