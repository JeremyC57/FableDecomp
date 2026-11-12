// CTCProjectileTrap::UpdateTrap() -- per-frame trap logic: target the hero when the current target is gone, turn the trap's look component toward the target, then count the idle/shot timers down and fire (or go idle after a full round)
// Leaf: UpdateTrap
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CComponentBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00787E12, 279 bytes (plain `ret`). CTCProjectileTrap vtable 0x0126B90C slot 29. The trap
//   TU is size-optimised with a frame pointer: `optimize("y", off)` + `("s", on)` (`push ebp ; mov ebp,esp`, `and [esi+68h],0`,
//   `leave`), as the landed TriggerTrap (0x00787CA5) and CTCMeleeTrap::UpdateAnimation (0x00788555).
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`, twice), CTCBase::GetPlayerManager @ 0x00686D40,
//   CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, IS_THING_ALIVE<CThing*> @ 0x0040DCE0
//   (`__fastcall`, by const reference), CIntelligentPointer(CThing*) @ 0x004A96F0, CIntelligentPointer::operator= @ 0x004A9760,
//   ~CBaseIntelligentPointer @ 0x00A01C10, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020 (interface
//   0x43 = CTCLook), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPos @ 0x004C73D0, CTCLook::LookAtPos(const
//   C3DVector&, long) @ 0x00753E90, __ftol2 @ 0x00BFEA70, virtual TriggerTrap (slot 30, +0x78). CMainGameComponent::GetConstantFPS() @ 0x01375550.
// Layout: Ego_r.pdb CTCTrapBase with 12-byte retail vectors (PTrapDef +0x54, Active +0x58, Triggered +0x59), CTCProjectileTrap
//   ShotTimer +0x60, IdleTimer +0x64, NumShotsThisRound +0x68, PTarget +0x6C (PDB -0xC); CTrapDef NumShotsToFireBeforeIdle +0x58,
//   SecondsIdle +0x5C (def bodies sit 4 bytes low).
// Shape: the hero is a named local passed by const reference (frame word [ebp-4]); the assignment goes through the implicit
//   conversion temporary; the look component is HasTC + the end/hit ternary lookup.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
class C3DVector;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char    _pad00[0x20];
    std::bitset<274> UsingComponents;                   // +0x20
    CInterfaceMap    m_interfaces;                      // +0x44
    const C3DVector& GetPos() const;                     // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };
class CTCLook { public:
    void LookAtPos(const C3DVector& position, long priority);   // 0x00753E90
};
void LookAtSink_s7(CTCLook* look, float x, long priority);   // stand-in: the look update (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for CTCLook::LookAtPos (0x00753E90): reads the position through its const reference, lets nothing escape.
__declspec(noinline) void CTCLook::LookAtPos(const C3DVector& position, long priority)
{
    LookAtSink_s7(this, *(const float*)&position, priority);
}
#pragma auto_inline(on)
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                             // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CBaseObject;
struct CBaseIntelligentPointer {                        // vptr +0x00
    void* m_pInfo;                                      // +0x04
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10 Ego_r/FableWin ??1CBaseIntelligentPointer@@UAE@XZ (virtual)
};
template <class T> class CIntelligentPointer;
template <> class CIntelligentPointer<CThing> : public CBaseIntelligentPointer { public:
    CIntelligentPointer(CThing* thing);          // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>* pointer);   // 0x004CC340
bool PointerAliveSink_s7(CThing* item);   // stand-in: the dead-flag test
#pragma auto_inline(off)
// Same-TU body of IS_THING_ALIVE<CIntelligentPointer<CThing> > (0x004CC340, __fastcall): reads the pointer only.
__declspec(noinline) bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>* pointer)
{
    CThing* item = ((CThing*)pointer->GetPItem());
    if (item == 0)
        return false;
    return PointerAliveSink_s7(item);
}
#pragma auto_inline(on)
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x0040DCE0
#pragma auto_inline(off)
// Same-TU body of IS_THING_ALIVE<CThing*> (0x0040DCE0, __fastcall): reads the pointer through its const reference only.
__declspec(noinline) bool __fastcall IS_THING_ALIVE(CThing* const& thing)
{
    if (thing == 0)
        return false;
    return PointerAliveSink_s7(thing);
}
#pragma auto_inline(on)
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CTrapDef { public:
    unsigned char _pad00[0x58];
    long  NumShotsToFireBeforeIdle;                     // +0x58 (PDB +0x5c)
    float SecondsIdle;                                  // +0x5c (PDB +0x60)
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCTrapBase : public CTCBase { public:
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x54 - 0x08];
    const CTrapDef* PTrapDef;                           // +0x54
    bool          Active;                               // +0x58
    bool          Triggered;                            // +0x59
    bool          LastTriggerOnProximity;               // +0x5a
    long          TrapType;                             // +0x5c ETrapType
};
struct CTCProjectileTrap : CTCTrapBase {
    long ShotTimer;                                     // +0x60
    long IdleTimer;                                     // +0x64
    long NumShotsThisRound;                             // +0x68
    CIntelligentPointer<CThing> PTarget;                 // +0x6c
    void UpdateTrap();
    void TriggerTrap();
};
struct CTCProjectileTrapV {                             // vtable view: slot 30 TriggerTrap
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void UpdateTrap();
    virtual void TriggerTrap();
};

void CTCProjectileTrap::UpdateTrap()
{
    if (!Triggered || !Active)
        return;
    if (!IS_THING_ALIVE(&PTarget)) {
        CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(hero))
            PTarget = hero;
    }
    if (!IS_THING_ALIVE(&PTarget))
        return;
    CThing* thing = ParentThing;
    if (thing->UsingComponents.test(0x43)) {
        long key = 0x43;
        CInterfaceMap* map = &thing->m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        CTCLook* look = (CTCLook*)((it == map->end() || 0x43 < it->first) ? map->end() : it)->second;
        look->LookAtPos(((CThing*)PTarget.GetPItem())->GetPos(), 0x17);
    }
    if (IdleTimer > 0) {
        --IdleTimer;
    } else {
        if (ShotTimer > 0)
            --ShotTimer;
        if (ShotTimer == 0) {
            if (NumShotsThisRound >= PTrapDef->NumShotsToFireBeforeIdle) {
                IdleTimer = (long)(CMainGameComponent::GetConstantFPS() * PTrapDef->SecondsIdle);
                NumShotsThisRound = 0;
            } else {
                ((CTCProjectileTrapV*)this)->TriggerTrap();
            }
        }
    }
}