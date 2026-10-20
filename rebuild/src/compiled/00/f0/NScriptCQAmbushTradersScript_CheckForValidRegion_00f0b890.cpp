// NScript::CQ_AmbushTradersScript::CheckForValidRegion() -- true when the hero is in one of the three regions the ambush can run in (BowerstoneJail, DesertedFarm or GibbetWoods is loaded)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0B890, length 197 bytes, plain `ret`, bool in al.
// Identity: catalog CQ_AmbushTradersScript::CheckForValidRegion, next to the landed PhaseOne 0x00F0B780 of the class.
// Callees: CGameScriptInterface slot 12 IsRegionLoaded(const CCharString&) (+0x30) through GSI (+0x40);
// CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. Strings: "BowerstoneJail" @ 0x012C7788,
// "DesertedFarm" @ 0x012C7778, "GibbetWoods" @ 0x012C776C.
// Shape: one `||` chain over three implicit-conversion temporaries: VC builds them in three frame words and tracks
// which ones exist with a constant-folded destruction mask in ebx (`mov ebx,1/3/7`, cleared bit by bit as each
// destructor runs, last-built first); the chain's result is parked in a frame byte across the destructors.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};

struct CQ_AmbushTradersScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    bool CheckForValidRegion();
};

bool CQ_AmbushTradersScript::CheckForValidRegion()
{
    return GSI->IsRegionLoaded("BowerstoneJail") || GSI->IsRegionLoaded("DesertedFarm")
        || GSI->IsRegionLoaded("GibbetWoods");
}
}