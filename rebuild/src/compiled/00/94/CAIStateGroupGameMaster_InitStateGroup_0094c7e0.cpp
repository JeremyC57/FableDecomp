// CAIStateGroup_GameMaster::InitStateGroup(const CWorld& world, CThingAICreature& creature) -- runs the go-to-pub initialisation, then resets the react timer, the follow flag and the pub-thing pointer
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094C7E0, length 102 bytes (+ int3 padding), `ret 8`.
// Callees: CAIStateGroup_GoToPub::InitStateGroup @ 0x00947DB0 (the base implementation, both arguments forwarded),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/90/50/0x00A01C10.
// Shape: base call first (arguments loaded before `push esi`), then TimeNextReactToPlayer (+0xa8) and
// WasAllowedToFollowPlayer (+0xac) cleared with immediates, then `PBaseThing = CIntelligentPointer<..>(0)` -- a
// frame temporary through the inline converting constructor (vtable 0x01244344), the inline operator= and the
// base destructor.
// Model: local (no engine header for these classes; GameMaster member names and offsets from struct_layouts_egor.tsv,
// whose CAIStateGroup_GoToPub is 0xa8 bytes like retail).
class CThing;
class CWorld;
class CThingAICreature;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<..>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointerThing PBaseThing;            // +0x14
    unsigned char _pad_0x1c[0xa8 - 0x1c];           // +0x1c..+0xa7
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);   // 0x00947DB0
};

struct CAIStateGroup_GameMaster : CAIStateGroup_GoToPub {
    long TimeNextReactToPlayer;                     // +0xa8
    bool WasAllowedToFollowPlayer;                  // +0xac
protected:                                          // `MAE` in the retail symbol
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_GameMaster::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    CAIStateGroup_GoToPub::InitStateGroup(world, creature);
    TimeNextReactToPlayer = 0;
    WasAllowedToFollowPlayer = false;
    PBaseThing = CIntelligentPointerThing(0);
}