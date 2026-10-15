// CQ_AmbushTradersScript::PhaseOne() -- wait one script frame at a time until the ambush region is loaded (stopping early when the script thread is terminating)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0B780, length 135 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), and through the quest's game script interface (+0x40):
// NewScriptFrame (slot 7, +0x1c), IsRegionLoaded (slot 12, +0x30). String: 0x012C7778 (the region name; masked).
// Shape: as CQ_CinemaTestScript::PhaseOne (0x00F02170): an inline test helper with a NAMED string local and
// `IsRegionLoaded(region) ? false : true` (the 8-bit `neg/sbb/inc` kept in bl across the destructor); the closing
// `if (IsActiveThreadTerminating()) return;` has nothing after it, so only the call survives.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30)
};

struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

struct CQ_AmbushTradersScript : CQuestScriptBase {
    bool IsAmbushRegionMissing()                            // the region test (inline)
    {
        CCharString region("DesertedFarm", -1);
        return GSI->IsRegionLoaded(region) ? false : true;
    }
    void PhaseOne();
};

void CQ_AmbushTradersScript::PhaseOne()
{
    while (IsAmbushRegionMissing()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}