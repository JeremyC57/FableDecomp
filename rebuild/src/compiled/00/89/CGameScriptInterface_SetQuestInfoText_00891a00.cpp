// CGameScriptInterface::SetQuestInfoText(const char* symbol) const -- look the game-text symbol up in the text bank
// and show its text as the body text of the player GUI's quest-info panel
// Leaf: ?SetQuestInfoText@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00891A00, length 192 bytes, `ret 4`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the CRC Calc @ 0x004014A0 (`__fastcall(seed,
// data, length)`), NGameText::NGameText::CDataBank::LookupValue @ 0x009CC410 (the bank's CRC -> tag map at +0xD0, 0 when
// absent; labelled CVectorMap::LookupValue), ~CCharString @ 0x0099EAE0, NGameText::CDataBank::GetPGroupEntry(unsigned
// long, bool) const @ 0x009C9070, NGameText::CGroupEntry::GetText() const @ 0x009C7A40, CWideString::operator= @
// 0x0099B7D0, ~CWideString @ 0x0099B510, the entry's slot 1 on release. Globals: the engine 0x013B86A0 (+0x14 the
// game-text bank), the player GUI 0x013B8790; "" @ 0x0129AAF4.
// Layout: CPlayerGui PDrawQuestInfo +0x1C8 (PDB +0x1D0), NPlayerGui::CDrawQuestInfo Name +0x50 / Text +0x54 (PDB
// +0x60/+0x64).
// Shape: the bank is a named local (edi across the calls); the symbol is an unnamed CCharString temporary whose inline
// GetCRC() evaluates the length then the chars (`Calc(0, chars, length)`, arguments right to left); the counted group
// entry lands in the dead `symbol` argument slot and its dtor is the null-tested inline ReleaseTemporary; the
// GetText() wide-string temporary takes the `push ecx` frame word.

unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0

class CCharStringData { public:
    char* PData;                          // +0x00
    unsigned long StringLength;           // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return Calc(0, GetString(), GetLength()); }
};

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                 // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

struct CRefCounted {
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy() const;                         // slot 1
};
namespace NGameText {
class CTextEntry : public CRefVirtual, public CRefCounted { public:
    CWideString GetText() const;                    // 0x009C7A40
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
}
template <class T> class CIVCountedPointer { public:
    T* Data;
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long tag, bool b) const;   // 0x009C9070
public:
    unsigned long LookupValue(unsigned long) const;   // 0x009CC410
};
}
class CEngine { public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;        // +0x14
};
extern CEngine* g_pEngine;                          // 0x013B86A0

namespace NPlayerGui {
class CDrawQuestInfo { public:
    unsigned char _pad00[0x50];
    CWideString Name;                               // +0x50
    CWideString Text;                               // +0x54
};
}
template <class T> class CCountedPointer { public:
    T* Data;
    void* Info;
};
class CPlayerGui { public:
    unsigned char _pad000[0x1c8];
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo;   // +0x1C8
    NPlayerGui::CDrawQuestInfo& GetQuestInfo() { return *PDrawQuestInfo.Data; }
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

class CGameScriptInterface { public:
    virtual void SetQuestInfoText(const char* symbol) const;
};

void CGameScriptInterface::SetQuestInfoText(const char* symbol) const
{
    NGameText::CDataBank* bank = g_pEngine->PGameTextDataBank;
    unsigned long tag = bank->LookupValue(CCharString(symbol, -1).GetCRC());
    if (tag > 0) {
        CIVCountedPointer<const NGameText::CTextEntry> entry = bank->GetPTextEntry(tag, true);
        g_pPlayerGui->GetQuestInfo().Text = entry->GetText();
    }
}