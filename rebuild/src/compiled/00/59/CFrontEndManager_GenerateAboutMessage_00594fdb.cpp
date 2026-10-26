// CFrontEndManager::GenerateAboutMessage() -- build the front end's "about" text: every entry of the TEXT_GUI_MENU_ABOUT_MESSAGE_PART_1 text group, one line each, then the string the registry holds under the product key (when not empty), then every entry of PART_2
// Leaf: GenerateAboutMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594FDB, length 0x247 bytes, `ret 4` (`this` unused; CWideString by hidden pointer).
//   Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)` (the landed CFrontEndManager rows).
// Callees: CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0 / GetCRC 0x00404310, NGameText::CDataBank::GetTagFromCRC
//   0x009CC410 / GetPGroup 0x009C8890 / GetPGroupEntry 0x009C9070, NGameText::CGroupEntry::GetText 0x009C7A40, CWideString()
//   0x0099AED0 / (const wchar_t*) 0x0099B6B0 / copy 0x0099B720 / dtor 0x0099B510 / operator+=(const CWideString&) 0x0099B8D0 /
//   operator+=(const wchar_t*) 0x0099B940 / GetLength 0x0099B220, CSystemRegistry(const CWideString&, long) 0x0099CF10 /
//   GetString 0x0099D000 / dtor 0x0099CF70. Globals: the front-end component 0x013B871C (text bank +0x60), the registry key path
//   0x013BCA30 and value name 0x013BCA34 (CWideString globals). Literals: L"\n" 0x01249F28, L"" 0x0122D70C.
// Layout: `this` is unused; engine/CFrontEndManager.h declares no methods (as the other landed CFrontEndManager rows, a local view).
// Shape: the group's entry list is reached through a reference into a counted temporary (released, and nulled, before the
//   loop); the per-entry counted temporary is released after the text is appended; the two parts are written out in full;
//   the counted-pointer release and the vector accessors need __forceinline in this size TU; `entries[i]` returns a reference
//   (`push [eax+edi*4]`); the registry default is an implicit L"" conversion; a non-static member gives `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString { public:
    void* PStringData;
    CWideString();                                         // 0x0099AED0
    CWideString(const unsigned short* text);               // 0x0099B6B0
    CWideString(const CWideString& other);                 // 0x0099B720
    ~CWideString();                                        // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    const CWideString& operator+=(const unsigned short* text); // 0x0099B940
    long GetLength() const;                                // 0x0099B220
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
    unsigned long GetCRC() const;                          // 0x00404310
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                                // slot 1
};
struct CRefCounted {
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CRefBase : CRefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
template <class T> class CIVCountedPointer { public:
    T* Data;
    __forceinline ~CIVCountedPointer() { if (Data) { Data->ReleaseTemporary(); Data = 0; } }
    __forceinline T* operator->() const { return Data; }
};
struct CTagArray {
    unsigned long* First;
    unsigned long* Last;
    unsigned long* Capacity;
    __forceinline long size() const { return Last - First; }
    __forceinline const unsigned long& operator[](unsigned long i) const { return First[i]; }
};
struct CGroup : CRefBase {                                 // NGameText group: its entry tags at +0x1c
    unsigned char _pad08[0x1c - 8];
    CTagArray     Entries;                                 // +0x1c
};
struct CGroupEntry : CRefBase {
    CWideString GetText() const;                           // 0x009C7A40
};
struct CDataBank {                                         // NGameText::CDataBank
    unsigned long GetTagFromCRC(unsigned long crc) const;  // 0x009CC410
    CIVCountedPointer<CGroup> GetPGroup(unsigned long tag, bool load) const;            // 0x009C8890
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long tag, bool load) const;  // 0x009C9070
};
class CNewFrontendGameComponent { public:
    unsigned char _pad00[0x60];
    CDataBank*    TextBank;                                // +0x60
};
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C
class CSystemRegistry {
public:
    CSystemRegistry(const CWideString& subKey, long rootSelection);   // 0x0099CF10
    ~CSystemRegistry();                                    // 0x0099CF70
    CWideString GetString(const CWideString& valueName, const CWideString& defaultValue) const;   // 0x0099D000
private:
    void* m_vfptr_or_root;
    void* m_key;
};
extern CWideString g_RegistryProductKey;                   // 0x013BCA30
extern CWideString g_RegistryAboutValue;                   // 0x013BCA34

struct CFrontEndManager {
    CWideString GenerateAboutMessage();
};

CWideString CFrontEndManager::GenerateAboutMessage()
{
    CDataBank* bank = g_pNewFrontendGameComponent->TextBank;
    CWideString message;
    long tag = bank->GetTagFromCRC(CCharString("TEXT_GUI_MENU_ABOUT_MESSAGE_PART_1").GetCRC());
    if (tag > 0) {
        const CTagArray& entries = bank->GetPGroup(tag, true)->Entries;
        for (unsigned long i = 0; i < entries.size(); ++i) {
            message += bank->GetPGroupEntry(entries[i], true)->GetText();
            message += (const unsigned short*)L"\n";
        }
    }
    CSystemRegistry registry(g_RegistryProductKey, 1);
    CWideString value = registry.GetString(g_RegistryAboutValue, (const unsigned short*)L"");
    if (value.GetLength()) {
        message += value;
        message += (const unsigned short*)L"\n";
    }
    tag = bank->GetTagFromCRC(CCharString("TEXT_GUI_MENU_ABOUT_MESSAGE_PART_2").GetCRC());
    if (tag > 0) {
        const CTagArray& entries = bank->GetPGroup(tag, true)->Entries;
        for (unsigned long i = 0; i < entries.size(); ++i) {
            message += bank->GetPGroupEntry(entries[i], true)->GetText();
            message += (const unsigned short*)L"\n";
        }
    }
    return message;
}