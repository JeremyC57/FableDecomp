// CQ_CinemaTestScript::PhaseOne() -- wait one script frame at a time until the arena level is loaded, then (unless the script thread is terminating) run the arena introduction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F02170, length 146 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CQ_CinemaTestScript::ArenaIntroduction @ 0x00F02210,
// and through the quest's game script interface (+0x40): NewScriptFrame (slot 7, +0x1c), IsLevelLoaded (slot 11, +0x2c).
// String: 0x012C7820 (the level name; masked as a relocation).
// Shape: the loop test is an inline helper with a NAMED string local (a temporary inside the call lets VC fetch the
// interface's vtable before the string is built); `IsLevelLoaded(level) ? false : true` keeps the 8-bit
// `neg/sbb/inc` result in bl across the string destructor (a plain `!` widens through eax); VC rotates the loop so
// the test appears twice.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual bool IsLevelLoaded(const CCharString& level) const;   // slot 11 (+0x2c)
};

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

namespace NScript {
class CQ_CinemaTestScript : public CQuestScriptBase {
public:
    bool IsArenaLevelMissing()                              // the level test (inline)
    {
        CCharString level("ArenaHallOfHeroes", -1);
        return GSI->IsLevelLoaded(level) ? false : true;
    }
    void PhaseOne();
    void ArenaIntroduction();                               // 0x00F02210
};
} // namespace NScript
using NScript::CQ_CinemaTestScript;

void NScript::CQ_CinemaTestScript::PhaseOne()
{
    while (IsArenaLevelMissing()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ArenaIntroduction();
}