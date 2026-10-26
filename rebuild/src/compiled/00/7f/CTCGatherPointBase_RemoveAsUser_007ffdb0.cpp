// CTCGatherPointBase::RemoveAsUser(const CThing& user) -- drop a user from the gather point's user list (STLport remove of its pointer, then a single-element erase at the new end)
// Leaf: ?RemoveAsUser@CTCGatherPointBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FFDB0, 136 bytes (+ int3 padding to 144), __fastcall, `ret 4`.
//   Callers: CAIStateGroup_MinionSitOnSeatBase::Cleanup 0x0090FA60 and CAIStateGroup_SitInPub::Update 0x0094B740 (on
//   the interface-0x7B component); the landed twin CTCGatherPointBase::AddAsUser 0x007FFEB0 follows it.
// Name: Ego_r.pdb `?RemoveAsUser@CTCGatherPointBase@@QAEXABVCThing@@@Z` (donor 0x007F3570, next to AddAsUser and the
//   vector<CIntelligentPointer<const CThing> >::erase instantiation); the catalog's
//   "CAIState_RemoveEquippedCombatAbilities" is descriptive.
// Callees: CBaseIntelligentPointer ctor/SetPItem/dtor 0x00A01B10/0x00A01B90/0x00A01C10 (CIntelligentPointer<const CThing>
//   vtable 0x01237F1C), STLport find(first, last, const value&, random_access_iterator_tag) 0x007379D0, remove_copy(first,
//   last, result, const value&) 0x00737AD0, __copy(first, last, result, random_access_iterator_tag, int*) 0x00737D50 (all
//   __fastcall), the element's virtual destructor (slot 0, flag 0).
// Layout: retail CTCGatherPointBase (the landed AddAsUser model): Users vector<CIntelligentPointer<const CThing> > +0x0c
//   (12-byte STLport vector of 8-byte elements), MaxOccupation +0x18.
// Shape: a named CIntelligentPointer<const CThing> local, `it = remove(Users.begin(), Users.end(), pointer)` and
//   `Users.erase(it)` as separate statements -- the single-position erase, so only the last slot is dropped. STLport
//   remove inlined (find with the tag, then remove_copy from the element after the hit), erase inlined (`if (pos + 1 !=
//   end()) __copy_ptrs(pos + 1, _M_finish, pos, __false_type())` -> `__copy(.., random_access_iterator_tag(), (int*)0)`;
//   `--_M_finish; _Destroy(_M_finish)` -- the explicit destructor call goes through the vtable with flag 0). Both empty
//   tag temporaries are homed in the dead `user` argument slot only when the two inline calls are separate statements
//   (`erase(remove(...))` nests remove's scope in erase's, so the second tag gets a new frame word: `sub esp,0Ch`).
class CThing;

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10 (slot 0)
    const CThing* PItem;                        // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
typedef CIntelligentPointerConstThing CPtr;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
CPtr* __fastcall find(CPtr* first, CPtr* last, const CPtr& value, const random_access_iterator_tag& tag);   // 0x007379D0
CPtr* __fastcall remove_copy(CPtr* first, CPtr* last, CPtr* result, const CPtr& value);                  // 0x00737AD0
CPtr* __fastcall __copy(CPtr* first, CPtr* last, CPtr* result, const random_access_iterator_tag& tag, int* dist);   // 0x00737D50
inline CPtr* remove(CPtr* first, CPtr* last, const CPtr& value)
{
    first = find(first, last, value, random_access_iterator_tag());
    if (first == last)
        return first;
    CPtr* next = first;
    return remove_copy(++next, last, first, value);
}

struct CPtrVector {                             // STLport vector<CIntelligentPointer<const CThing> > (12 bytes)
    CPtr* _M_start;
    CPtr* _M_finish;
    CPtr* _M_end_of_storage;
    CPtr* begin() { return _M_start; }
    CPtr* end() { return _M_finish; }
    CPtr* erase(CPtr* position)
    {
        if (position + 1 != end())
            __copy(position + 1, _M_finish, position, random_access_iterator_tag(), (int*)0);
        --_M_finish;
        _M_finish->~CPtr();
        return position;
    }
};

class CTCGatherPointBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CPtrVector Users;                           // +0x0C
    long MaxOccupation;                         // +0x18
    void RemoveAsUser(const CThing& user);
};

void CTCGatherPointBase::RemoveAsUser(const CThing& user)
{
    CPtr pointer(&user);
    CPtr* it = remove(Users.begin(), Users.end(), pointer);
    Users.erase(it);
}