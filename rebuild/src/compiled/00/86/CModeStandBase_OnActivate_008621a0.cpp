// CModeStandBase::OnActivate() -- base activate, look up the stand/slow-walk/walk/jog/run/sprint animations (stand falls back to DEFAULT, missing gaits to stand/run), refresh the player's movement speeds or measure the walk/jog/run speeds, then restart the animation when the creature is idle
// Leaf: OnActivate
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- engine/CCountedPointer.h model plus the ternary GetInterface store (landed 0x007A0490 form), which fixes the zero-register choice around the ST_RUN/ST_SPRINT temporaries
// Retail: Fable.exe (TLC, VC7.1) @ 0x008621A0, 1504 bytes (virtual, `ret`).
// Callees: CTCCreatureModeManager::CModeBase::OnActivate 0x0073AAC0, CModeBase::GetWorld 0x0073A680, the world's animation
//   bank getter 0x0049D810 (world +8 + 0x1613C), own virtual slot 4 (+0x10), the file-static animation lookup 0x00861E60
//   (__fastcall, result in ecx, name in edx) and speed helper 0x00861950 (__fastcall), CCharString 0x0099EBF0 / ~0x0099EAE0,
//   operator new 0x00BFEA1A, CThingCreatureBase::GetAnimFromName 0x00662A00, CAnimEntryLite(CAnimationEntry*) 0x0073A9B0,
//   CCountedPointer<CAnimEntryLite>::Reset 0x00863090 / operator= 0x00754F10 (out of line), operator delete 0x00BFE9BC,
//   CThingPlayerCreature::InitialiseMovementSpeeds 0x006A9A00, CVectorMap LowerBound 0x0040F020 (component 0x5a),
//   0x0070B460 (component -> animate thing), CAnimateThing::SetAsFinished 0x0070BE80, IsPerformingAnyAction 0x006622A0,
//   own virtual slot 2 (+8).
// Data: "ST_IDLE_SUBTLE" 0x01276DBC, "ST_SLOW_WALK" 0x0125D14C, "ST_WALK" 0x0125A990, "ST_JOG" 0x0125D144, "ST_RUN"
//   0x0125A998, "ST_SPRINT" 0x0125D138, "DEFAULT" 0x01230554, 0.0f 0x0122DEDC.
// Layout: Ego_r.pdb CModeStandBase (engine/CModeStandBase.h): MaxRunSpeed +0x28, MaxJogSpeed +0x2c, MaxWalkSpeed +0x30,
//   counted-pointer animations +0x38..+0x64 (engine/CCountedPointer.h); the class is modelled locally because engine/CModeStandBase.h flattens the CCountedPointer members.
#include "fable_compiler.h"
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
class CAnimationEntry;
namespace NAnimationEntryBodge {
class CAnimEntryLite { public:                                // 0x10 bytes
    CAnimationEntry* Entry;                            // +0x00
    unsigned char _rest[0xc];
    CAnimEntryLite(CAnimationEntry* entry);            // 0x0073A9B0
};
}
using NAnimationEntryBodge::CAnimEntryLite;
class CAnimateThing { public: void SetAsFinished(); };      // 0x0070BE80
struct CTCAnimate { CAnimateThing* GetAnimateThing(); };   // 0x0070B460
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingCreatureBase { public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                           // +0x10 (2 player, 1 creature; descriptive)
    unsigned char _pad11[0x0f];
    unsigned long UsingComponents[9];                  // +0x20
    CInterfaceMap Components;                          // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
    bool IsPerformingAnyAction() const;                      // 0x006622A0
};
class CThingPlayerCreature : public CThingCreatureBase { public: void InitialiseMovementSpeeds(); };   // 0x006A9A00
struct CAnimBank;
class CWorld {
public:
    unsigned long* GetWorldSeed() const;   // 0x0049D810 (Ego_r.pdb ?GetWorldSeed@CWorld@@QBEPAKXZ)
private: public:
    unsigned char _pad[0x3c];
    long Value3c;                                      // +0x3c
};
CCountedPointer<CAnimEntryLite> __fastcall LookupStandAnim(const CCharString& name, CThingCreatureBase* creature,
                                                          long animSet, CAnimBank* bank);   // 0x00861E60
float __fastcall GetAnimSpeed(CAnimationEntry* entry, CThingCreatureBase* creature, long world3c);   // 0x00861950

