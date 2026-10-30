// CTCEnemy::RemoveAllyThing(const CThing&) -- removes the thing from the ally list
// Leaf: ?RemoveAllyThing@CTCEnemy@@QAEX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076C850, 66 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: CTCEnemy::RemoveAllyThing(const CThing&) -- proven; evidence: it sits in the CTCEnemy module (after CTCEnemy::SetFaction 0x0076C810, before the CTCEnemy constructor 0x0076C8A0);
//   Ego_r.pdb CTCEnemy has EnemyThings / AllyThings, two list<CIntelligentPointer<const CThing> > (retail +0x10 / +0x14);
//   callers CGameScriptInterface::ImpSetEntityAsAllyOfThing 0x0089C100 and EntityUnsetThingAsAllyOfThing 0x008A7EE0;
//   FableWin.pdb name set holds ?RemoveAllyThing@CTCEnemy@@QAEXABVCThing@@@Z.
//   Family G039 (8 bodies: one temporary intelligent pointer handed to a list/vector member; the propagated label
//   "CCreatureActionBase::AddDependentThing" is wrong for all eight -- that member sits at +0x9C in retail).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90, std::list<CIntelligentPointer<const CThing> >::remove 0x0076D7B0, CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10.
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
class CFaction;
class CTCEnemy : public CTCBase {
public:
    void RemoveAllyThing(const CThing& thing);
    bool FriendsWithEverythingFlag;                          // +0x0c
    bool EnableFollowersEnemyProxy;                          // +0x0d
    std::list<CIntelligentPointer<const CThing> > EnemyThings;   // +0x10
    std::list<CIntelligentPointer<const CThing> > AllyThings;    // +0x14
    CFaction* PFaction;                                      // +0x18
};

void CTCEnemy::RemoveAllyThing(const CThing& thing)
{
    AllyThings.remove(CIntelligentPointer<const CThing>(&thing));
}