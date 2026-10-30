// NScript::CQ_CinemaTestScript::WatchForLeaving() -- watch thread: frame by frame while the arena level stays loaded and the hero alive, and the mission is neither failed nor succeeded; then mark the mission failed unless it succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F02030, length 193 bytes, plain `ret`.
// Identity: catalog CQ_CinemaTestScript::WatchForLeaving, consistent with its landed neighbours EndMission 0x00F02100 and
// PhaseOne 0x00F02170 (same 0x48-byte quest base, same "ArenaHallOfHeroes" level name).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 11 IsLevelLoaded(const CCharString&) (+0x2C), 70 GetHero (+0x118, CScriptThing&); CScriptThing slot 75 IsAlive
// (+0x12C); CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. String: "ArenaHallOfHeroes" @ 0x012C7820.
// Layout: retail quest base 0x48 bytes (CScriptBase 0x40, GSI +0x40, owner +0x44); MissionSucceeded +0x48,
// MissionFailed +0x49 (Ego_r.pdb +0x5C/+0x5D less the 0x14 debug-container shift).
// Shape: a top-tested wait loop whose `&&` condition starts with the level name built by an implicit conversion; the
// temporary is conditional for VC, so its destruction flag lives in a frame word (`or eax,1` load/store) and the
// condition rides in bl across the destructor; the loop body re-tests termination at the bottom (rotated loop).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQ_CinemaTestScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Owner;                        // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    void WatchForLeaving();
};

void CQ_CinemaTestScript::WatchForLeaving()
{
    while (GSI->IsLevelLoaded("ArenaHallOfHeroes") && GSI->GetHero().IsAlive() && !MissionFailed && !MissionSucceeded) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}
}