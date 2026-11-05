// CAIStateGroup_BoastingCrowdMember::IsStillValid() -- still valid while the boaster is alive, its crowd component (interface 0x74) is in an active state (neither 0 nor 10) and that component's audience target is alive
// Leaf: ?IsStillValid@CAIStateGroup_BoastingCrowdMember@@QAEJXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E870, length 134 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on the boaster pointer at +0x14, once on
// the component's pointer at +0x24), the interface-map lower bound @ 0x004365B0 (a second instantiation of the
// 0x0040F020 CVectorMap::LowerBound; interface 0x74 = bitset word 3 bit 20 -> `test dword ptr [eax+2Ch],100000h`).
// Shape: nested ifs with one `return 1` (`mov eax,1`: an int-width result, not bool) and the shared
// `xor eax,eax` tail; the dead flag is bit 0 of the thing's byte at +0x91; the boaster pointer's address is
// formed once into esi and re-fetched for every use; the lookup is the landed GetInterface out-parameter
// template whose bool is tested.
// Model: local (no engine header for this class); the boaster pointer is a CIntelligentPointer<CThing>.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


struct CTCBoastingCrowd {                       // interface 0x74
    unsigned char _pad[0x24];
    CIntelligentPointer<CThing> PAudienceTarget;    // +0x24
    long State;                                 // +0x2c
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_BoastingCrowdMember : CAIStateGroupBase {
    CIntelligentPointer<CThing> PBoaster;           // +0x14
    long IsStillValid();
};

long CAIStateGroup_BoastingCrowdMember::IsStillValid()
{
    if (PBoaster.Get() != 0 && !PBoaster.Get()->IsDead()) {
        const CTCBoastingCrowd* crowd;
        if (PBoaster.Get()->PeekTC(TCI_CREATURE_GROUP_BOASTING_CROWD, &crowd)) {
            if (crowd->State != 0 && crowd->State != 10) {
                CThing* target = crowd->PAudienceTarget.Get();
                if (target != 0 && !target->IsDead())
                    return 1;
            }
        }
    }
    return 0;
}