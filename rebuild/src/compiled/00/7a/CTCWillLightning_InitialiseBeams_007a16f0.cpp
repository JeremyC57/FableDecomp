// CTCWillLightning::InitialiseBeams() -- gather the main beams toward targets around the caster, then for every main beam mark it main, pick its hand, gather its sub-beams and activate it; finally flag the beams initialised and ask the caster's targeting component to pick a new lock target
// Leaf: ?InitialiseBeams@CTCWillLightning@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members Count->RefCount, Destroy->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A16F0, length 375 bytes (plain `ret`).
// Callees: the CRT `malloc` / `free` import thunks @ 0x00BFEA0E / 0x00BFEA14 (the local STLport list<CThing*> head node
//   and its nodes), CTCWillLightning::GetTargets @ 0x007A0D90 (manifest signature: beam list, max beams, origin thing,
//   target list, damage, decapitation time, delay, bool), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//   CTCWillLightning::CLightningBeam::Activate @ 0x007A1380, operator delete @ 0x00BFE9BC (the counted-pointer info block),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC on interface 8),
//   CTCTargetingPlayer::SetAsPickUpNewLockTarget @ 0x008023A0.
// Layout: retail CTCWillLightning is the PDB layout less 0xC after the timer (12-byte STLport vector, 4-byte list, see
//   the landed Deactivate 0x007A0310 / ResumeForCutscene 0x007A0400): WeaponState +0x0C, LeftHandAnim +0x10,
//   m_lbBeams +0x68, BeamsInitialised +0x95, m_fDamagePerHit +0xA0, m_fSubBeamDamagePerHit +0xA4, m_nMaxMainBeams +0xB4,
//   m_nMaxSubBeams +0xBC, m_fMainBeamDecapitationTime +0xCC, m_fSubBeamDecapitationTime +0xD0, m_fSubBeamDelay +0xD4.
//   CLightningBeam (landed ctor 0x007A0630): Target +0x14, Children +0x4C, MainBeam +0x51, RightHand +0x52. CThing m_type
//   byte +0x10 (2 tested here), interface bitset +0x20, CVectorMap +0x44.
// Shape: the local target list is built by its inline ctor (malloc'd sentinel); the beam is a by-value
//   CCountedPointer copy (add-ref on the info block, inline release); the hand is a switch on WeaponState. GetTargets
//   (0x007A0D90, same TU) gets a same-TU `auto_inline(off)` stand-in that writes through the beam list and only reads
//   the target list: VC7.1 then keeps the target list's head word in edi/[esp+14h] and re-reads m_lbBeams after the
//   call, as retail. The caster pointer and the beam target are named locals (retail loads them before the other
//   argument loads).
#include <stdlib.h>
class CTCBase;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCTargetingPlayer { public:
    void SetAsPickUpNewLockTarget();            // 0x008023A0
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


void operator delete(void* p);                  // 0x00BFE9BC

#include "engine/CCountedPointer.h"

template <class T> struct CRetailList {          // STLport list: the sentinel node pointer only (4 bytes; PDB 12)
    struct Node { Node* Next; Node* Prev; T Value; };
    Node* Head;
};

struct CThingPtrList {                          // the local list<CThing*>
    struct Node { Node* Next; Node* Prev; CThing* Value; };
    Node* Head;
    CThingPtrList()
    {
        Node* node = (Node*)malloc(sizeof(Node));
        node->Next = node;
        node->Prev = node;
        Head = node;
    }
    ~CThingPtrList()
    {
        Node* cur = Head->Next;
        while (cur != Head) {
            Node* tmp = cur;
            cur = cur->Next;
            free(tmp);
        }
        Head->Next = Head;
        Head->Prev = Head;
        free(Head);
    }
};

struct CLightningBeam;
typedef CRetailList<CCountedPointer<CLightningBeam> > CBeamList;

struct CLightningBeam {
    long State;                                 // +0x00
    CIntelligentPointer<CThing> ParticleHandle;     // +0x04
    CIntelligentPointer<CThing> Home;               // +0x0c
    CIntelligentPointer<CThing> Target;             // +0x14
    float Start[3], End[3], Destination[3];     // +0x1c
    float DamagePerHit, Time, TimeBase;         // +0x40
    CBeamList Children;                         // +0x4c
    bool Temporary;                             // +0x50
    bool MainBeam;                              // +0x51
    bool RightHand;                             // +0x52
    void Activate();                            // 0x007A1380
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad_0x08[0x4];
};

class CTCWillLightning : public CTCBase { public:             // vtable 0x0126C724
    long  WeaponState;                          // +0x0c
    bool  LeftHandAnim;                         // +0x10
    unsigned char _pad_0x11[0x68 - 0x11];
    CBeamList m_lbBeams;                        // +0x68
    unsigned char _pad_0x6c[0x95 - 0x6c];
    bool  BeamsInitialised;                     // +0x95
    unsigned char _pad_0x96[0xa0 - 0x96];
    float m_fDamagePerHit;                      // +0xa0
    float m_fSubBeamDamagePerHit;               // +0xa4
    float m_fActivationTime;                    // +0xa8
    float m_fDeActivationTime;                  // +0xac
    long  m_nMinMainBeams;                      // +0xb0
    long  m_nMaxMainBeams;                      // +0xb4
    long  m_nMinSubBeams;                       // +0xb8
    long  m_nMaxSubBeams;                       // +0xbc
    float m_fTemporaryBeamLife;                 // +0xc0
    float m_fTargettingFOV;                     // +0xc4
    float m_fStaminaPerSecond;                  // +0xc8
    float m_fMainBeamDecapitationTime;          // +0xcc
    float m_fSubBeamDecapitationTime;           // +0xd0
    float m_fSubBeamDelay;                      // +0xd4

