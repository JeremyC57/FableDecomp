// CQ_AmbushTradersScript::PhaseOne() -- wait one script frame at a time until the ambush region is loaded (stopping early when the script thread is terminating)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0B780, length 135 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), and through the quest's game script interface (+0x40):
// NewScriptFrame (slot 7, +0x1c), IsRegionLoaded (slot 12, +0x30). String: 0x012C7778 (the region name; masked).
// Shape: as CQ_CinemaTestScript::PhaseOne (0x00F02170): an inline test helper with a NAMED string local and
// `IsRegionLoaded(region) ? false : true` (the 8-bit `neg/sbb/inc` kept in bl across the destructor); the closing
// `if (IsActiveThreadTerminating()) return;` has nothing after it, so only the call survives.
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
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30)
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
class CQ_AmbushTradersScript : public CQuestScriptBase {
public:
    bool IsAmbushRegionMissing()                            // the region test (inline)
    {
        CCharString region("DesertedFarm", -1);
        return GSI->IsRegionLoaded(region) ? false : true;
    }
    void PhaseOne();
};
} // namespace NScript
using NScript::CQ_AmbushTradersScript;

void NScript::CQ_AmbushTradersScript::PhaseOne()
{
    while (IsAmbushRegionMissing()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}