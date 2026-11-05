// CAIStateGroup_UsesSeat::OnReplace() -- release the seat's action point when one was taken, and when sitting get up (movement 8, or 9 with the alternate get-up name), removing the movement mode if the get-up fails
// Leaf: ?OnReplace@CAIStateGroup_UsesSeat@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009387F0, length 368 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PSeat +0x14), the inlined GetTC 0x2d (CTCUserActionPointBase,
//          bit 13 of +0x24) and 0x31 (CTCCreatureModeManager, bit 17 of +0x24), the action-point release @ 0x007FEF40,
//          CCharString copy ctor @ 0x0099EC30 (from the globals @ 0x013BB6E0 / 0x013BB6E8) / dtor @ 0x0099EAE0,
//          CBrainOutput ctor @ 0x00835620 (implicit destructor: intelligent pointer +0x40, then the counted pointer +0x20),
//          GetUpFromSitting @ 0x0085FF10 (__fastcall), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, operator delete
//          @ 0x00BFE9BC, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the seat release is guarded by the flag and a live seat (out-param GetTC, miss reads the key slot); the get-up
//          name is copied from one of two global strings; an unused CBrainOutput local (0x48 bytes) is constructed and
//          destroyed around the get-up call.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04) with PSeat +0x14,
//          the seat flag +0x1c, sitting +0x1d, the action point index +0x20 and the alternate flag +0x30 (descriptive).
class CCharStringData;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
extern CCharString g_GetUpName;                 // 0x013BB6E0 (descriptive)
extern CCharString g_GetUpNameAlternate;        // 0x013BB6E8 (descriptive)
class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

__forceinline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
struct CTCUserActionPointBase {
public:
    void SetActionPointAsFree(long a0);   // 0x007FEF40 (Ego_r.pdb ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z)
public:                 // interface 0x2d
};
class CTCCreatureModeManager { public:                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CCountBlock { long Count; void (__fastcall* Deleter)(void*); void* Object; };
struct CCountedPointer {                        // CCountedPointer<T>: object + count block, released inline
    void* Object;
    CCountBlock* Block;
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Deleter(Block->Object);
                operator delete(Block);
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:
    unsigned char _pad[0x20];
    CCountedPointer Counted;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CIntelligentPointer<CThing> PTarget;            // +0x40 (retail offset; the frame places it 0x40 into the 0x48-byte local)
    CBrainOutput();                             // 0x00835620
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
bool __fastcall GetUpFromSitting(CThing* creature, long movement, const CCharString* anim, CAIBrain* brain);   // 0x0085FF10

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CIntelligentPointer<CThing> PSeat;              // +0x14
    bool GotSeat;                               // +0x1c (descriptive)
    bool Sitting;                               // +0x1d (descriptive)
    unsigned char _pad1e[2];
    long ActionPointIndex;                      // +0x20 (descriptive)
    unsigned char _pad24[0x30 - 0x24];
    bool AlternateGetUp;                        // +0x30 (descriptive)
    virtual void OnReplace();
};

void CAIStateGroup_UsesSeat::OnReplace()
{
    if (GotSeat && IS_THING_ALIVE(PSeat)) {
        CTCUserActionPointBase* points;
        PSeat.Get()->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &points);
        ((CTCUserActionPointBase*)points)->SetActionPointAsFree(ActionPointIndex);
        GotSeat = false;
    }
    ActionPointIndex = -1;
    if (Sitting) {
        long movement = (AlternateGetUp != 0) + 8;
        CCharString anim(AlternateGetUp ? g_GetUpNameAlternate : g_GetUpName);
        CBrainOutput output;
        if (!GetUpFromSitting(GetCreature(), movement, &anim, PBrain)) {
            CTCCreatureModeManager* modes;
            GetCreature()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
            modes->RemoveMode(movement);
        }
        Sitting = false;
    }
}