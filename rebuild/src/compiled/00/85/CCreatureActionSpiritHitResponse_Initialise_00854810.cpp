// CCreatureAction_SpiritHitResponse::Initialise() -- base Initialise, set the action length from a global frame count, double the knock-back vector and apply it to the creature's physics, then tell the co-op spirit component (interface type 0xc7) it was hit
// Leaf: ?Initialise@CCreatureAction_SpiritHitResponse@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_SpiritHitResponseFrames (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00854810, length 158 bytes (+ int3 padding).
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call), __ftol2 @ 0x00BFEA70, CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8, twice), CTCPhysicsBase::ApplyAcceleration(const C3DVector&) @ 0x006B0210 (the creature's physics at +0x60),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0xc7 = bit 7 of the bitset byte at +0x38: `test cl,cl ; jns`),
// CTCCoopSpirit::OnHit @ 0x006707A0. Global 0x01375550: a long (15) converted through float (`fild ; call __ftol2`).
// Shape: TotalTime (+0x10) = (long)(float)global; the vector at +0xa8 is scaled by 2 through its inline `operator*=` (x87 `fadd st(0),st(0)`
// per component, addressed through esi, which stays live for the argument push); the creature is fetched BEFORE that push, so it is
// a named local; the spirit pointer is an UNINITIALISED out-parameter used unconditionally, so the OnHit call and the
// epilogue are duplicated into the miss path (which reads the never-written frame word). `this` becomes &PCreature (`add edi,8`) once dead.
// The PDB header (CCreatureAction_SpiritHitResponse.h) flattens the base and sits 0x0c high; modelled locally.
class CTCBase;
#include "engine/C3DVector.h"
class CTCPhysicsBase { public:
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCCoopSpirit { public:                      // interface type 0xc7 component
    void OnHit();                           // 0x006707A0
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    long TotalTime;                         // +0x10
    unsigned char _pad14[0xa8 - 0x14];      // +0x14
};
class CCreatureAction_SpiritHitResponse : public CCreatureActionBase { public:
    C3DVector Knockback;                    // +0xa8
    virtual void Initialise();
};

void CCreatureAction_SpiritHitResponse::Initialise()
{
    CCreatureActionBase::Initialise();
    TotalTime = (long)(float)CMainGameComponent::GetConstantFPS();
    Knockback *= 2.0f;
    CThing* creature = PCreature.Get();   // fetched before the argument push: a named local
    creature->GetPhysicsTC()->ApplyAcceleration(Knockback);
    CTCCoopSpirit* pSpirit;
    PCreature.Get()->GetTC(TCI_COOP_SPIRIT, &pSpirit);
    pSpirit->OnHit();
}