struct CModeBase {
    virtual void v00(); virtual void v01();
    virtual void StartAnim(CAnimateThing* thing);      // slot 2 (+0x08)
    virtual void v03();
    virtual long GetAnimSet();                         // slot 4 (+0x10)
    virtual void OnActivate();                         // slot 5, 0x0073AAC0
    CThingCreatureBase* ParentThing;                   // +0x04
    unsigned char _body[0x14];
    CWorld* GetWorld();                                // 0x0073A680
};
struct CModeStandBase : CModeBase {
    long IdleCounter;                                  // +0x1c
    long FramesToKeepUpdatingMovement;                 // +0x20
    bool ForceAddNewAnims;                             // +0x24
    float MaxRunSpeed;                                 // +0x28
    float MaxJogSpeed;                                 // +0x2c
    float MaxWalkSpeed;                                // +0x30
    CCharString PoseAnimName;                          // +0x34
    CCountedPointer<CAnimEntryLite> PStandAnimation;   // +0x38
    CCountedPointer<CAnimEntryLite> PSlowWalkAnimation;   // +0x40
    CCountedPointer<CAnimEntryLite> PWalkAnimation;    // +0x48
    CCountedPointer<CAnimEntryLite> PJogAnimation;     // +0x50
    CCountedPointer<CAnimEntryLite> PRunAnimation;     // +0x58
    CCountedPointer<CAnimEntryLite> PSprintAnimation;  // +0x60
    virtual void OnActivate();
};

void CModeStandBase::OnActivate()
{
    CModeBase::OnActivate();
    CThingCreatureBase* creature = ParentThing;
    PStandAnimation = LookupStandAnim("ST_IDLE_SUBTLE", creature, GetAnimSet(), ((CAnimBank*)((CWorld*)(GetWorld()))->GetWorldSeed()));
    PSlowWalkAnimation = LookupStandAnim("ST_SLOW_WALK", creature, GetAnimSet(), ((CAnimBank*)((CWorld*)(GetWorld()))->GetWorldSeed()));
    PWalkAnimation = LookupStandAnim("ST_WALK", creature, GetAnimSet(), ((CAnimBank*)((CWorld*)(GetWorld()))->GetWorldSeed()));
    PJogAnimation = LookupStandAnim("ST_JOG", creature, GetAnimSet(), ((CAnimBank*)((CWorld*)(GetWorld()))->GetWorldSeed()));
    PRunAnimation = LookupStandAnim("ST_RUN", creature, GetAnimSet(), ((CAnimBank*)((CWorld*)(GetWorld()))->GetWorldSeed()));
    PSprintAnimation = LookupStandAnim("ST_SPRINT", creature, GetAnimSet(), ((CAnimBank*)((CWorld*)(GetWorld()))->GetWorldSeed()));
    if (!PStandAnimation.Data)
        PStandAnimation.Reset(new CAnimEntryLite(creature->GetAnimFromName("DEFAULT", 0)));
    if (!PWalkAnimation.Data)
        PWalkAnimation = PStandAnimation;
    if (!PRunAnimation.Data)
        PRunAnimation = PStandAnimation;
    if (!PSlowWalkAnimation.Data)
        PSlowWalkAnimation = PStandAnimation;
    if (!PSprintAnimation.Data)
        PSprintAnimation = PRunAnimation;
    if (creature->ThingType == 2) {
        CThingPlayerCreature* player = (CThingPlayerCreature*)ParentThing;
        player->InitialiseMovementSpeeds();
        CTCAnimate* animate;
        player->GetInterface(&animate, 0x5a);
        animate->GetAnimateThing()->SetAsFinished();
    } else if (creature->ThingType == 1) {
        long world3c = GetWorld()->Value3c;
        MaxWalkSpeed = !PWalkAnimation.Data ? 0.0f : GetAnimSpeed(PWalkAnimation.Data->Entry, creature, world3c);
        MaxJogSpeed = !PJogAnimation.Data ? 0.0f : GetAnimSpeed(PJogAnimation.Data->Entry, creature, world3c);
        MaxRunSpeed = !PRunAnimation.Data ? 0.0f : GetAnimSpeed(PRunAnimation.Data->Entry, creature, world3c);
    }
    CTCAnimate* animate;
    ParentThing->GetInterface(&animate, 0x5a);
    if (!creature->IsPerformingAnyAction()) {
        ForceAddNewAnims = true;
        StartAnim(animate->GetAnimateThing());
        ForceAddNewAnims = false;
    }
}