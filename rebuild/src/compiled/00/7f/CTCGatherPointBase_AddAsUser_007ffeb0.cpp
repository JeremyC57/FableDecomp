// CTCGatherPointBase::AddAsUser(const CThing& user) -- register a user of the gather point: true when it already is one or there was room for it, false when the point is over its occupation limit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FFEB0, length 167 bytes. __fastcall, `ret 4`.
//         Ego_r.pdb: `?AddAsUser@CTCGatherPointBase@@QAE_NABVCThing@@@Z`.
// Callees: CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10 (CIntelligentPointer<const CThing>
//          vtable 0x01237F1C); STLport find(first, last, const value&, random_access_iterator_tag) @ 0x007379D0 (__fastcall:
//          first in ecx, last in edx); vector<CIntelligentPointer<const CThing>>::push_back(const T&) @ 0x007398B0.
// Shape: the lookup value is a named temporary in its own block (destroyed before the end test); the iterator tag is an
//        empty temporary homed in the dead `user` slot; size() is the signed pointer difference compared unsigned with
//        MaxOccupation (`sar ; cmp ; ja`); the exits are byte constants, the push in the `if` arm
//        and `return false` in the `else` (an early `return false` places its epilogue before the push).
// Layout: retail CTCGatherPointBase: Users +0x0C (12-byte STLport vector), MaxOccupation +0x18 (PDB +0x1C).
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    const CThing* PItem;                        // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
typedef CIntelligentPointerConstThing CPtr;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
CPtr* __fastcall find(CPtr* first, CPtr* last, const CPtr& value, const random_access_iterator_tag& tag);   // 0x007379D0

struct CPtrVector {                             // STLport vector<CIntelligentPointer<const CThing> > (12 bytes)
    CPtr* _M_start;
    CPtr* _M_finish;
    CPtr* _M_end_of_storage;
    CPtr* begin() { return _M_start; }
    CPtr* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    void push_back(const CPtr& value);          // 0x007398B0
};

struct CTCGatherPointBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CPtrVector Users;                           // +0x0C
    long MaxOccupation;                         // +0x18
    bool AddAsUser(const CThing& user);
};

bool CTCGatherPointBase::AddAsUser(const CThing& user)
{
    CPtr* it;
    {
        CPtr pointer(&user);
        it = find(Users.begin(), Users.end(), pointer, random_access_iterator_tag());
    }
    if (it == Users.end()) {
        if (Users.size() <= (unsigned long)MaxOccupation)
            Users.push_back(CPtr(&user));
        else
            return false;
    }
    return true;
}