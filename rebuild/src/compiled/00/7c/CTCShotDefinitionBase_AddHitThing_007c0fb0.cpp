// CTCShotDefinitionBase::AddHitThing(const CThing*) -- appends the thing to the shot's hit-things list
// Leaf: ?AddHitThing@CTCShotDefinitionBase@@QAEX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C0FB0, 66 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: CTCShotDefinitionBase::AddHitThing(const CThing*) -- descriptive name; evidence: it sits in the CTCShotDefinitionBase module (after CreateShot 0x007C0E80, before
//   CTCShotDefinitionBase::OnCollisionProper 0x007C1000); Ego_r.pdb CTCShotDefinitionBase::HitThingsList is the
//   vector<CIntelligentPointer<const CThing> > at +0x14 (after the 8-byte PEffect); callers the predictive projectile's
//   sub-shot trigger 0x007BD527 and CCreatureAction_DragonRearUpFull::PerformAction 0x008F3630. No PDB name set holds a
//   CTCShotDefinitionBase method; the name is descriptive (the landed callers' spelling).
//   Family G039 (8 bodies: one temporary intelligent pointer handed to a list/vector member; the propagated label
//   "CCreatureActionBase::AddDependentThing" is wrong for all eight -- that member sits at +0x9C in retail).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90, std::vector<CIntelligentPointer<const CThing> >::push_back 0x007398B0, CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10.
//   Vtable: CIntelligentPointer<const CThing> 0x01237F1C.
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
class CTCShotDefinitionBase : public CTCBase {
public:
    void AddHitThing(const CThing* thing);
    CIntelligentPointer<CThing> PEffect;                     // +0x0c
    std::vector<CIntelligentPointer<const CThing> > HitThingsList;   // +0x14
};

void CTCShotDefinitionBase::AddHitThing(const CThing* thing)
{
    HitThingsList.push_back(CIntelligentPointer<const CThing>(thing));
}