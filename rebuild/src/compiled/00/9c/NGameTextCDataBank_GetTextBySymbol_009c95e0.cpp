// NGameText::CDataBank::GetTextBySymbol(const CCharString& symbol) const -- the wide text of a symbol: CRC the symbol, look the entry up, and when it is a plain text entry (data type 0) return a copy of its text; otherwise return the bank's empty string
// Leaf: GetTextBySymbol
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C95E0, length 150 bytes (`ret 8`: hidden result + symbol).
// Callees: Calc @ 0x004014A0 (`__fastcall(0, chars, length)`, the symbol's CRC; "" @ 0x0129AAF4 for a null string),
// CDataBank::LookupValue(unsigned long) @ 0x009CC410 (entry index, 0 = none), CDataBank::GetEntryDataType(unsigned long)
// @ 0x009CC210, CDataBank::GetPGroupEntry(unsigned long, bool) @ 0x009C9070 (returns CIVCountedPointer<const CGroupEntry> by
// value through the hidden pointer), CWideString::CWideString(const CWideString&) @ 0x0099B720 (the entry's text at +0x28,
// or the empty wide string 0x013BCA24).
// Shape: length then text are read through two null-case-first accessors (VC evaluates the right-hand argument first);
// the counted pointer returned by value is a NAMED local homed in the dead `symbol` argument slot (re-read from the slot:
// `mov esi,[esp+10h]`; reading the temporary's member uses the returned eax); the release is written by hand on the
// pointer local through an inline guard after the text copy (`dec [esi+4] ; jne ; call [vtable+4]`): a destructor on the
// counted-pointer object re-reads the slot at every use.
class CCharStringData { public:
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: ""
class CCharString { public:
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC

class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);          // 0x0099B720
};
extern CWideString g_emptyWideString_013bca24;      // 0x013BCA24

namespace NGameText {
class CTextEntry { public:                                // NGameText::CGroupEntry (ref-counted, polymorphic)
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long  m_refCount;                       // +0x04
    unsigned char _pad08[0x20];
    CWideString   m_text;                           // +0x28
};
}
using NGameText::CTextEntry;
template <class T> class CIVCountedPointer { public:   // CIVCountedPointer<const NGameText::CTextEntry>
    T* m_pObject;
    CIVCountedPointer(const CIVCountedPointer& other);
};
struct CEntryReference {                            // holds the reference taken by GetPGroupEntry until the text is copied
    const CTextEntry* m_pEntry;
    __forceinline CEntryReference(const CTextEntry* entry) : m_pEntry(entry) { }
    __forceinline ~CEntryReference()
    {
        if (--m_pEntry->m_refCount == 0)
            m_pEntry->Destroy();
    }
};

class CBankFile { public:                                       // primary base of NGameText::CDataBank (Ego_r)
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210 (FableWin ?GetEntryDataType@CBankFile@@QBEKK@Z)
};
namespace NGameText {
class CDataBank : public CBankFile { public:
    unsigned long LookupValue(unsigned long crc) const;          // 0x009CC410
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
    const CWideString GetTextBySymbol(const CCharString& symbol) const;
};

const CWideString CDataBank::GetTextBySymbol(const CCharString& symbol) const
{
    unsigned long index = LookupValue(Calc_004014a0(0, symbol.GetChars(), symbol.GetLength()));
    if (index > 0 && GetEntryDataType(index) == 0) {
        CIVCountedPointer<const CTextEntry> pointer = GetPTextEntry(index, true);
        const CTextEntry* entry = pointer.m_pObject;
        if (entry) {
            CEntryReference reference(entry);
            return entry->m_text;
        }
    }
    return g_emptyWideString_013bca24;
}
}