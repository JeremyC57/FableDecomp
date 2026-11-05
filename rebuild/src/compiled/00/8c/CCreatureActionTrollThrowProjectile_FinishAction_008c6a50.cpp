// CCreatureAction_TrollThrowProjectile::FinishAction() -- if the troll still carries the live shot, kill the shot; clear the combat component's word +0x7c; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_TrollThrowProjectile@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C6A50, length 207 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x46 =
// CTCCarrying, then 0x30 = the combat component), CTCCarrying::IsCarryingThing(CThing*) @ 0x006A1060, CThing::Kill(bool)
// @ 0x004C9B80, CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: two CHECKED out-parameter lookups in the function body sharing the one key word; the shot is tested present / not
// dead (thing byte +0x91 bit 0) before the bool carrying query; the combat word +0x7c is zeroed through the component in eax.
// Layout: Ego_r.pdb PShot +0x12c, retail +0x114 (CCombatActionBase is 0x114 in retail).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCarrying { public:
    bool IsCarryingThing(CThing* thing);        // 0x006A1060
};
class CTCCombat { public:
    unsigned char _pad[0x7c];
    long          m_word7c;                     // +0x7c
};

#include "engine/C3DVector.h"

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:
    protected: virtual void FinishAction(); public:                // 0x00859A20
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

class CCreatureAction_TrollThrowProjectile : public CCombatActionBase { public:
    CIntelligentPointer<CThing> PShot;              // +0x114 (PDB +0x12c)
    C3DVector               TargetPos;          // +0x11c
    bool                    UseTargetPos;       // +0x128
    long                    ShotAlpha;          // +0x12c
    virtual void FinishAction();
};

void CCreatureAction_TrollThrowProjectile::FinishAction()
{
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetTC(TCI_CARRYING, &carrying)) {
        if (PShot.GetPItem() && !PShot.GetPItem()->IsDead() && carrying->IsCarryingThing(PShot.GetPItem()))
            PShot.GetPItem()->Kill(true);
    }
    CTCCombat* combat;
    if (PCreature.GetPItem()->GetTC(TCI_COMBAT, &combat))
        combat->m_word7c = 0;
    CCombatActionBase::FinishAction();
}