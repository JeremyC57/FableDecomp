// CTCKraken::StartHighAttackBeam() -- arm the high beam attack: the delay before the beam is the def's DelayBeforeHighBeam seconds in frames, then enter mode 5 (SetMode starts the beam effect)
// Leaf: ?StartHighAttackBeam@CTCKraken@@QAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FC7D0, 38 bytes, thiscall, tail jump to StartBeamEffect.
// Identity (proven): Ego_r.pdb donor order SetMode, StartLowAttackBeam, StartHighAttackBeam, EndAttackBeam, SetPhase is the retail
//   order 0x007FC730 (landed SetMode), 0x007FC7A0, 0x007FC7D0, 0x007FC800, 0x007FC820 (landed SetPhase); this body reads
//   CKrakenDef +0x50 (Ego_r DelayBeforeHighBeam; the twin 0x007FC7A0 reads +0x4C DelayBeforeLowBeam) and enters mode 5 (the twin 4).
//   The catalog label is right for this copy. Caller 0x009242B0 (the kraken boss AI).
// Callees: _ftol2 (the float-to-long conversion), CTCKraken::StartBeamEffect() 0x007FB6A0 (Ego_r ?StartBeamEffect@CTCKraken@@AAEXXZ),
//   reached through SetMode(EKrakenMode) (Ego_r ?SetMode@CTCKraken@@AAEXW4EKrakenMode@1@@Z, landed 0x007FC730), defined above in the
//   same TU as retail's TU holds it (0x007FC730, after InitTentaclesOnly / InitTentaclesAndHead): VC7.1 inlines it with the constant
//   mode and folds the switch to the mode store + StartBeamEffect (case 4/5). EndAttackBeam 0x007FC800 is SetMode(3) folded the same
//   way (mode store, KillBeamEffect, KillTentacles). A direct "Mode = 4; StartBeamEffect();" body compiles to the same bytes; the
//   SetMode form is kept because SetMode's case 4/5 is exactly the beam-start entry action.
// Globals: CMainGameComponent::ConstantFPS 0x01375550 (?ConstantFPS@CMainGameComponent@@1JA, protected static) through the inline
//   PDB accessor GetConstantFPS() (?GetConstantFPS@CMainGameComponent@@SIJXZ).
// Layout: CTCBase (engine/CTCBase.h) + the Ego_r CTCKraken members at STLport sizes, as the landed SetMode 0x007FC730:
//   PKrakenDef +0x0C, DelayBeforeBeam +0x3C, Mode +0x64; CKrakenDef (engine/CDefClassBase.h base) DelayBeforeLowBeam +0x4C,
//   DelayBeforeHighBeam +0x50 (Ego_r offsets: no container before them). Enumerator names of EKrakenMode are in no name set.
#include "engine/CTCBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
namespace std {
template <class _Tp> class allocator { public: };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
};
} // namespace std
class CThing;
class CTCBoss;
class CMainGameComponent {
protected:
    static long ConstantFPS;                                     // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CKrakenDef : public CDefClassBase {
public:
    unsigned char _pad25[0x4c - sizeof(CDefClassBase)];
    float         DelayBeforeLowBeam;                            // +0x4C
    float         DelayBeforeHighBeam;                           // +0x50
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCKraken : public CTCBase {
public:
    enum EKrakenMode { };
    enum EKrakenBossPhase { };
private:
    void CreateTentacles();                              // 0x007FC210
    void InitTentaclesOnly();                            // 0x007FC6F0
    void InitTentaclesAndHead();                         // 0x007FC710
    void SetMode(EKrakenMode mode);                      // 0x007FC730
    void StartBeamEffect();                              // 0x007FB6A0
    void KillBeamEffect();                               // 0x007FB870
    void KillTentacles();                                // 0x007FB1C0
public:
    void RaiseSunkenTentacles();                         // 0x007FB610
    void EndHeadEffects();                               // 0x007FB150
    void StartHighAttackBeam();
    CDefPointer<const CKrakenDef>           PKrakenDef;          // +0x0c
    CIntelligentPointer<CThing>             PChamber;            // +0x10
    CIntelligentPointer<CThing>             PCentreMarker;       // +0x18
    CIntelligentPointer<CThing>             PTarget;             // +0x20
    CIntelligentPointer<CThing>             PHeadEffect;         // +0x28
    std::list<CIntelligentPointer<CThing> > ExplosionList;       // +0x30
    CIntelligentPointer<CThing>             PBeamWeaponEffect;   // +0x34
    long                                    DelayBeforeBeam;     // +0x3c
    CTCBoss*                                PTCBoss;             // +0x40
    std::list<CIntelligentPointer<CThing> > Tentacles;           // +0x44
    std::_List_node_base*                   AttackingTentacleIterator;   // +0x48 (list iterator)
    long                                    TentacleStrikeCounter;       // +0x4c
    bool                                    TentacleReadyToStrike;       // +0x50
    bool                                    HeadTrackTarget;             // +0x51
    bool                                    HeadHasBeenHit;              // +0x52
    long                                    LastStrikeTick;              // +0x54
    long                                    KillBeamCounter;             // +0x58
    bool                                    ReturnHeadToWater;           // +0x5c
    EKrakenBossPhase                        Phase;                       // +0x60
    EKrakenMode                             Mode;                        // +0x64
};
void CTCKraken::InitTentaclesOnly()
{
    if (Tentacles.empty())
        CreateTentacles();
    RaiseSunkenTentacles();
    EndHeadEffects();
}
void CTCKraken::InitTentaclesAndHead()
{
    if (Tentacles.empty())
        CreateTentacles();
    RaiseSunkenTentacles();
    EndHeadEffects();
}
void CTCKraken::SetMode(EKrakenMode mode)
{
    Mode = mode;
    switch (mode) {
    case 1: InitTentaclesOnly(); break;
    case 2: InitTentaclesAndHead(); break;
    case 3: KillBeamEffect(); KillTentacles(); break;
    case 4:
    case 5: StartBeamEffect(); break;
    }
}
void CTCKraken::StartHighAttackBeam()
{
    DelayBeforeBeam = (long)(PKrakenDef->DelayBeforeHighBeam * CMainGameComponent::GetConstantFPS());
    SetMode((EKrakenMode)5);
}