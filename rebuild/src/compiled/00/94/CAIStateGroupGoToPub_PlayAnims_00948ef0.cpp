// CAIStateGroup_GoToPub::PlayAnims(const CCharString&, const CCharString&, const CCharString&, float) -- play the into animation, repeat the loop animation for the given number of seconds and play the out-of animation, stopping early (true) once the table's tavern game is being played and failing (false) when an animation does not complete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00948EF0, length 480 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTable +0x24), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xcf = bit 15 of +0x38), CCreatureAction_PlayAnimation ctor @ 0x008424F0, AIAction_PerformActionAndWait
// @ 0x0097DB70, AIAction_PerformAction @ 0x0097D8B0 (the out-of animation), the action base dtor @ 0x00693EF0,
// CWorld::GetWorldFrame @ 0x0049D870, g_framesPerSecond @ 0x01375550.
// Shape: as the sibling 0x00948D40 (same-TU LowerBound stand-in, brain creature getter, explicit `if (failed) return false;`);
// the fps global is copied into a named long before the frame call. The loop test is retail's `frame > endFrame` (fild frame;
// fcomp end; test ah,41h; jne exit): the loop animation only repeats while the current frame is already PAST the end frame,
// so for a positive duration it never plays (retail behaviour, kept).
// Layout: GoToPub from Ego_r.pdb (base 0x14, PTable +0x24); the 0xcf component is the tavern game (see the landed sibling
// 0x00948D40): IsBeingPlayed +0x20 in retail (Ego_r.pdb +0x28 with a 16-byte vector).
// Layout (supersedes any local-model note above): engine/CTCTavernGame.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCTavernGame CTCTavernGameDonor
#include "engine/CTCTavernGame.h"
#undef CTCTavernGame
class CCharString;
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (stand-in body below)
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (0x0040F020, STLport lower_bound; retail's LTCG saw its body): the key's address
// does not escape, so its frame slot is shared with the later fps/frame temporaries as retail.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCTavernGame : public CTCTavernGameDonor {
 public:                          // interface 0xcf
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
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
extern long g_framesPerSecond;                  // 0x01375550
class CWorld { public:
    static long __fastcall GetWorldFrame();     // 0x0049D870
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    CThingCreatureBase* GetPCreature() const { return PCreature; }
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    long AIAction_PerformAction(CCreatureActionBase* action);                     // 0x0097D8B0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);  // 0x0097DB70
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBaseIntelligentPointer PPub;               // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CBaseIntelligentPointer PTable;             // +0x24
    bool PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, float seconds);
};

bool CAIStateGroup_GoToPub::PlayAnims(const CCharString& intoAnim, const CCharString& loopAnim, const CCharString& outAnim, float seconds)
{
    CTCTavernGame* game = 0;
    PTable.GetPItem()->GetInterface(&game, 0xcf);
    bool failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(PBrain->GetPCreature(), intoAnim, CSoundPair(),
                                                                              false, true, false, 0, 0), false) != 2;
    if (failed)
        return false;
    long fps = g_framesPerSecond;
    float endFrame = (float)CWorld::GetWorldFrame() + fps * seconds;
    while ((float)CWorld::GetWorldFrame() > endFrame) {
        if (game && game->IsBeingPlayed)
            return true;
        failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(PBrain->GetPCreature(), loopAnim, CSoundPair(),
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