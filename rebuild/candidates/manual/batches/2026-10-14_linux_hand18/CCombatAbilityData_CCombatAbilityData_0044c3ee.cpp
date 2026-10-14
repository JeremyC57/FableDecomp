// CCombatAbilityData::CCombatAbilityData(ECombatAbilityInterfaceType) -- frameless: vtable, 100% chance, -1s duration, 0s repeat wait, empty anim def-string, the interface type
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C3EE, length 41 bytes.
// Callees: none. Vtable 0x01232484 = CCombatAbilityData (rtti-vtables.tsv).
// Layout = engine/CCombatAbilityData.h (PercentageChance +4, SecondsDuration +8,
// SecondsToWaitBeforeRepeat +0xc, Anim (CDefString, TablePos -1) +0x10, InterfaceType +0x14); the
// header is a flat POD, so the `_Retail` model carries the constructor. The zero float goes through
// `fldz ; fstp` while -1.0f is an immediate store, and the def string's -1 is `or dword ptr,-1`:
// both are the size optimiser's choices (`#pragma optimize("s", on)`).
#pragma optimize("s", on)
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CCombatAbilityData_Retail {
    virtual void v00();
    long       PercentageChance;                            // +0x04
    float      SecondsDuration;                             // +0x08
    float      SecondsToWaitBeforeRepeat;                   // +0x0c
    CDefString Anim;                                        // +0x10
    long       InterfaceType;                               // +0x14 ECombatAbilityInterfaceType
    CCombatAbilityData_Retail(long interfaceType);
};

CCombatAbilityData_Retail::CCombatAbilityData_Retail(long interfaceType)
    : PercentageChance(100)
    , SecondsDuration(-1.0f)
    , SecondsToWaitBeforeRepeat(0.0f)
    , Anim()
    , InterfaceType(interfaceType)
{
}
