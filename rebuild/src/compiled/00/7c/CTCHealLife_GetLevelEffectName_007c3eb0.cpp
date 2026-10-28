// CTCHealLife::GetLevelEffectName() -- the healing particle effect name for the spell level: "HEALING_EFFECT_LEVEL1" by default, LEVEL2/3/4 for spell levels 1/2/3
// Leaf: ?GetLevelEffectName@CTCHealLife
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C3EB0, 92 bytes (+4 int3), thiscall `ret 4` (CCharString through the hidden
//   result pointer), no EH frame.
// Identity: Ego_r.pdb name set ?GetLevelEffectName@CTCHealLife@@AAE?AVCCharString@@XZ (private, non-const); both
//   callers are CTCHealLife (FrameUpdate 0x007C3FB0 at 0x007C3FFC, StartHealingEffect 0x007C4430 at 0x007C4550). The
//   catalog label "CThing_GetHealingEffectName" is descriptive.
// Callees: CCharString(const char*, long) @ 0x0099EBF0, CCharString::operator=(const char*) @ 0x0099EFE0,
//   CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0.
// Layout: CTCHealLife (Ego_r.pdb) SpellLevel +0x18.
// Shape: a named CCharString built from the level-1 literal, a three-case switch (`dec eax` chain: 1 -> LEVEL2 @
//   0x0126E304, 2 -> LEVEL3 @ 0x0126E31C, 3 -> LEVEL4 @ 0x0126E334, checked against the retail pushes) assigning the
//   literal, then the copy into the result and the local's destructor (no NRVO).
class CCharString {
public:
    void* PString;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};

class CTCHealLife {
public:
    virtual void v00();
    void*         ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad08[0x18 - 0x08];
    long          SpellLevel;                   // +0x18
    CCharString GetLevelEffectName();
};

CCharString CTCHealLife::GetLevelEffectName()
{
    CCharString name("HEALING_EFFECT_LEVEL1");
    switch (SpellLevel) {
    case 1:
        name = "HEALING_EFFECT_LEVEL2";
        break;
    case 2:
        name = "HEALING_EFFECT_LEVEL3";
        break;
    case 3:
        name = "HEALING_EFFECT_LEVEL4";
        break;
    }
    return name;
}