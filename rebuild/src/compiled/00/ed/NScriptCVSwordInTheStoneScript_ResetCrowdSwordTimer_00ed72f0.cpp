// CV_SwordInTheStoneScript::ResetCrowdSwordTimer() -- restart the crowd timer (+0x7c) with a random 10..15 second period through the game script interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED72F0, length 40 bytes.
// Callees: rand @ 0x00BFEB16 (CRT), CGameScriptInterface slot 89 (+0x164: (timer, seconds)) through the
// interface global @ 0x0143E8F8 (RegisterTimer is its slot 87 in the landed naming).
// Shape: `rand() % 6` as `cdq ; idiv` by an immediate in ecx into a local, `+ 10` on the remainder, the global
// and its vtable fetched AFTER the rand call, the timer word pushed last.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" int __cdecl rand(void);                 // 0x00BFEB16

class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void ResetTimer(long timer, long seconds);   // slot 89 (+0x164)
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

namespace NScript {
class CV_SwordInTheStoneScript {
public:
    virtual void v00();
    unsigned char _pad04[0x78];                    // +0x04..+0x7b
    long          m_crowdSwordTimer;               // +0x7c
    void ResetCrowdSwordTimer();
};
} // namespace NScript
using NScript::CV_SwordInTheStoneScript;

void NScript::CV_SwordInTheStoneScript::ResetCrowdSwordTimer()
{
    long seconds = rand() % 6 + 10;               // a local: the lane otherwise hoists the global's vtable fetch above rand()
    g_pGameScriptInterface->ResetTimer(m_crowdSwordTimer, seconds);
}