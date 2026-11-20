// CAIStateGroup_OpinionReactionRoot::WatchUpdate() -- keep looking at the thing reacted to for 30 + random(0..44)
// Leaf: ?WatchUpdate@CAIStateGroup_OpinionReactionRoot@@QAEJXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// frames: 1 when the look action finishes (4) or the time is up, 0 on any other result (a long result).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00984A20, length 149 bytes.
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CWorld::GetFrame @ 0x0049D870
// (twice), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @
// 0x00A01B90 (the by-value CIntelligentPointer<const CThing> argument, vtable 0x01237F1C, converted in its argument
// slot from the member at +0x6b: the landed 0x0095EDF0 TurnToFace shape), CAIStateGroupBase::AIAction_LookAtThing
// @ 0x0097D6F0 (repeat 1, flags 0x21, turn speed 0.0027777f).
// Shape: the random step with the RANGE PASSED BY CONST REFERENCE (`mov ecx,2Dh ; div ecx`), the end frame in
// ebp (`lea ebp,[eax+esi+1Eh]`), the member address in ebx; a `do { r = LookAtThing(...); if (r != 2) return
// r == 4 ...; } while (GFGetCurrentFrame() < end); return true;` loop with the full-register results
// (`mov eax,1` / `xor eax,eax`). The member is unaligned at +0x6b (engine header, pack 1), so the class is
// modelled packed.
// NF175 (2026-11-20, q4968): AIAction_LookAtThing 0x0097D6F0 respelled from the CAIStateGroupBaseL stand-in
//   (?AIAction_LookAtThing@CAIStateGroupBaseL@@QAEJJUCIntelligentPointerConstThing@@JM@Z) to the PDB
//   ?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z (Ego_r.pdb
//   0x008484A0, protected), declared on the row-local CAIStateGroupBase. The local CBaseIntelligentPointer / CBaseIntelligentPointerCtor /
//   CIntelligentPointerThing / CIntelligentPointerConstThing stand-ins are replaced by the shared engine/CIntelligentPointer.h (retail
//   branch, converting constructor inline), so 0x00A01B10 / 0x00A01B50 / 0x00A01B90 now bind the shared CBaseIntelligentPointer spellings;
//   the enums are engine/EAIAction.h.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#include "engine/CIntelligentPointer.h"
#include "engine/EAIAction.h"

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0 (world random seed)
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long flags, float turnSpeed);   // 0x0097D6F0
    public:
};

class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CTCTalk;

struct CAIStateGroupBaseL : CAIStateGroupBase {
    // a random frame count below `range`: the range arrives by const reference, so the literal is materialised
    // in a register and divided with `div` (the landed seed step: seed*0x24A1+0x24DF, GFROR13)
    FABLE_FORCEINLINE unsigned long RandomFrames(const unsigned long& range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBaseL { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointer<CThing> ThingReactingTo;   // +0x6b
    long WatchUpdate();                         // full-register 1 / 0 result (`mov eax,1` / `xor eax,eax`)
};
#pragma pack(pop)

long CAIStateGroup_OpinionReactionRoot::WatchUpdate()
{
    long endFrame = CWorld::GetFrame() + RandomFrames(45) + 30;
    do {
        long result = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, ThingReactingTo, 0x21, 0.0027777f);
        if (result != 2) {
            if (result != 4)
                return 0;
            break;                              // shares the `mov eax,1` exit of the timed-out loop
        }
    } while (CWorld::GetFrame() < endFrame);
    return 1;
}