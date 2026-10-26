// CThingCreatureBase::GetMaxSneakingSpeed(const C3DVector& direction) const -- the sneaking speed limit from the creature's movement component (kind 0x36); 0 without one (the direction is unused)
// Leaf: GetMaxSneakingSpeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664CB0, length 71 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation, key by address), the
// component's float getter @ 0x00747020 (`fld dword ptr [ecx+18h]`; manifest "CMap::DrawGetSeaLevel" is propagated).
// Identity: Ego_r.pdb orders `IsAbleToRecoverFromStunned` (0x0053EC90), `?GetMaxSneakingSpeed@CThingCreatureBase@@QBEMABVC3DVector@@@Z`
// (0x0053ECF0), `IsBlocking` (0x0053ED50); retail has 0x00664C60, this body, 0x00664D00 in the same order, and the one
// caller (CActionFollowCreature::Update, 0x007140F8) passes a C3DVector built on its stack: the catalog's
// "IsBlocking" is a propagated label.
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last}; engine/CThingCreatureBase.h is the PDB layout and names
// nothing at +0x20..+0x50, so a local CThing model is used.
// Shape: `HasTC(k) ? GetTC(k)->f() : 0.0f` with the pointer-returning inline find (component loaded straight into ecx),
// the float pool 0x0122DEDC = 0.0f for the absent case, `ret 4` for the unused reference argument.
class CTCBase;

struct STCEntry {                               // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                 // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (CVectorMap<ETCInterfaceType,...>::LowerBound)
    CTCBase* Find(long kind) const                 // the inlined map find: lower bound, then end when the key is absent
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) const { return m_tcs.Find(kind); }
};

class C3DVector;

struct CTCSneakMovement {                       // component kind 0x36
    float GetMaxSneakingSpeed() const;          // 0x00747020
};

class CThingCreatureBase : public CThing { public:
    float GetMaxSneakingSpeed(const C3DVector& direction) const;
};

float CThingCreatureBase::GetMaxSneakingSpeed(const C3DVector& direction) const
{
    if (HasTC(0x36))
        return ((CTCSneakMovement*)GetTC(0x36))->GetMaxSneakingSpeed();
    return 0.0f;
}