    bool GetTargets(CBeamList* beams, long maxBeams, CThing* origin, CThingPtrList* targets,
                    float damagePerHit, float decapitationTime, float delay, bool subBeams);   // 0x007A0D90
    bool IsRightHanded() const
    {
        switch (WeaponState) {
        case 0:
            return !LeftHandAnim;
        case 1:
            return false;
        case 2:
            return true;
        }
        return true;
    }
    void InitialiseBeams();
};

// Same-TU stand-in for GetTargets (0x007A0D90 sits in this TU): only its visibility matters -- VC7.1 then knows the
// target list's head word does not escape and keeps it in a register across the calls, as retail's compiler did.
#pragma auto_inline(off)
bool CTCWillLightning::GetTargets(CBeamList* beams, long maxBeams, CThing* origin, CThingPtrList* targets,
                                  float damagePerHit, float decapitationTime, float delay, bool subBeams)
{
    if (maxBeams <= 0 || origin == 0 || damagePerHit + decapitationTime + delay < 0.0f)
        return false;
    CBeamList::Node* head = beams->Head;
    head->Prev->Next = head;
    return subBeams || (targets->Head->Next != targets->Head);
}
#pragma auto_inline(on)

void CTCWillLightning::InitialiseBeams()
{
    CThingPtrList targets;
    CThing* parent = ParentThing;
    if (GetTargets(&m_lbBeams, m_nMaxMainBeams, parent, &targets, m_fDamagePerHit,
                   m_fMainBeamDecapitationTime, 0.0f, m_lbBeams.Head->Next != m_lbBeams.Head)) {
        for (CBeamList::Node* it = m_lbBeams.Head->Next; it != m_lbBeams.Head; it = it->Next) {
            CCountedPointer<CLightningBeam> beam = it->Value;
            beam->MainBeam = true;
            beam->RightHand = IsRightHanded();
            CThing* target = beam->Target.Get();
            GetTargets(&beam->Children, m_nMaxSubBeams, target, &targets, m_fSubBeamDamagePerHit,
                       m_fSubBeamDecapitationTime, m_fSubBeamDelay, false);
            beam->Activate();
        }
        BeamsInitialised = true;
        CThing* caster = ParentThing;
        if (caster->GetType() == 2 && caster->HasTC(TCI_TARGETING))
            ((CTCTargetingPlayer*)&caster->GetTC(TCI_TARGETING))->SetAsPickUpNewLockTarget();
    }
}