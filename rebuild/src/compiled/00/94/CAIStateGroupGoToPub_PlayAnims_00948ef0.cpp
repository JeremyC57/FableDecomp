// CAIStateGroup_GoToPub::PlayAnims(const CCharString&, const CCharString&, const CCharString&, float) -- play the into animation, repeat the loop animation for the given number of seconds and play the out-of animation, stopping early (true) once the table's tavern game is being played and failing (false) when an animation does not complete
// Leaf: ?PlayAnims@CAIStateGroup_GoToPub@@QAE_NABVCCharString@@00M@Z
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00948EF0, length 480 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTable +0x24), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xcf = bit 15 of +0x38), CCreatureAction_PlayAnimation ctor @ 0x008424F0, AIAction_PerformActionAndWait
// @ 0x0097DB70, AIAction_PerformAction @ 0x0097D8B0 (the out-of animation), the action base dtor @ 0x00693EF0,
// CWorld::GetWorldFrame @ 0x0049D870, CMainGameComponent::GetConstantFPS() @ 0x01375550.
// Shape: as the sibling 0x00948D40 (same-TU LowerBound stand-in, brain creature getter, explicit `if (failed) return false;`);
// the fps global is copied into a named long before the frame call. The loop test is retail's `frame > endFrame` (fild frame;
// fcomp end; test ah,41h; jne exit): the loop animation only repeats while the current frame is already PAST the end frame,
// so for a positive duration it never plays (retail behaviour, kept).
// Layout: GoToPub from Ego_r.pdb (base 0x14, PTable +0x24); the 0xcf component is the tavern game (see the landed sibling
// 0x00948D40): IsBeingPlayed +0x20 in retail (Ego_r.pdb +0x28 with a 16-byte vector).
// Layout (supersedes any local-model note above): engine/CTCTavernGame.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCTavernGame CTCTavernGameDonor
#define FABLE_RETAIL_CINTELLIGENTPOINTER
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
#include "engine/CIntelligentPointer.h"

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
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld { public:
    static long __fastcall GetFrame();     // 0x0049D870
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
    bool PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, float seconds);
};

bool CAIStateGroup_GoToPub::PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, float seconds)
{
    CTCTavernGame* game = 0;
    PTable.Get()->GetTC(TCI_TAVERN_GAME, &game);
    bool failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->GetPCreature(), intoAnim, CSoundPair(),
                                                                              false, true, false, 0, 0), false) != 2;
    if (failed)
        return false;
    long fps = CMainGameComponent::GetConstantFPS();
    float endFrame = (float)CWorld::GetFrame() + fps * seconds;
    while ((float)CWorld::GetFrame() > endFrame) {
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