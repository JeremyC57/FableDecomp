// CTCThingOwner::RemoveOwnedObject(CThing&) -- removes the thing from the owned-object list
// Leaf: ?RemoveOwnedObject@CTCThingOwner@@AAEX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071B1B0, 66 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: CTCThingOwner::RemoveOwnedObject(CThing&) -- proven; evidence: it sits in the CTCThingOwner module (before CTCThingOwner::OnKill 0x0071B200 / ClearDeadEntries 0x0071B240);
//   Ego_r.pdb CTCThingOwner::ObjectList is the list<CIntelligentPointer<CThing> > at +0xC; its one caller is
//   CTCOwnedEntity::SetPOwner 0x007E8590 (the old owner drops the entity; RemoveOwnedObject is private, CTCOwnedEntity a friend);
//   FableWin.pdb name set holds ?RemoveOwnedObject@CTCThingOwner@@AAEXAAVCThing@@@Z.
//   Family G039 (8 bodies: one temporary intelligent pointer handed to a list/vector member; the propagated label
//   "CCreatureActionBase::AddDependentThing" is wrong for all eight -- that member sits at +0x9C in retail).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90, std::list<CIntelligentPointer<CThing> >::remove 0x0065DE8A, CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10.
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
class CTCOwnedEntity;
class CTCThingOwner : public CTCBase {
    friend class CTCOwnedEntity;
    void RemoveOwnedObject(CThing& thing);                   // private (FableWin.pdb AAE)
public:
    std::list<CIntelligentPointer<CThing> > ObjectList;      // +0x0c
    bool CallOnDie;                                          // +0x10
};

void CTCThingOwner::RemoveOwnedObject(CThing& thing)
{
    ObjectList.remove(CIntelligentPointer<CThing>(&thing));
}