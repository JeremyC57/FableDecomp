// CTCMeshAutomaticEntityCreator::CatchMessage(const CMessageOnMeshReplaced& message) -- the owner's mesh was replaced: kill (not immediately) every live owned thing that carries interface 0x32, prune the thing-owner list, then create the child things again
// Leaf: CatchMessage
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072E4C0, length 144 bytes (`ret 4`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCThingOwner, type 0x21 = bit 1 of
// the byte at CThing+0x24), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80 (false),
// CTCThingOwner::ClearDeadEntries @ 0x0071B240, CTCMeshAutomaticEntityCreator::CreateChildThings @ 0x0072E290.
// Layout (retail, local model): CThing bitset +0x20 (type 0x32 = bit 18 of +0x24), map +0x44, dead bit +0x91 bit 0; CTCThingOwner
// ObjectList +0x0C (PDB; the retail STLport list is the 4-byte sentinel pointer).
// Shape: HasTC is `if (bits & mask) return true; return false;` (`test dword ptr [eax+24h],40000h`; a `!= 0` form gives `shr ; test cl,1`
// and changes the register allocation); the walk re-reads the list end each pass; VC keeps the constant 1 of the dead test in bl and spills `this` to the frame.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct SThingNode {
    SThingNode* Next;
    SThingNode* Prev;
    CIntelligentPointer<CThing> Thing;
};
class CTCThingOwner { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    SThingNode* ObjectList;                     // +0x0c list<CIntelligentPointer<CThing>> sentinel
    void ClearDeadEntries();                    // 0x0071B240
};
struct CMessageOnMeshReplaced;
struct CTCMeshAutomaticEntityCreator {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
private:
    void CreateChildThings();                   // 0x0072E290 ?CreateChildThings@CTCMeshAutomaticEntityCreator@@AAEXXZ (Ego_r/FableWin: private)
public:
    void CatchMessage(const CMessageOnMeshReplaced& message);
};

void CTCMeshAutomaticEntityCreator::CatchMessage(const CMessageOnMeshReplaced& message)
{
    CTCThingOwner* owner;
    if (ParentThing->GetTC(TCI_THING_OWNER, &owner)) {
        for (SThingNode* it = owner->ObjectList->Next; it != owner->ObjectList; it = it->Next) {
            CThing* thing = it->Thing.Get();
            if (thing && !thing->IsDead() && thing->HasTC(TCI_CREATED_ENTITY))
                thing->Kill(false);
        }
        owner->ClearDeadEntries();
    }
    CreateChildThings();
}