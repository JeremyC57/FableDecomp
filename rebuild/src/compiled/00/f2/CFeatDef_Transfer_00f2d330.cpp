// CFeatDef::Transfer(CPersistContext& context) -- persist every CFeatDef field under its key (FeatName .. CO_ItemName), each with its default (empty def string, 0, 0.0f, false)
// Leaf: ?Transfer@CFeatDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F2D330, 403 bytes, thiscall, `ret 4` (virtual slot 18 of the def vtable).
// Identity (proven): slot 18 (Transfer) of the CFeatDef vtable 0x012FB778 (no RTTI) -- the vptr the landed CFeatDef copy
//   constructor 0x00F2D270 stores; slot 19 0x00F2D680 forwards to the landed CFeatDef::operator= 0x00F2D200 (Copy), slot
//   20 0x00F2D2F0 returns 0x64 (GetSizeofClass; retail and Ego_r.pdb sizeof(CFeatDef) are 100). Ego_r.pdb has
//   ?Transfer@CFeatDef@@UAEXAAVCPersistContext@@@Z, and the keys pushed here are exactly the Ego_r.pdb CFeatDef member names in
//   member order. The row is referenced only from that vtable slot. Catalog label: sub_00f2d330.
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) for T = CDefString 0x007E39D0, float 0x00410620, long
//   0x00410BE0, bool 0x004045C0, EFeatAttackType 0x00F2D4D0 (Ego_r.pdb ??$Transfer@W4EFeatAttackType@@@CPersistContext@@...;
//   landed under the catalog name Transfer<EKNAttackType>).
// Literals: "FeatName" 0x012FB88C, "Verb" 0x012FB884, "TimeLimit" 0x012FB878, "TargetNumber" 0x012FB868, "GoldReward"
//   0x01264CC4, "XPReward" 0x012FB85C, "ItemReward" 0x012FB850, "NoBlocking" 0x012FB844, "KN_AttackType" 0x012FB834,
//   "KN_Perfect" 0x012FB828, "KN_CreatureType" 0x012FB818, "GF_FromRegion" 0x012FB808, "GF_ToRegion" 0x012FB7FC,
//   "GF_NoTeleporting" 0x012FB7E8, "CO_ItemName" 0x012FB7DC.
// Layout: Ego_r.pdb CFeatDef (0x64, no STL members, same in retail): CDefClassBase 0x28 (retail polymorphic chain from the
//   shared header, FABLE_RETAIL_DEF_CLASS_BASE), FeatName +0x28 .. CO_ItemName +0x60. CDefString { long TablePos; } whose
//   default constructor sets TablePos -1 (the `or ebp,-1` default).
// Shape: one Transfer per member with a default temporary (all homed in one frame slot); the -1 / 0 constants live in
//   ebp / ebx.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CDefString {
public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};
enum EFeatAttackType { };
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
class CFeatDef : public CDefClassBase {                 // 0x64 bytes; vtable 0x012FB778 (no RTTI)
public:
    virtual ~CFeatDef();                                // slot 0  0x00F2D300 (vector deleting)
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F2D330
    virtual void Copy(const CDefClassBase* other);      // slot 19 0x00F2D680
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F2D2F0
    CDefString      FeatName;                           // +0x28
    CDefString      Verb;                               // +0x2c
    float           TimeLimit;                          // +0x30
    long            TargetNumber;                       // +0x34
    long            GoldReward;                         // +0x38
    long            XPReward;                           // +0x3c
    CDefString      ItemReward;                         // +0x40
    bool            NoBlocking;                         // +0x44
    EFeatAttackType KN_AttackType;                      // +0x48
    bool            KN_Perfect;                         // +0x4c
    long            KN_CreatureType;                    // +0x50
    CDefString      GF_FromRegion;                      // +0x54
    CDefString      GF_ToRegion;                        // +0x58
    bool            GF_NoTeleporting;                   // +0x5c
    CDefString      CO_ItemName;                        // +0x60
};

void CFeatDef::Transfer(CPersistContext& context)
{
    context.Transfer("FeatName", FeatName, CDefString());
    context.Transfer("Verb", Verb, CDefString());
    context.Transfer("TimeLimit", TimeLimit, 0.0f);
    context.Transfer("TargetNumber", TargetNumber, 0L);
    context.Transfer("GoldReward", GoldReward, 0L);
    context.Transfer("XPReward", XPReward, 0L);
    context.Transfer("ItemReward", ItemReward, CDefString());
    context.Transfer("NoBlocking", NoBlocking, false);
    context.Transfer("KN_AttackType", KN_AttackType, EFeatAttackType(0));
    context.Transfer("KN_Perfect", KN_Perfect, false);
    context.Transfer("KN_CreatureType", KN_CreatureType, 0L);
    context.Transfer("GF_FromRegion", GF_FromRegion, CDefString());
    context.Transfer("GF_ToRegion", GF_ToRegion, CDefString());
    context.Transfer("GF_NoTeleporting", GF_NoTeleporting, false);
    context.Transfer("CO_ItemName", CO_ItemName, CDefString());
}