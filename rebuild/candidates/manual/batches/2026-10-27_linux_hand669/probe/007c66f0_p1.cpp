// CTCTurncoatSpell::~CTCTurncoatSpell() -- turncoat-spell component dtor: the members in reverse order (the three turncoating effects, the removed item, the target, the def pointer, the will-button helper timer) and ~CTCBase
// Leaf: ??1CTCTurncoatSpell
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
inline void _Destroy(float*, float*) { }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CThingCreatureBase;
class CSpecialAbilitiesTurncoatSpellDef;
class CBaseObjectPointer;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                                          // vtable 0x0129C95C, 8 bytes
public:
    CBaseIntelligentPointer();                                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                                  // 0x00A01C10
private:
    CBaseObjectPointer* PData;                                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
#pragma pack(pop)

enum EHeroAbility { };
enum EButtonHeldState { };
class CWillButtonHelperTimer {                              // retail 0x34 bytes (Ego_r 0x38 less the STLport vector)
public:
    CWillButtonHelperTimer(EHeroAbility ability, CThing* creature);      // 0x008EDFF0
    long                         ButtonHeldDownTimer;                    // +0x00
    long                         WillAccumulatedLevel;                   // +0x04
    CIntelligentPointer<CThing>  PCreature;                              // +0x08
    EHeroAbility                 MyAbility;                              // +0x10
    EButtonHeldState             CurrentState;                           // +0x14
    std::vector<float>           WillPowerUpAccumulatedTimings;          // +0x18
    float                        MaxNumberLevels;                        // +0x24
    float                        CurMaxLevels;                           // +0x28
    bool                         WillIncrementedFlag;                    // +0x2C
    bool                         UpdateAccLevel;                         // +0x2D
    bool                         IsAICasting;                            // +0x2E
    bool                         IsAIHoldingDown;                        // +0x2F
    long                         LevelToCast;                            // +0x30
};

class CTCBase : public CBaseClassNonCopyable {              // retail 0x0C bytes
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCTurncoatSpell : public CTCBase {                   // vtable 0x0126E5C4
public:
    virtual ~CTCTurncoatSpell();
    CWillButtonHelperTimer                      Timer;                              // +0x0C
    CDefPointer<const CSpecialAbilitiesTurncoatSpellDef> PDef;                      // +0x40
    CIntelligentPointer<CThingCreatureBase>     PTarget;                            // +0x44
    long                                        LevelOfSpell;                       // +0x4C
    unsigned long                               LoopingSound;                       // +0x50
    CIntelligentPointer<CThing>                 PItemRemoved;                       // +0x54
    float                                       PerFrameStaminaModify;              // +0x5C
    float                                       AccModifyStamina;                   // +0x60
    long                                        StaminaModify;                      // +0x64
    float                                       PerFrameResistanceModify;           // +0x68
    float                                       AccModifyResistance;                // +0x6C
    long                                        ResistanceModify;                   // +0x70
    CIntelligentPointer<CThing>                 TurncoatingEffectOnMe;              // +0x74
    CIntelligentPointer<CThing>                 TurncoatingEffectOnTarget;          // +0x7C
    CIntelligentPointer<CThing>                 TurncoatingEffectOnMeOutOfRange;    // +0x84
    bool                                        InRange;                            // +0x8C
    long                                        CurXPFrameCounter;                  // +0x90
    long                                        XPFrameCounter;                     // +0x94
};

CTCTurncoatSpell::~CTCTurncoatSpell()
{
}
