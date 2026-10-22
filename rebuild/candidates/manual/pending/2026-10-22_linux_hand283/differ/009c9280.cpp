// NGameText::CDataBank::GetTextEntryFromGroup(unsigned long groupID, const CCharString& speaker, bool* found,
// Grade: DIFFER(ours v retail) -- 923v861 (first pass, 3 grades): frame 0x18 v 0x20 and register roles (retail reloads `found` from its arg slot, keeps the speaker table base and index*4 in edi/ebp across both compares, and returns early without the vector free); speaker compares, temp release forms and the inline LCG pick are in place
//   long* lastIndex, bool allowPending) -- collect the group's text entries whose speaker is `speaker` (or
//   `speaker` + "_1_5"), pick one at random (not the last one picked when more than one), and return its text entry;
//   when the pick is not loaded yet (and pending is not allowed) start loading it and return the first loaded
//   candidate after it, else null
// Leaf: GetTextEntryFromGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C9280, length 864 bytes, `ret 0x14`.
// Callees: GetEntryDataType 0x009CC210, GetPGroupEntry 0x009C8890, operator+(CCharString, const char*) 0x0099F600
//   (__fastcall), ~CCharString 0x0099EAE0, CBasicString Compare 0x00411570 (__fastcall), vector<long>::push_back
//   0x005178A0, GFROR13 0x00497890 (__fastcall), FlushAsyncUpdates 0x009D68B0, GetTextInfo 0x009C7D00,
//   CDataBankTextEntry::IsLoaded 0x009C7BE0, CBankFileAsyncData::IsFinished 0x009D54E0, BeginLoadingText 0x009C8630,
//   GetPTextEntry 0x009C9070, free 0x00BFEA14.
// Layout: retail CDataBank: Entries +0x1e0 (landed GetTextInfo 0x009C7D00), speaker names +0x204, random seed +0x210;
//   group entry ids at +0x1c (landed GetPGroupEntry 0x009C8890 model: CResource 0x1c bytes then the id array).
extern "C" void __cdecl free(void* block);                    // 0x00BFEA14
unsigned long __fastcall GFROR13(unsigned long value);         // 0x00497890
struct CCharStringData {
    char*         PData;
    unsigned long Length;
};
long __fastcall Compare(const char* a, const char* b);        // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                           // 0x0099EAE0
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->PData, other.PStringData->PData) ? false : true;
        return false;
    }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

struct CLongVector {                                          // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) free(_M_start); }
    void push_back(const long& value);                        // 0x005178A0
    bool empty() const { return _M_start == _M_finish; }
    long& operator[](long i) { return _M_start[i]; }
};

struct CBankFileAsyncData {
    bool IsFinished() const;                                  // 0x009D54E0
};
struct CResourceVirtual {
    virtual void v00();
    virtual void Delete();                                    // slot 1
};
struct CRefCounted {                                          // the count sits at +4 behind the vptr
    long IVRefCount;
    void DecRef() { --IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
};
struct CTextEntry : CResourceVirtual, CRefCounted {
    void Release() { if (--IVRefCount == 0) Delete(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Delete(); }
    unsigned char       _pad08[0x38 - 8];
    CBankFileAsyncData* PFileData;                            // +0x38
};
struct CDataBankTextEntry {
    CTextEntry* PText;                                        // +0x00
    long        SpeakerIndex;                                 // +0x04
    bool IsLoaded() const;                                    // 0x009C7BE0
    __forceinline bool IsLoadedInline() const
    {
        if (PText != 0) {
            if (PText->PFileData == 0 || PText->PFileData->IsFinished())
                return true;
        }
        return false;
    }
};
struct CGroupEntry : CResourceVirtual, CRefCounted {
    void Release() { if (--IVRefCount == 0) Delete(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Delete(); }
    unsigned char _pad08[0x1c - 8];
    CLongVector   Entries;                                    // +0x1c
};
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = other.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
};

struct CDataBank {
    void*               __vftable;
    unsigned char       _pad004[0x1e0 - 4];
    CDataBankTextEntry** Entries;                             // +0x1e0 (CArray first)
    unsigned char       _pad1e4[0x204 - 0x1e4];
    CCharString*        Speakers;                             // +0x204 (CArray first)
    unsigned char       _pad208[0x210 - 0x208];
    unsigned long       RandomSeed;                           // +0x210
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool allowPending) const;   // 0x009C8890
    CIVCountedPointer<CTextEntry> GetPTextEntry(unsigned long index, bool allowPending) const;    // 0x009C9070
    void FlushAsyncUpdates() const;                           // 0x009D68B0
    CDataBankTextEntry* GetTextInfo(unsigned long index) const;   // 0x009C7D00
    void BeginLoadingText(unsigned long index) const;         // 0x009C8630
    bool IsGroup(unsigned long index) const { return GetEntryDataType(index) == 1 && Entries[index] != 0; }
    CDataBankTextEntry* GetTextInfoInline(unsigned long index) const
    {
        if (GetEntryDataType(index) == 0)
            return Entries[index];
        return 0;
    }
    unsigned long Random(unsigned long range)
    {
        RandomSeed = RandomSeed * 0x24a1 + 0x24df;
        RandomSeed = GFROR13(RandomSeed);
        return range ? RandomSeed % range : 0;
    }
    CTextEntry* GetTextEntryFromGroup(unsigned long groupID, const CCharString& speaker, bool* found,
                                      long* lastIndex, bool allowPending);
};

CTextEntry* CDataBank::GetTextEntryFromGroup(unsigned long groupID, const CCharString& speaker, bool* found,
                                             long* lastIndex, bool allowPending)
{
    *found = false;
    if (groupID <= 0 || GetEntryDataType(groupID) != 1)
        return 0;
    CLongVector candidates;
    {
        CIVCountedPointer<CGroupEntry> group;
        if (!IsGroup(groupID))
            return 0;
        group = GetPGroupEntry(groupID, allowPending);
        if (group.Object == 0) {
            *found = true;
            return 0;
        }

        for (unsigned long i = 0; i < (unsigned long)(group.Object->Entries._M_finish - group.Object->Entries._M_start); ++i) {
            long id = group.Object->Entries._M_start[i];
            CDataBankTextEntry* info = GetTextInfoInline(id);
            const CCharString& name = Speakers[info->SpeakerIndex];
            if (name == speaker || name == speaker + "_1_5")
                candidates.push_back(id);
        }
    }
    if (candidates.empty())
        return 0;
    *found = true;
    long count = candidates._M_finish - candidates._M_start;
    long pick = 0;
    long id = candidates[0];
    if (count > 1) {
        do {
            pick = Random(count);
            id = candidates[pick];
        } while (lastIndex && id == *lastIndex);
    }
    FlushAsyncUpdates();
    if (GetTextInfo(id)->IsLoadedInline() || allowPending) {
        if (lastIndex)
            *lastIndex = id;
        return GetPTextEntry(id, true).Object;
    }
    BeginLoadingText(id);
    long index = pick;
    for (long tries = 0; tries < count; ++tries) {
        long other = candidates[index];
        FlushAsyncUpdates();
        if (GetTextInfo(other)->IsLoaded()) {
            if (lastIndex)
                *lastIndex = other;
            return GetPTextEntry(other, true).Object;
        }
        index = (index + 1) % count;
    }
    return 0;
}
