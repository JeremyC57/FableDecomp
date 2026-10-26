// CMK_OFI_GWLL_WHIS2::Main() -- when the parent raid script (+0x14) has not run its cutscene (+0x88), holds no raid thing (+0x78) and this script is not aborted (0x00F35B30), mark the cutscene done and run the parent's multiplier cutscene (0x00DD1AF0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD1EB0, length 55 bytes.
// Callees: the abort test @ 0x00F35B30 (on `this`, bool; the manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated), CQ_OrchardFarmRaidScript::DoMultiplierCutscene @ 0x00DD1AF0 (a tail jump). Shape: the parent re-read after the call, `ret`.
namespace NScript { class CPredicateBase; }
struct CQ_OrchardFarmRaidScript {                  // the parent quest script (+0x14)
    unsigned char _pad[0x78];
    void*         m_pRaidThing;                    // +0x78
    unsigned char _pad7c[0xc];
    bool          m_cutsceneDone;                  // +0x88
    void DoMultiplierCutscene();                   // 0x00DD1AF0
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CMK_OFI_GWLL_WHIS2 : public CEntityScriptBase {
    void*                     __vftable;           // +0x00
    unsigned char             _pad04[0x10];
    CQ_OrchardFarmRaidScript* ParentClass;         // +0x14
    void Main();
};

void CMK_OFI_GWLL_WHIS2::Main()
{
    if (!ParentClass->m_cutsceneDone && !ParentClass->m_pRaidThing && !IsActiveThreadTerminating()) {
        ParentClass->m_cutsceneDone = true;
        ParentClass->DoMultiplierCutscene();
    }
}