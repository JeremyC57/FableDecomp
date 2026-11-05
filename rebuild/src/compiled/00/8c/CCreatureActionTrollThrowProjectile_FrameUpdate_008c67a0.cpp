// CCreatureAction_TrollThrowProjectile::FrameUpdate() -- fade the carried projectile in: raise ShotAlpha by 16 a frame up to 255 (snapped to 255 once the troll's hands are empty), push it to the shot's graphic appearance while the shot exists and is not being deleted, then the combat-action frame update
// Leaf: ?FrameUpdate@CCreatureAction_TrollThrowProjectile@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C67A0, length 192 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PShot three times), CVectorMap::LowerBound 0x0040F020
// (the non-const interface lookup of CTCCarrying, interface 0x46 = bit 6 of the present-bits byte +0x28), CTCCarrying::
// GetThingInPrimarySlot 0x006A1440, CTCGraphicAppearance::SetAlpha(unsigned char) 0x004BFAC0 (the component at thing +0x64),
// CCombatActionBase::FrameUpdate 0x00859D80 (direct base call, last).
// Layout: retail PShot +0x114 and ShotAlpha +0x12C (the PDB header engine/CCreatureAction_TrollThrowProjectile.h has them
// 0x18 higher, +0x12C/+0x144, because the debug build's base is larger), so the action is a local model.
// Shape: the clamp is two statements (the second re-reads the member); the interface fetch is the inline GetInterface with
// the absent path skipping the test; the shot is fetched into a named local before the alpha argument is pushed; its "being deleted" byte is bit 0 of thing +0x91.
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;
class CTCCarrying;
class CThing;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAlpha(unsigned char alpha);         // 0x004BFAC0
};
}
using NGraphicAppearance::CTCGraphicAppearance;

class CTCCarrying { public:
    CThing* GetThingInPrimarySlot() const;            // 0x006A1440
};

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
    virtual void v00();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void FrameUpdate();                         // 0x00859D80
};

class CCreatureAction_TrollThrowProjectile : public CCombatActionBase { public:
    CIntelligentPointer<CThing> PShot;              // +0x114
    unsigned char           _pad11c[0x12c - 0x11c];
    long                    ShotAlpha;          // +0x12c
    void FrameUpdate();
};

void CCreatureAction_TrollThrowProjectile::FrameUpdate()
{
    if (ShotAlpha < 255)
        ShotAlpha += 16;
    if (ShotAlpha > 255)
        ShotAlpha = 255;
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetTC(TCI_CARRYING, &carrying) && carrying->GetThingInPrimarySlot() == 0)
        ShotAlpha = 255;
    if (PShot.GetPItem() && !PShot.GetPItem()->IsDead()) {
        CThing* shot = PShot.GetPItem();
        shot->GetTCGraphicAppearance()->SetAlpha((unsigned char)ShotAlpha);
    }
    CCombatActionBase::FrameUpdate();
}