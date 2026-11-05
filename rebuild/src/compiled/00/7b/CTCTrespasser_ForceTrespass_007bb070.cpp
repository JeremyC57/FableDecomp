// CTCTrespasser::ForceTrespass() -- when the trespassed thing is alive and trespassable, posts the trespass deed to the owner's opinion deed log, keeping the handle when no trespass deed is open yet
// Leaf: ForceTrespass
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BB070, 219 bytes (plain `ret`). Ego_r.pdb
// `?ForceTrespass@CTCTrespasser@@QAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (IS_THING_ALIVE, the trespassable test and the deed
//          target); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x51 = word 2 bit 17, the
//          opinion deed log); CThing::GetPos() const 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType,
//          const C3DVector&, const CThing*, CThingAICreature*, EOpinionPostDeedType, bool, bool, const CFaction*, bool,
//          long) 0x006C6990 (Ego_r.pdb signature; the landed callers spell it four different ways).
// Layout (Ego_r.pdb CTCTrespasser, retail equal up to the list): DeedHandles[3] +0x10, TrespassingThing +0x2d (an
//         intelligent pointer at an odd offset: the pointer classes are `#pragma pack(1)`); the thing's dead flag +0x91
//         bit 0, interface bitset +0x20 (0xBD, trespassable = word 5 bit 29), map +0x44.
// Shape: `T* log = 0;` preloads edi, and the out-parameter GetInterface result is `&&`-ed with the open-handle test, so
//        the absent component skips straight to the else call; both arms are complete PostDeed calls (deed type 2,
//        post type 0x12) differing in the seventh argument, the first storing the new handle.
class CTCBase;
class CBaseObject;
class CFaction;
class CThingAICreature;
#include "engine/C3DVector.h"

enum EOpinionDeedType { OPINION_DEED_TYPE_TRESPASS = 2 };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_0x12 = 0x12 };

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCHeroOpinionDeedLog { public:                  // interface 0x51
    long PostDeed(EOpinionDeedType deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};

#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CTCTrespasser { public:
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    unsigned char _pad0a[2];
    long          InsideBuildingNotTrespassingDeedHandle;   // +0x0c
    long          DeedHandles[3];               // +0x10
    long          CurrentDeedHandleIndex;       // +0x1c
    long          OccupantsKnowOfDeedHandleIndex;   // +0x20
    unsigned long FrameSomeoneWasInformedOfLastDeed;    // +0x24
    unsigned long FramePlayerLeft;              // +0x28
    bool          AttachedOpinionSource;        // +0x2c
    CBaseIntelligentPointer TrespassingThing;   // +0x2d CIntelligentPointer<CThing>
    void ForceTrespass();
};

void CTCTrespasser::ForceTrespass()
{
    if (IS_THING_ALIVE(TrespassingThing) && ((CThing*)TrespassingThing.GetPItem())->HasTC(TCI_TRESPASSABLE)) {
        CTCHeroOpinionDeedLog* log = 0;
        if (ParentThing->GetTC(TCI_HERO_OPINION_DEED_LOG, &log) && DeedHandles[2] == -1)
            DeedHandles[2] = log->PostDeed(OPINION_DEED_TYPE_TRESPASS, ParentThing->GetPos(), ((CThing*)TrespassingThing.GetPItem()), 0,
                                           OPINION_POST_DEED_TYPE_0x12, true, true, 0, false, 0);
        else
            log->PostDeed(OPINION_DEED_TYPE_TRESPASS, ParentThing->GetPos(), ((CThing*)TrespassingThing.GetPItem()), 0,
                          OPINION_POST_DEED_TYPE_0x12, true, false, 0, false, 0);
    }
}