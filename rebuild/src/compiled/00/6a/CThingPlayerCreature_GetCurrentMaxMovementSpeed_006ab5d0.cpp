// CThingPlayerCreature::GetCurrentMaxMovementSpeed() const -- the speed limit for the current movement band: 0 while blocking, the stealth component's sneaking speed when sneaking (always under the sneak-speed option, else in the slow/walking bands), else walking/running/jogging by band (3 = running, 4 = jogging), overridden by sprinting when the hero special-movement component is sprinting
// Leaf: ?GetCurrentMaxMovementSpeed@CThingPlayerCreature@@IBEMXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB5D0, 412 bytes including the 5-entry band jump table at 0x006AB758.
// Name: Ego_r.pdb ?GetCurrentMaxMovementSpeed@CThingPlayerCreature@@IBEMXZ (protected const; the parked file was public).
// Callees (Ego_r.pdb spellings): CTCPhysicsBase::GetFacingVector() const (virtual slot 70, +0x118, by-value C3DVector: the only
//   UBE ?AVC3DVector@@XZ member of CTCPhysicsBase), CThing::GetPos() const 0x004C73D0, CThingCreatureBase::IsBlocking() const
//   0x00664D00, CTCStealth::GetSneakingSpeed() const 0x00747020 (`fld [ecx+18h]` = Ego_r CTCStealth::MaxSneakingSpeed +0x18;
//   interface 0x36 = CTCStealth, Sneaking +0x0F), the const interface-map LowerBound 0x004365B0 (fleet descriptive spelling),
//   the virtual GetMaxWalkingSpeed / GetMaxJoggingSpeed / GetMaxRunningSpeed / GetMaxSprintingSpeed(const C3DVector&) const
//   (slots 79-82). Interface 0xDF = CTCHeroSpecialMovement (Ego_r Sprinting +0x0C).
// Data: float pool 0.0f 0x0122DEDC; the sneak-speed option byte 0x01375749 (descriptive name g_bSneakSpeedOption).
// Shape: the parked 412v412 body (band switch inline, single return of `speed`) plus two levers: the const component lookups use
//   the out-parameter PeekTC form (Ego_r ??$PeekTC@$$CBV..@CThing@@QBE_NPAPBV..@@Z), which loads the 0xDF component into eax and
//   its flag into cl as retail; the sprint override goes through a named float (`float sprint = ...; speed = sprint;`), which
//   keeps the position + facing sum's X operand order (fld facing.X ; fadd pos.X) that the PeekTC change had flipped.
// Band switch (jump table 0x006AB758, checked by vland RELOC_JUMPTABLE): 0 -> 0, 1/2 -> sneak or walking, 3 -> running (slot 81),
//   4 -> jogging (slot 80), default -> running; slot names from the Ego_r donor order of CThingPlayerCreature / CThingAICreature.
// Layout (retail, local model; see NOTES.md header debt): component bitset +0x20, component map +0x44, physics TC +0x60,
//   MovementBandType +0x200 (Ego_r.pdb +0x22C minus 0x10 CThing and 0x1C of container shrink). The vtable fillers v000..v078 and
//   v00..v69 are unnamed slot placeholders; only the named slots are proven.
#include <math.h>
#include "engine/C3DVector.h"
class CTCBase;
class CTCPhysicsBase {
public:
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacingVector() const;  // slot 70 (+0x118), Ego_r.pdb ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@XZ
};
struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase* Find(long kind) const
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return (it == m_pLast || kind < it->m_kind ? m_pLast : it)->m_pTC;
    }
};
class CTCStealth {                              // interface 0x36
public:
    unsigned char _pad00[0x0f];
    bool          Sneaking;                     // +0x0f (Ego_r.pdb)
    float GetSneakingSpeed() const;             // 0x00747020 (fld [ecx+18h]: MaxSneakingSpeed)
};
class CTCHeroSpecialMovement {                  // interface 0xDF
public:
    unsigned char _pad00[0x0c];
    bool          Sprinting;                    // +0x0c (Ego_r.pdb)
    bool          Running;                      // +0x0d
};
class CThing {
public:
    virtual void v000();
    unsigned char  _pad04[0x1c];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned char  _pad50[0x60 - 0x50];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) const { return m_tcs.Find(kind); }
    const C3DVector& GetPos() const;            // 0x004C73D0
    template <class T> __forceinline bool PeekTC(const T** ppOut, long kind) const
    {
        if (HasTC(kind)) {
            *ppOut = (const T*)m_tcs.Find(kind);
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsBlocking() const;                    // 0x00664D00
};
class CThingPlayerCreature : public CThingCreatureBase {
public:
    unsigned char _pad64[0x200 - 0x64];
    long MovementBandType;                      // +0x200 EMovementBandType
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078();
    virtual float GetMaxWalkingSpeed(const C3DVector& direction) const;     // slot 79 (+0x13c)
    virtual float GetMaxJoggingSpeed(const C3DVector& direction) const;     // slot 80 (+0x140)
    virtual float GetMaxRunningSpeed(const C3DVector& direction) const;     // slot 81 (+0x144)
    virtual float GetMaxSprintingSpeed(const C3DVector& direction) const;   // slot 82 (+0x148)
protected:
    float GetCurrentMaxMovementSpeed() const;
    __forceinline float GetBandSpeed(const C3DVector& direction) const
    {
        switch (MovementBandType)
        {
        case 0:
            return 0.0f;
        case 1:
        case 2:
            {
                const CTCStealth* sneak;
                if (PeekTC(&sneak, 0x36) && sneak->Sneaking)
                    return sneak->GetSneakingSpeed();
            }
            return GetMaxWalkingSpeed(direction);
        case 3:
            return GetMaxRunningSpeed(direction);
        case 4:
            return GetMaxJoggingSpeed(direction);
        default:
            return GetMaxRunningSpeed(direction);
        }
    }
};
extern bool g_bSneakSpeedOption;                // 0x01375749

float CThingPlayerCreature::GetCurrentMaxMovementSpeed() const
{
    C3DVector direction = GetPos() + PhysicsTC->GetFacingVector();
    if (IsBlocking())
        return 0.0f;
    if (g_bSneakSpeedOption)
    {
        const CTCStealth* sneak;
        if (PeekTC(&sneak, 0x36) && sneak->Sneaking)
            return sneak->GetSneakingSpeed();
    }
    float speed = GetBandSpeed(direction);
    const CTCHeroSpecialMovement* special;
    if (PeekTC(&special, 0xdf) && special->Sprinting) {
        float sprint = GetMaxSprintingSpeed(direction);
        speed = sprint;
    }
    return speed;
}