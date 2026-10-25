// CTCRegionFollower::RemoveFollower(CThing&) -- removes the thing from the region-follower list
// Leaf: ?RemoveFollower@CTCRegionFollower@@QAEX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AEBA0, 66 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: CTCRegionFollower::RemoveFollower(CThing&) -- proven; evidence: it sits in the CTCRegionFollower module (between CTCRegionFollower::IsRegionFollower 0x006AEB60 and
//   IsCreatureAllowedInRegionAt 0x006AEBF0); Ego_r.pdb CTCRegionFollower::Followers is the list<CIntelligentPointer<CThing> > at +0xC;
//   callers CTCFollowed::QuitAIFollower / FrameUpdate, CGameScriptInterface::SetEntityAsRegionFollowing, CWorld::UpdateRegionLoad;
//   FableWin.pdb name set holds ?RemoveFollower@CTCRegionFollower@@QAEXAAVCThing@@@Z.
//   Family G039 (8 bodies: one temporary intelligent pointer handed to a list/vector member; the propagated label
//   "CCreatureActionBase::AddDependentThing" is wrong for all eight -- that member sits at +0x9C in retail).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90, std::list<CIntelligentPointer<CThing> >::remove 0x0065DE8A, CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10.
//   Vtable: CIntelligentPointer<CThing> 0x01238C6C.
// Shape: the temporary's inline (T*) constructor (base ctor, own vtable, SetPItem) and its implicit destructor (a direct
//   call of the base destructor, no vtable reset); no EH frame (the module is built without /GX).
// Model: local -- retail STLport lists are 4 bytes and vectors 12 (the generated engine headers carry Ego_r's 12/16-byte
//   Dinkumware containers and no member functions), so the class is laid out here at its retail offsets.
class CBaseObject {
public:
    virtual ~CBaseObject();
};
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
class CTCRegionFollower : public CTCBase {
public:
    void RemoveFollower(CThing& thing);
    std::list<CIntelligentPointer<CThing> > Followers;       // +0x0c
};

void CTCRegionFollower::RemoveFollower(CThing& thing)
{
    Followers.remove(CIntelligentPointer<CThing>(&thing));
}