// CTCBerserk::AddDependentThing(CThing&) -- appends the thing to the berserk component's dependent-things list
// Leaf: ?AddDependentThing@CTCBerserk@@QAEX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6D90, 66 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: CTCBerserk::AddDependentThing(CThing&) -- probable; evidence: Ego_r.pdb CTCBerserk::DependentThings is the vector<CIntelligentPointer<CThing> > at +0x4C (no STL member before
//   it, so the retail offset is the same) and no other class with an Add*(CThing&) name holds such a vector there; the body
//   sits among the CCreatureAction_CastStrengthBerserk / ReleaseStrengthBerserk functions; no direct caller or reference
//   (probable rather than proven);
//   FableWin.pdb name set holds ?AddDependentThing@CTCBerserk@@QAEXAAVCThing@@@Z.
//   Family G039 (8 bodies: one temporary intelligent pointer handed to a list/vector member; the propagated label
//   "CCreatureActionBase::AddDependentThing" is wrong for all eight -- that member sits at +0x9C in retail).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90, std::vector<CIntelligentPointer<CThing> >::push_back 0x004AD520, CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10.
//   Vtable: CIntelligentPointer<CThing> 0x01238C6C.
// Shape: the temporary's inline (T*) constructor (base ctor, own vtable, SetPItem) and its implicit destructor (a direct
//   call of the base destructor, no vtable reset); no EH frame (the module is built without /GX).
// Model: local -- retail STLport lists are 4 bytes and vectors 12 (the generated engine headers carry Ego_r's 12/16-byte
//   Dinkumware containers and no member functions), so the class is laid out here at its retail offsets.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CThing : public CBaseObject { };
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void remove(const _Tp& __val);                           // out of line
    void* _M_node;                                           // _STLP_alloc_proxy<_List_node<_Tp>*, ...>
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    void push_back(const _Tp& __x);                          // out of line
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;                                  // _STLP_alloc_proxy<_Tp*, ...>
};
}
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                     // +0x04
    bool    ToBeRemoved;                                     // +0x08
    bool    IsInGlobalUpdateTCs;                             // +0x09
};
class CTCBerserk : public CTCBase {
public:
    void AddDependentThing(CThing& thing);
    long  Timer;                                             // +0x0c
    long  DarkenEnvTimer;                                    // +0x10
    float StrengthPercentage;                                // +0x14
    float MaxIncreaseInStrength;                             // +0x18
    float ReleasedAnimationTimer;                            // +0x1c
    long  AnimTimer;                                         // +0x20
    float BerserkDamageToMe;                                 // +0x24
    float BerserkDamageToEnemy;                              // +0x28
    float BerserkScale;                                      // +0x2c
    float RumbleLevel;                                       // +0x30
    float RumbleSmoothness;                                  // +0x34
    float RumbleAttack;                                      // +0x38
    float RumbleDecay;                                       // +0x3c
    unsigned long BerserkingLoopingSound;                    // +0x40
    long  SpellLevel;                                        // +0x44
    bool  JustReleased;                                      // +0x48
    bool  AbilityInitialised;                                // +0x49
    std::vector<CIntelligentPointer<CThing> > DependentThings;   // +0x4c
};

void CTCBerserk::AddDependentThing(CThing& thing)
{
    DependentThings.push_back(CIntelligentPointer<CThing>(&thing));
}