// CAIStateGroup_CreatureUseCoverBase::RemoveThingFromCoverList(CThing* thing) -- erases every cover-list entry that points at the thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009708A0, length 141 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90, the element's virtual
// destructor (slot 0, `push 0 ; call [eax]`).
// Shape: `for (it = begin(); it != end(); ) if (it->GetPItem() == thing) it = erase(it); else ++it;` over the
// retail STLport vector<CIntelligentPointer<CThing>> at +0x28 (first) / +0x2c (finish); erase is STLport's
// `if (pos + 1 != _M_finish) copy(pos + 1, _M_finish, pos); --_M_finish; _Destroy(_M_finish); return pos;` with
// the random-access `__copy` loop (`for (n = last - first; n > 0; --n)`, the count in ebx, 16-aligned head) whose
// element assignment is the inline operator= (SetPItem(other.GetPItem())), and the destroy a direct
// virtual-destructor call. The iterator and its successor are spilled to the frame across the copy loop.
// Model: local (engine/CAIStateGroup_CreatureUseCoverBase.h gives CoverList +0x28 with the PDB's 16-byte vector;
// retail's is 12).
// Shape: the CIntelligentPointer operator= names the GetPItem() CALL result (`item = other.GetPItem(); SetPItem(item);`), which
//   keeps the member's address (this of SetPItem) after the source's GetPItem call and colours the registers as retail; the
//   inline `SetPItem(other.GetPItem())` form evaluates the inlined member's `this` first.
struct CThing;

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    void* m_pItem;                                  // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

__forceinline CIntelligentPointerThing* CopyRange(CIntelligentPointerThing* first, CIntelligentPointerThing* last, CIntelligentPointerThing* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

struct CCoverThingVector {                          // std::vector<CIntelligentPointer<CThing> > (STLport, 12 bytes)
    CIntelligentPointerThing* _M_start;
    CIntelligentPointerThing* _M_finish;
    CIntelligentPointerThing* _M_end_of_storage;
    CIntelligentPointerThing* begin() { return _M_start; }
    CIntelligentPointerThing* end() { return _M_finish; }
    __forceinline CIntelligentPointerThing* erase(CIntelligentPointerThing* pos)
    {
        if (pos + 1 != _M_finish)
            CopyRange(pos + 1, _M_finish, pos);
        --_M_finish;
        _M_finish->~CIntelligentPointerThing();
        return pos;
    }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {
    unsigned char _pad_0x14[8];                     // +0x14
    bool HasDiveAnim;                               // +0x1c
    bool HasJumpOutAnims;                           // +0x1d
    bool TimingCritical;                            // +0x1e
    unsigned char _pad_0x1f;
    unsigned char PCoverThing[8];                   // +0x20 CIntelligentPointer<CThing>
    CCoverThingVector CoverList;                    // +0x28
    void RemoveThingFromCoverList(CThing* thing);
};

void CAIStateGroup_CreatureUseCoverBase::RemoveThingFromCoverList(CThing* thing)
{
    for (CIntelligentPointerThing* it = CoverList.begin(); it != CoverList.end(); ) {
        if (it->GetPItem() == thing)
            it = CoverList.erase(it);
        else
            ++it;
    }
}