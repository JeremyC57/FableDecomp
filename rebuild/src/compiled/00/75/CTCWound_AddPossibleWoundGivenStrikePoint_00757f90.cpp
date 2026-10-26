// CTCWound::AddPossibleWoundGivenStrikePoint(const C3DVector&) -- a 5% roll adds a wound at the body location the strike height picks
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757F90, 111 bytes. __fastcall, `ret 4`, returns bool.
// Callees: CTCBase::GetWorld() 0x00686A70; CWorld random-seed accessor 0x0049D810 (returns &seed; catalog
//          "CVertexBufferWin32::DoSizeof" is a byte-twin label); GFROR13(unsigned long) 0x00497890 (__fastcall);
//          CTCWound::AddWound(EBodyLocation) 0x00757B30. Float constant 1.55f at 0x01269954.
// Shape: location 1 when the strike point's Z is above 1.55, else 6 (`fcomp ; test ah,41h ; je` over `mov ebx,6`);
//        the world's LCG seed is advanced in place (`seed = seed*0x24A1 + 0x24DF`, stored, then `seed = GFROR13(seed)`),
//        and an unsigned `% 100` compared signed (`jl`) against 95.
// Layout: CTCWound ParentThing +0x04 (CTCBase).
class C3DVector { public: float X, Y, Z; };
long __fastcall GFROR13(unsigned long value);    // 0x00497890
class CWorld { public:
    unsigned long* GetRandomSeed();              // 0x0049D810
};
__forceinline long GetRandomBelow(CWorld* world, unsigned long n)
{
    unsigned long* seed = world->GetRandomSeed();
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed % n;
}
class CTCWound { public:
    virtual void v00();
    void* ParentThing;                           // +0x04
    CWorld* GetWorld();                          // 0x00686A70 (CTCBase)
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
    if (GetRandomBelow(GetWorld(), 100) >= 95) {
        AddWound(location);
        return true;
    }
    return false;
}