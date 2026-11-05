// CAIStateGroup_GoToPub::PlayAnims(const CCharString&, const CCharString&, const CCharString&, long) -- play the into animation, then the loop animation up to loopCount times and the out-of animation, stopping early (true) once the table's tavern game is being played and failing (false) when an animation does not complete
// Leaf: ?PlayAnims@CAIStateGroup_GoToPub@@QAE_NABVCCharString@@00J@Z
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00948D40, length 432 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTable +0x24), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xcf = bit 15 of +0x38), CCreatureAction_PlayAnimation ctor @ 0x008424F0, AIAction_PerformActionAndWait
// @ 0x0097DB70, AIAction_PerformAction @ 0x0097D8B0 (the out-of animation), the action base dtor @ 0x00693EF0.
// Shape: a zero-preloaded out-param GetInterface whose lookup key shares the out word's slot (the same-TU LowerBound stand-in keeps
// the key from escaping); the creature through an inline brain getter; `failed` in bl and the final `sete al` from an explicit
// `if (failed) return false; return true;` (`return !failed` adds an int-width `xor eax,eax`); ebp is pushed after the first exit.
// Layout: GoToPub from Ego_r.pdb (base 0x14, PTable +0x24); the 0xcf component is the tavern game (landed GameMaster
// 0x0094CA80 names it the game table): Ego_r.pdb CTCTavernGame IsBeingPlayed is +0x28 there, +0x20 in retail because the
// UIComponents vector at +0x0c is 12 bytes (STLport) and the following double re-aligns to +0x18.
// Layout (supersedes any local-model note above): engine/CTCTavernGame.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCTavernGame CTCTavernGameDonor
#include "engine/CTCTavernGame.h"
#undef CTCTavernGame
class CCharString;
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCTavernGame : public CTCTavernGameDonor {
 public:                          // interface 0xcf
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThingCreatureBase;
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0xb4 - 0xa8];
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x008424F0
    ~CCreatureAction_PlayAnimation();           // 0x00693EF0 (the base dtor)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    CThingCreatureBase* GetPCreature() const { return PCreature; }
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    long AIAction_PerformAction(CCreatureActionBase* action);                     protected: // 0x0097D8B0
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:  // 0x0097DB70
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointer<CThing> PPub;               // +0x14
    CIntelligentPointer<CThing> PBar;               // +0x1c
    CIntelligentPointer<CThing> PTable;             // +0x24
    bool PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, long loopCount);
};

bool CAIStateGroup_GoToPub::PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, long loopCount)
{
    CTCTavernGame* game = 0;
    PTable.GetPItem()->GetTC(TCI_TAVERN_GAME, &game);
    bool failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->GetPCreature(), intoAnim, CSoundPair(),
                                                                              false, true, false, 0, 0), false) != 2;
    if (failed)
        return false;
    while (loopCount > 0) {
        --loopCount;
        if (game && game->IsBeingPlayed)
            return true;
        failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->GetPCreature(), loopAnim, CSoundPair(),
                                                                             false, true, false, 0, 0), false) != 2;
        if (failed)
            return false;
    }
    if (game && game->IsBeingPlayed)
        return true;
    failed = AIAction_PerformAction(&CCreatureAction_PlayAnimation(PBrain->GetPCreature(), outAnim, CSoundPair(),
                                                                  false, true, false, 0, 0)) != 2;
    if (failed)
        return false;
    return true;
}