// CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo(const CCharString&) -- speak the named AI speech to the
// Leaf: ?SayToThingReactingTo@CAIStateGroup_OpinionReactionRoot@@QAEJABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// (alive, creature-typed) thing this reaction is about; the speech priority is 0x93 and non-pure-AI speakers get
// the extended form. Returns the action result, or 3 when there is nothing to speak to.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095EE70, length 259 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ThingReactingTo, four times), ctor @ 0x00A01B10, SetPItem
// @ 0x00A01B90 (the by-value pointer arguments built in their slots: CIntelligentPointer<const CThing> vtable
// 0x01237F1C for the target, CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8 for the speaker),
// CAIStateGroupBase::AIAction_SpeakAISpeechToTarget seven-argument @ 0x00981350 / six-argument @ 0x00981EA0.
// Shape: inlined IS_THING_ALIVE on the packed +0x6b pointer, then the creature type bit (ThingTypeFlags +0x6c bit 3)
// read through a third GetPItem; the target is a named local (ebx) read through a fourth.
// Engine header names (rebuild/include/engine/CAIStateGroup_OpinionReactionRoot.h); the header flattens the base,
// so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

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

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CCharString;

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)((CThing*)item)); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {       // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
inline bool IsCreatureThing(const CThing* thing)        // ThingTypeFlags (+0x6c) bit 3
{
    return (((const unsigned char*)thing)[0x6c] & 8) != 0;
}

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    void* PTCTalk;                              // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CBaseIntelligentPointer ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad73[0x88 - 0x73];
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    long AIAction_SpeakAISpeechToTarget(long repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerConstThing target, const CCharString& speech, bool interrupt, long priority,
        bool allowNonPureAI);                   // 0x00981350
    long AIAction_SpeakAISpeechToTarget(long repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerConstThing target, const CCharString& speech, bool interrupt, long priority);  // 0x00981EA0
    long SayToThingReactingTo(const CCharString& speech);
};
#pragma pack(pop)

long CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo(const CCharString& speech)
{
    if (IS_THING_ALIVE(ThingReactingTo) && IsCreatureThing(((CThing*)ThingReactingTo.GetPItem()))) {
        CThing* target = ((CThing*)ThingReactingTo.GetPItem());
        if (AllowSpeechOnNonPureAISpeaker)
            return AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, target, speech, false, 0x93, false);
        return AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, target, speech, false, 0x93);
    }
    return 3;
}