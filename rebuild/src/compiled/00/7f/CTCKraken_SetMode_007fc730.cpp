// CTCKraken::SetMode(EKrakenMode mode) -- store the kraken's mode and run its entry action: (re)create and raise the tentacles and end the head effects, kill the beam and the tentacles, or start the beam effect
// Leaf: ?SetMode@CTCKraken@@AAEXW4EKrakenMode@1@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FC730, 104 bytes, thiscall, `ret 4`, 5-entry jump table at 0x007FC784.
// Identity (proven): Ego_r.pdb ?SetMode@CTCKraken@@AAEXW4EKrakenMode@1@@Z, whose donor neighbours (CreateTentacles,
//   InitTentaclesOnly, InitTentaclesAndHead, SetMode, StartLowAttackBeam, StartHighAttackBeam, EndAttackBeam, SetPhase,
//   OnInitialActivate, FrameUpdate) sit in the same order in retail (0x007FC210 .. 0x007FC9D0; OnInitialActivate / FrameUpdate
//   are CTCKraken vtable 0x01241B94 slots 6 / 9); the mode store at +0x64 is Ego_r CTCKraken::Mode (+0x74) shifted down by the
//   two 4-byte STLport lists (ExplosionList, Tentacles: 12 bytes each in Ego_r); StartLowAttackBeam 0x007FC7A0 stores 4 and
//   StartHighAttackBeam 5 into the same word before StartBeamEffect, EndAttackBeam 0x007FC800 stores 3 before KillBeamEffect +
//   KillTentacles -- the same actions as the switch arms. Callers 0x00923C42 / 0x00924E59 (2).
// Callees (Ego_r donor order = retail order): CreateTentacles 0x007FC210, RaiseSunkenTentacles 0x007FB610, EndHeadEffects
//   0x007FB150, KillBeamEffect 0x007FB870, KillTentacles 0x007FB1C0, StartBeamEffect 0x007FB6A0.
// Shape: modes 1 and 2 run InitTentaclesOnly / InitTentaclesAndHead (0x007FC6F0 / 0x007FC710, byte-identical bodies defined just
//   above in the TU), which VC7.1 inlines and merges into one arm.
// Layout: CTCBase (engine/CTCBase.h, 0xc) + the Ego_r CTCKraken members at STLport sizes.
#include "engine/CTCBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
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
class CKrakenDef;
class CTCBoss;
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
    void SetMode(EKrakenMode mode);
    void StartBeamEffect();                              // 0x007FB6A0
    void KillBeamEffect();                               // 0x007FB870
    void KillTentacles();                                // 0x007FB1C0
public:
    void RaiseSunkenTentacles();                         // 0x007FB610
    void EndHeadEffects();                               // 0x007FB150
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
    case 1:
        InitTentaclesOnly();
        break;
    case 2:
        InitTentaclesAndHead();
        break;
    case 3:
        KillBeamEffect();
        KillTentacles();
        break;
    case 4:
    case 5:
        StartBeamEffect();
        break;
    }
}