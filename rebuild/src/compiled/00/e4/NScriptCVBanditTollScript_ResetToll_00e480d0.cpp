// CV_BanditTollScript::ResetToll() -- watcher thread: whenever the hero leaves the toll region and walks into Greatwood after paying or angering the bandits, forget both, so the toll applies again
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E480D0, length 522 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; GameCalls slots 7 NewScriptFrame (+0x1c) and 12 IsRegionLoaded (+0x30) through the interface at +0x40.
// Layout: Ego_r.pdb CV_BanditTollScript has BanditsAngered +0x5c and TollPaid +0x5d; retail CScriptBase is 0x14 smaller
//   than the PDB view (0x40, no debug containers), so they sit at +0x48 / +0x49.
// Shape: the usual script WAIT loops (`while (!cond) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`).
//   The `(TollPaid || BanditsAngered) && (Greatwood || GreatwoodLake)` test builds its two strings conditionally, so the
//   flag word for their destruction lives in the frame; the two clears reuse the zero in al. The second wait is
//   written `!= false`: a plain bool condition keeps `mov bl,al` where retail materialises `test al,al ; setne bl`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing;
struct GameCalls {                                       // CGameScriptInterface, retail vtable 0x01260F0C
 virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
 virtual void NewScriptFrame(void) const; //slot7 0x006e7410
 virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
 virtual bool IsRegionLoaded(CCharString const &) const; //slot12 0x0088e300
};
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};
namespace NScript {
class CV_BanditTollScript : public CScriptBase {
public:
    GameCalls* Game;                                     // +0x40
    void*      Owner;                                    // +0x44
    bool       BanditsAngered;                           // +0x48
    bool       TollPaid;                                 // +0x49
    void ResetToll();
};
} // namespace NScript
using NScript::CV_BanditTollScript;

void NScript::CV_BanditTollScript::ResetToll()
{
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsRegionLoaded("GreatwoodBanditToll")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if ((TollPaid || BanditsAngered) && (Game->IsRegionLoaded("Greatwood") || Game->IsRegionLoaded("GreatwoodLake"))) {
                if (IsActiveThreadTerminating()) return;
                TollPaid = false;
                BanditsAngered = false;
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (Game->IsRegionLoaded("GreatwoodBanditToll") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
    }
}