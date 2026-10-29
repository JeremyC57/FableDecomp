// CArenaCreatureDef::Transfer(CPersistContext& context) -- persist the arena creature def: CreatureType, NumCreatures (default 0), HUDType, DeathScore (default 0), each under its member name
// Leaf: ?Transfer@CArenaCreatureDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F25E70, 143 bytes, thiscall, `ret 4` (virtual slot 18 of the def vtable).
// Identity (proven): the row is referenced once, from slot 18 (Transfer) of the RTTI-less def vtable 0x012FB358 (vt[-1] == 0),
//   which the def's constructors at 0x00F25DE7 / 0x00F263DD store; slot 0 is the scalar deleting dtor 0x00F25E20 (its dtor
//   0x00F25E40 destroys +0x30 and +0x28 and stores the CDefPointeeBase vtable 0x01230BA0), slot 19 Copy 0x00F25F00 (jmp to
//   the landed CArenaCreatureDef Copy 0x00F25F10), slot 20 GetSizeofClass 0x00F25E10 (`mov eax,38h`). The four key strings
//   "CreatureType" / "NumCreatures" / "HUDType" / "DeathScore" are exactly the Ego_r.pdb CArenaCreatureDef members
//   (CreatureType +0x28, NumCreatures +0x2c, HUDType +0x30, DeathScore +0x34, sizeof 56 = 0x38 = GetSizeofClass), and Ego_r.pdb
//   has ?Transfer@CArenaCreatureDef@@UAEXAAVCPersistContext@@@Z. Catalog label sub_00f25e70 (gap scan).
// Callees: CPersistContext::GetDefaultVal<CCharString> 0x0040E1E0 (the two-argument Transfer inline), Transfer<CCharString>
//   0x004109A0, ~CCharString 0x0099EAE0, Transfer<long> 0x00410BE0 (Ego_r ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z).
// Strings: "CreatureType" 0x012FB3E0, "NumCreatures" 0x012FB3D0, "HUDType" 0x012FB3C8, "DeathScore" 0x012FB3BC.
// Shape: the landed CV_FishingCompetitionScript::OnPersist 0x00E7E210 model: one Transfer per member in declaration order; the
//   CCharString default from GetDefaultVal into a frame temporary, the long default a temporary in the dead `context` slot.
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, 0x28 bytes).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CPersistContext {
protected:
    template <class T> static T __fastcall GetDefaultVal();   // the CCharString instance is 0x0040E1E0
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CCharString 0x004109A0, long 0x00410BE0
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
};

class CArenaCreatureDef : public CDefClassBase {       // vtable 0x012FB358 (no RTTI), 0x38 bytes
public:
    virtual ~CArenaCreatureDef();                       // slot 0  0x00F25E20 (scalar deleting)
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F25E70
    virtual void Copy(const CDefClassBase* other);      // slot 19 0x00F25F00
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F25E10
    CCharString CreatureType;                           // +0x28
    long        NumCreatures;                           // +0x2c
    CCharString HUDType;                                // +0x30
    long        DeathScore;                             // +0x34
};

void CArenaCreatureDef::Transfer(CPersistContext& context)
{
    context.Transfer("CreatureType", CreatureType);
    context.Transfer("NumCreatures", NumCreatures, 0L);
    context.Transfer("HUDType", HUDType);
    context.Transfer("DeathScore", DeathScore, 0L);
}