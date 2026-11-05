// CAIStateGroup_RangedCombatUsePreferredFirePoint::Update() -- register as a combatant against the enemy, strafe
// Leaf: ?Update@CAIStateGroup_RangedCombatUsePreferredFirePoint@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// (shooting) to the preferred fire point, stamp the arrival frame, keep attacking while still runnable, then
// deregister; any failure terminates the group.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D200, length 267 bytes (+ int3 padding).
// Callees: IsStillRunnable @ 0x0095D0E0 (x4), CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::
// RegisterCombatant(creature, enemy, bool) @ 0x006EDC50 / DeregisterCombatant @ 0x006ED9C0, CBaseIntelligentPointer
// GetPItem/ctor/SetPItem (the by-value CIntelligentPointer<const CThing> target, vtable 0x01237F1C), CThing::
// GetPosition @ 0x004C73D0 (the fire point; C3DVector by value through the user copy constructor),
// AIAction_ProjectileAttackAndStrafeToPos @ 0x0097F3A0, CAIStateGroupBase::GetWorld @ 0x008FCFC0 (result unused:
// the frame getter @ 0x0049D870 ignores `this`), AttackEnemy @ 0x0095D070, Terminate @ 0x008FCF40.
// Shape: separate `if (...) { Terminate(); return; }` exits tail-merged into one Terminate block; a do/while
// yield loop on the strafe action.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
protected:
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CThingCreatureBase;
class CCombatManager { public:
    void RegisterCombatant(CThingCreatureBase* creature, CThingCreatureBase* enemy, bool ranged);   // 0x006EDC50
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
};
class CThingCreatureBase : public CThing { public:
    CCombatManager& GetCombatManager();         // 0x00661ED0
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }
};

struct CAIStateGroup_RangedCombatUsePreferredFirePoint : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CBaseIntelligentPointer PFirePoint;         // +0x1c CIntelligentPointer<CThing>
    long TimeOutCounter;                        // +0x24
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    bool IsStillRunnable();                     // 0x0095D0E0
    bool AttackEnemy();                         // 0x0095D070
    long AIAction_ProjectileAttackAndStrafeToPos(long repeat, C3DVector pos, CIntelligentPointerConstThing target,
        bool* reached, long movement, bool a);  // 0x0097F3A0
    virtual void Update();
};

void CAIStateGroup_RangedCombatUsePreferredFirePoint::Update()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CThingCreatureBase* creature = GetCreature();
    creature->GetCombatManager().RegisterCombatant(creature, (CThingCreatureBase*)PEnemy.GetPItem(), false);
    bool reached;
    long result;
    do {
        result = AIAction_ProjectileAttackAndStrafeToPos(2,
            ((CThingCreatureBase*)PFirePoint.GetPItem())->GetPos(), PEnemy, &reached, 5, true);
    } while (result == 4);
    if (result != 2) {
        Terminate();
        return;
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    TimeOutCounter = GetWorld().GetFrame();
    while (AttackEnemy()) {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    GetCreature()->GetCombatManager().DeregisterCombatant(GetCreature());
}