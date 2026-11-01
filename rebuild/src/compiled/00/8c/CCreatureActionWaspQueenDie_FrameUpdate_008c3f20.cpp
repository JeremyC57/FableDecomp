// CCreatureAction_WaspQueenDie::FrameUpdate() -- fade the dying wasp queen: alpha = 255 - 51 x LoopNum, less 51 x the fraction of the action run so far, set on the creature's graphic appearance; then the combat-action frame update
// Leaf: ?FrameUpdate@CCreatureAction_WaspQueenDie@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C3F20, length 111 bytes, tail jump to CCombatActionBase::FrameUpdate.
// Identity (proven): the only reference is vtable 0x01280D84 (RTTI .?AVCCreatureAction_WaspQueenDie@@) slot 56, the action family's
//   FrameUpdate slot (CCombatActionBase::FrameUpdate 0x00859D80 is slot 56 of 0x01259BD4); Ego_r.pdb / FableWin.pdb
//   ?FrameUpdate@CCreatureAction_WaspQueenDie@@UAEXXZ; ends in the base FrameUpdate. RTTI chain: CCreatureAction_WaspQueenDie :
//   CCombatActionBase : CCreatureActionBase : CBaseObject : CBaseClass.
// Callees: CCreatureActionBase::GetTotalTimeWorldFrames() const @ 0x006925B0 and GetFramesRunning() const @ 0x006925C0 (spellings
//   as the landed 0x0087CA30 model), CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (PCreature +0x08), _ftol2 @ 0x00BFEA70,
//   NGraphicAppearance::CTCGraphicAppearance::SetAlpha(unsigned char) @ 0x004BFAC0 (Ego_r.pdb QAEXE@Z), CCombatActionBase::FrameUpdate
//   @ 0x00859D80. Floats 51.0f @ 0x01253D40, 255.0f @ 0x01230014.
// Layout: CCreatureAction_WaspQueenDie LoopNum +0x114 (Ego_r.pdb +300 less the 0x18 of the base's three STLport containers);
//   CThing PTCGraphicAppearance +0x64 through the inline CThing::GetTCGraphicAppearance() (Ego_r.pdb QAE), as the landed
//   CCreatureAction_BalverineCorneredJumpIntoSky::FrameUpdate 0x008CC7A0.
// Shape: the total time is fetched first into a named local (the `fidiv` memory operand); the loop's start alpha is its own
//   named local and the fade a second one: that is what puts the frames temporary, the total and the alpha in retail's frame
//   words (+0 / +4 / +8). A single `alpha` local with `-=` (or one expression, a block-scope fraction, a float total, a named
//   creature) shuffles the three words or adds a spill (probe/008c3f20.log, 13 probes).
#include "engine/CBaseClass.h"
class CBaseObject;
class CBaseObjectPointer;
#pragma pack(push, 1)   // C3DVector has alignment 1 (Ego_r CCreatureAction_BlockRespond::HitPos at +317)
class C3DVector {
public:
    float X, Y, Z;
};
#pragma pack(pop)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class vector {               // retail STLport vector (12 bytes; only named here)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}   // namespace std
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAlpha(unsigned char alpha);                          // 0x004BFAC0
};
}
class CThing { public:                                           // partial view, retail offsets
    virtual ~CThing();
    unsigned char                               _pad_0x04[0x64 - 0x04];
    NGraphicAppearance::CTCGraphicAppearance*   PTCGraphicAppearance;   // +0x64
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; }
};
class CThingCreatureBase : public CThing { };
class CWorldMap;
class CGameDefinitionManager;

class CBaseIntelligentPointer {                                  // vptr + PData (8 bytes)
public:
    CBaseIntelligentPointer();                                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
    CBaseObject* GetPItem() const;                               // 0x00A01B50
    CBaseObjectPointer* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};

class CBaseObject : public CBaseClass { };
class CCreatureActionBase : public CBaseObject {                 // retail sizeof 0xA8 (Ego_r 0xB4: three STL containers)
public:
    unsigned char                           _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;           // +0x08
    long             TotalTime;                                  // +0x10
    long             ActionTime;                                 // +0x14
    long             AnimDelayTime;                              // +0x18
    long             FramesRunning;                              // +0x1C
    long             InterruptionGroupID;                        // +0x20
    long             InterruptionPriority;                       // +0x24
    long             ActionID;                                   // +0x28
    CIntelligentPointer<CThing> PItemRemoved;                    // +0x2C
    unsigned char    _pad34[0xA8 - 0x34];
    long GetTotalTimeWorldFrames() const;                        // 0x006925B0
    long GetFramesRunning() const;                               // 0x006925C0
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const; // 0x00692910
    CWorldMap& GetWorldMap();                                    // 0x00692C10 (folded with the const overload)
};
class CCombatActionBase : public CCreatureActionBase {           // retail sizeof 0x114 (Ego_r 0x12C)
public:
    virtual void FrameUpdate();                                  // slot 56, 0x00859D80
    CIntelligentPointer<CThing>              PTarget;            // +0xA8
    CIntelligentPointer<const CThing>        PCameraPoint;       // +0xB0
    unsigned char _padB8[0x114 - 0xB8];
};
class CCreatureAction_WaspQueenDie : public CCombatActionBase {
public:
    virtual void FrameUpdate();
    long LoopNum;                                                // +0x114
};

void CCreatureAction_WaspQueenDie::FrameUpdate()
{
    long totalTime = GetTotalTimeWorldFrames();
    float startAlpha = 255.0f - LoopNum * 51.0f;
    float alpha = startAlpha - (float)GetFramesRunning() / totalTime * 51.0f;
    PCreature->GetTCGraphicAppearance()->SetAlpha((unsigned char)(long)alpha);
    CCombatActionBase::FrameUpdate();
}