// Grade: DIFFER(269v268) -- register colouring: with the same-TU getter stand-ins the frame (0x10, outer index spilled) and the text pointer held across the CWideString calls match, but retail keeps `this` in ebp and the index/text pointer in ebx where the lane swaps them; the group release forms `add eax,4` where the lane uses `lea edx,[eax+4]`. Tried: member vs local-copy releases (swapped forms), DecRefToZero, getter stand-ins (this).
// NGameText::CDataBank::Validate() -- walk every bank entry from index 1: for each group entry, touch each member text entry (its data type and its counted text-entry pointer, copying the entry's text) -- the checks themselves are compiled out of the release build
// Leaf: Validate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C9170, length 268 bytes (+ int3 padding).
// Callees: CBankFile::GetEntryDataType(unsigned long) @ 0x009CC210, CDataBank::GetPGroupEntry(unsigned long, bool) @ 0x009C8890 and the
// text-entry getter @ 0x009C9070 (labelled "GetPGroupEntry" too; both return a CIVCountedPointer by hidden pointer), CWideString copy
// ctor / dtor @ 0x0099B720 / 0x0099B510; each pointee is destroyed through its virtual slot 1 at zero references.
// Layout: as the landed CDataBank::GetTextInfo 0x009C7D00 (retail Entries CArray at +0x1e0); the group entry's member-index array at
// +0x1c, the text entry's CWideString at +0x28 (descriptive).
// Shape: the group pointer is a full-expression temporary (released right after `->Members` is bound: the long `DecRef ;
// GetRefCount` release through eax), the text pointer a loop-scope local (short `dec [ebx+4] ; jne` release); the text copy is an
// unused CWideString local (an assertion body in the original); the outer index is kept in the frame.
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
};

template <class T> struct CArray {      // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }

struct CGroupEntry {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long RefCount;                                 // +0x04
    unsigned char _pad08[0x14];
    CArray<unsigned long> Members;                 // +0x1c
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
};
struct CTextEntry {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long RefCount;                                 // +0x04
    unsigned char _pad08[0x20];
    CWideString Text;                              // +0x28
};

struct CGroupEntryPointer {                        // CIVCountedPointer<const CGroupEntry>
    CGroupEntry* Object;
    CGroupEntryPointer(CGroupEntry* object) : Object(object) { if (object) ++object->RefCount; }
    CGroupEntry* operator->() const { return Object; }
    ~CGroupEntryPointer()
    {
        CGroupEntry* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
};
struct CTextEntryPointer {                         // CIVCountedPointer<const CTextEntry>
    CTextEntry* Object;
    CTextEntryPointer(CTextEntry* object) : Object(object) { if (object) ++object->RefCount; }
    CTextEntry* operator->() const { return Object; }
    ~CTextEntryPointer()
    {
        if (Object) {
            if (DecRefToZero(Object->RefCount))
                Object->Destroy();
            Object = 0;
        }
    }
};

struct CDataBankEntry;
struct CDataBank {
    void*                   __vftable;               // +0x000
    unsigned char           _bankFile[0x1e0 - 4];
    CArray<CDataBankEntry*> Entries;                 // +0x1e0
    unsigned long GetEntryDataType(unsigned long index) const;                    // 0x009CC210
    CGroupEntryPointer GetPGroupEntry(unsigned long index, bool load) const;      // 0x009C8890
    CTextEntryPointer GetPTextEntry(unsigned long index, bool load) const;        // 0x009C9070
    CGroupEntry* FindGroupEntry(unsigned long index, bool load) const;            // descriptive: the lookup the real getters perform
    CTextEntry* FindTextEntry(unsigned long index, bool load) const;
    void Validate();
};

// Same-TU stand-ins for the two out-of-line getters (0x009C8890, 0x009C9070), compiled above Validate under auto_inline(off) so
// VC7.1 sees, as retail's whole-program build did, that the returned pointer objects do not escape: the text pointer then stays
// in ebx across the CWideString calls. Only Validate is graded.
#pragma auto_inline(off)
CGroupEntryPointer CDataBank::GetPGroupEntry(unsigned long index, bool load) const { return CGroupEntryPointer(FindGroupEntry(index, load)); }
CTextEntryPointer CDataBank::GetPTextEntry(unsigned long index, bool load) const { return CTextEntryPointer(FindTextEntry(index, load)); }
#pragma auto_inline(on)

void CDataBank::Validate()
{
    for (unsigned long i = 1; i < Entries.size(); ++i) {
        if (GetEntryDataType(i) == 1) {
            const CArray<unsigned long>& members = GetPGroupEntry(i, true)->Members;
            for (unsigned long j = 0; j < members.size(); ++j) {
                GetEntryDataType(members[j]);
                CTextEntryPointer text = GetPTextEntry(members[j], true);
                CWideString copy(text->Text);
            }
        }
    }
}
