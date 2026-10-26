// CTCSolidToThing::FrameUpdate() -- every 75th frame (staggered by the owner's UID) strip the null entries out of the not-solid-to-things list
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770E80, length 158 bytes (padded row 160).
// Callees: CTCBase::GetWorld @ 0x00686A70 (result unused: the static CWorld::GetWorldFrame @ 0x0049D870 is called through it), __aullrem
// @ 0x00BFEBB0 (`% 75` on the unsigned 64-bit sum), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 + the inline
// CIntelligentPointer<const CThing> vtable store 0x01237F1C + SetPItem @ 0x00A01B90 (the null value), STLport
// find(first, last, const value&, random_access_iterator_tag) @ 0x007379D0 and remove_copy(first, last, result, const value&) @ 0x00737AD0
// (both __fastcall; manifest labels "CCombatAbilityBlockingData_UpdateFromEquipped" / "_Remove_if<...>" are byte twins), vector::erase(first,
// last) @ 0x00738D70 (manifest "CTCInterface_ProcessQueuedCalls"), ~CBaseIntelligentPointer @ 0x00A01C10.
// Layout: Ego_r.pdb CTCSolidToThing: NotSolidToThings (vector<CIntelligentPointer<const CThing> >, retail 12 bytes) +0x0C. CThing UID
// (unsigned __int64) +0x18.
// Shape: `v.erase(remove(v.begin(), v.end(), CIntelligentPointer<const CThing>(0)), v.end())` with STLport's inline remove (find with the
// iterator tag, then remove_copy from the element after the hit); erase's `end()` is read first (ebx). The world and the UID are named
// locals in retail order (GetWorld call, UID load, frame call); the inline expression reorders the loads and the later registers.
class CThing { public:
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
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
CPtr* __fastcall remove_copy(CPtr* first, CPtr* last, CPtr* result, const CPtr& value);                  // 0x00737AD0
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
    bool empty() const { return _M_start == _M_finish; }
    CPtr* erase(CPtr* first, CPtr* last);       // 0x00738D70
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CWorld* GetWorld();                         // 0x00686A70
};
class CTCSolidToThing : public CTCBase { public:
    CPtrVector NotSolidToThings;                // +0x0c
    void FrameUpdate();
};

void CTCSolidToThing::FrameUpdate()
{
    CWorld* world = GetWorld();
    unsigned __int64 uid = ParentThing->UID;
    if ((uid + world->GetWorldFrame()) % 75 == 0) {
        if (!NotSolidToThings.empty())
            NotSolidToThings.erase(remove(NotSolidToThings.begin(), NotSolidToThings.end(), CPtr(0)), NotSolidToThings.end());
    }
}