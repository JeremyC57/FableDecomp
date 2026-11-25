// CAIStateGroup_MeleeCombatNearby2::IsStillRunnable(bool) -- still runnable when the base says so AND the enemy
// Leaf: ?IsStillRunnable@CAIStateGroup_MeleeCombatNearby2@@MBE_N_N@Z
// NF137 (2026-11-20, q5030): the base call 0x0093D800 now binds the PDB ?IsStillRunnable@CAIStateGroup_MeleeCombatNearbyBase@@MBE_N_N@Z (in place on
//   the row-local base: protected virtual const), was QAE; own leaf respelled in place to the PDB protected virtual const
//   ?IsStillRunnable@CAIStateGroup_MeleeCombatNearby2@@MBE_N_N@Z (Ego_r.pdb 0x00961A40 + FableWin.pdb 0x02B90F10), was QAE (no landed referrer).
// and the creature stand on the same navigation region (the region lookup takes each thing's position).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DB10, length 94 bytes (+ int3 padding), `ret 4`.
// Callees: CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(bool) @ 0x0093D800 (the argument forwarded),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy at +0x14, twice), CThing::GetPos @ 0x004C73D0
// (const C3DVector&), the creature's region-of-position query @ 0x00661D20 (manifest "CFrontEndManager::
// CreateComponent" is a propagated label: it takes a C3DVector* and returns a long, called on both things).
// Shape: `if (Base(arg)) { enemy = GetPItem(); creature = PBrain->Creature; a = enemy->Region(GetPItem()->
// GetPos()); b = creature->Region(creature->GetPos()); if (a == b) return true; } return false;` --
// the nested-if form gives the single shared `xor al,al` tail that both misses jump to (an early `return
// false` guard emits its own epilogue, and `if (a != b) return false; return true;` becomes a `sete`); the
// named enemy/creature locals land in ebx/edi, the first region in esi. Member names from the engine header (PEnemy +0x14, Archer +0x1c); the header
// flattens the base, so the layout is local.
class C3DVector;

class CThing { public:
    const C3DVector& GetPos() const;         // 0x004C73D0
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                       // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;         // +0x14 CIntelligentPointer<CThingCreatureBase>
protected:
    virtual bool IsStillRunnable(bool checkEnemy) const;  // 0x0093D800
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroup_MeleeCombatNearbyBase {
    bool Archer;                            // +0x1c
protected:
    virtual bool IsStillRunnable(bool checkEnemy) const;
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00661D20 ?GetNavigationLevelAt@CThingCreatureBase@@QBEJABVC3DVector@@@Z (was ?GetRegionAtPosition@CThing@@QAEJABVC3DVector@@@Z)
class CThingCreatureBase { public: long GetNavigationLevelAt(C3DVector const&) const; };
bool CAIStateGroup_MeleeCombatNearby2::IsStillRunnable(bool checkEnemy) const
{
    if (CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(checkEnemy)) {
        CThing* enemy = PEnemy.GetPItem();
        CThing* creature = PBrain->Creature;
        long enemyRegion = reinterpret_cast<const CThingCreatureBase*>(enemy)->GetNavigationLevelAt(PEnemy.GetPItem()->GetPos());
        long creatureRegion = reinterpret_cast<const CThingCreatureBase*>(creature)->GetNavigationLevelAt(creature->GetPos());
        if (enemyRegion == creatureRegion)
            return true;
    }
    return false;
}