// CSubtitleRenderer::GetTextFromTextTag(const CWideString& tag, CWideString& text) -- look the tag up in the game text bank: a text entry gives its text; a group entry gives its next member's text (remembering the tag, advancing the group index and whether entries remain); an unknown tag, or a group already exhausted, copies the tag itself and returns false
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: GetTextFromTextTag
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048F6BD, 259 bytes, `ret 8`. Ego_r.pdb
//   `?GetTextFromTextTag@CSubtitleRenderer@@AAE_NABVCWideString@@AAV2@@Z` (private).
// Callees: CWideString::ConvertToCharString() const 0x0099B2C0, the CCharString hash 0x00404310 (manifest
//   CCharString_ComputeCRC32), CDataBank::LookupValue(unsigned long) const 0x009CC410, ~CCharString 0x0099EAE0,
//   CBankFile::GetEntryDataType(unsigned long) const 0x009CC210, CDataBank::GetPGroupEntry(unsigned long, bool) const
//   0x009C8890, CDataBank::GetPTextEntry(unsigned long, bool) const 0x009C9070 (the landed rows call it GetPGroupEntry;
//   type 0 is text, so this is the text-entry getter), CTextEntry::GetText() const 0x009C7A40 (CWideString by hidden
//   pointer), CWideString::operator= 0x0099B7D0, ~CWideString 0x0099B510.
// Globals: g_pMainGameComponent 0x013B86A0 (+0x14 the game text bank).
// Shape: frame-pointer size TU (the CSubtitleRenderer siblings); the bank in ebx; the text call is written in both
//   arms (their identical pushes hoist above the type test and the calls cross-jump); the group member is read through a
//   named `items` pointer (reuses esi; indexing the array directly picks eax); every by-value temporary (the char
//   string, both counted pointers) lives in the one frame word, the text in the dead `tag` slot; the counted pointer's
//   destructor releases and nulls (the null store dies at the end); the group and text getters share their pushes and
//   the text call (cross-jump).
// Retail layout: CSubtitleRenderer TextGroupHasEntriesRemaining +0x28, TextGroupIndex +0x2C, TextGroupTag +0x30 (PDB
//   offsets); NGameText::CGroupEntry Entries (CArray<long>) +0x1C.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    unsigned long GetCRC() const;                       // 0x00404310
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};
template <class T> class CArray { public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long i) const { return m_pFirst[i]; }
};
class CIVCountedPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
    __forceinline void Release() const { if (--RefCount == 0) Destroy(); }
};
template <class T> class CIVCountedPointer {
public:
    T* Data;
    __forceinline ~CIVCountedPointer() { if (Data) { Data->Release(); Data = 0; } }
    T* operator->() const { return Data; }
};
class CBankFile
{
public:
    unsigned long GetEntryDataType(unsigned long) const;   // 0x009CC210
};
class CBankFileAsync : public CBankFile
{
public:
};

namespace NGameText {
class CGroupEntry : public CIVCountedPointeeBase { public:
    unsigned char _pad08[0x1c - 0x08];
    CArray<long>  Entries;                              // +0x1c
};
class CTextEntry : public CIVCountedPointeeBase { public:
    CWideString GetText() const;                        // 0x009C7A40
};
class CDataBank : public CBankFileAsync {
public:
    unsigned long LookupValue(unsigned long crc) const;                                  // 0x009CC410
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long id, bool load) const;   // 0x009C8890
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long id, bool load) const;     // 0x009C9070
};
}
class CMainGameComponent { public:
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* TextBank;                     // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

class CSubtitleRenderer {
public:
    void*         __vftable;                            // +0x00
    unsigned char _pad04[0x28 - 0x04];
    bool          TextGroupHasEntriesRemaining;         // +0x28
    long          TextGroupIndex;                       // +0x2c
    CWideString   TextGroupTag;                         // +0x30
    bool GetTextFromTextTag(const CWideString& tag, CWideString& text);
};

bool CSubtitleRenderer::GetTextFromTextTag(const CWideString& tag, CWideString& text)
{
    NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
    unsigned long id = bank->LookupValue(tag.ConvertToCharString().GetCRC());
    if (id > 0) {
        if (bank->GetEntryDataType(id)) {
            const CArray<long>* entries = &bank->GetPGroupEntry(id, true)->Entries;
            TextGroupTag = tag;
            ++TextGroupIndex;
            TextGroupHasEntriesRemaining = false;
            if ((unsigned long)TextGroupIndex >= entries->size())
                goto fail;
            if (TextGroupIndex < (long)entries->size() - 1)
                TextGroupHasEntriesRemaining = true;
            const long* items = entries->m_pFirst;
            text = bank->GetPTextEntry(items[TextGroupIndex], true)->GetText();
        } else
            text = bank->GetPTextEntry(id, true)->GetText();
        return true;
    }
fail:
    text = tag;
    return false;
}