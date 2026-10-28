// CGameScriptInterface::AddScreenMessage(const CCharString& textTag, ETextGroupSelectionMethod method) const -- look the tag up in the game text bank (by CRC): a text entry's string is shown as a screen message; for a group entry only the random pick is drawn; an unknown tag shows "(" + tag
// Leaf: AddScreenMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00892850, length 383 bytes (`ret 8`).
// Callees: CWideString ctor/dtor/operator=/operator!= @ 0x0099AED0 / 0x0099B510 / 0x0099B7D0 / 0x0099B150, CCRC::Calc
//   @ 0x004014A0 (`__fastcall`), the text bank's CRC lookup @ 0x009CC410, CBankFile::GetEntryDataType @ 0x009CC210,
//   NGameText::CDataBank::GetPTextEntry / GetPGroupEntry (intrusive counted pointers by value) @ 0x009C9070 / 0x009C8890,
//   the text entry's text @ 0x009C7A40, the world's random seed @ 0x0049D810, GFRandom @ 0x004CDAE0, CCharString::ToWideString
//   @ 0x0099E480, operator+(const wchar_t*, const CWideString&) @ 0x0099BFF0, CPlayerGui::AddScreenMessage @ 0x00438AA0.
// Globals: the engine 0x013B86A0 (text bank +0x14), the player GUI 0x013B8790.
// Shape: the text and group entries are the landed SetQuestInfoText 0x00891A00 model (CRefVirtual + CRefCounted bases,
//   inline ReleaseTemporary = DecRef / GetRefCount / Destroy, the `mov esi,[eax+4] ; ... ; mov [eax],esi` release);
//   CIVCountedPointer's destructor zeroes Data inside its null test. Both entry pointers are TEMPORARIES of their full
//   expressions: the text path's zero store dies with the temporary, the group path keeps a pointer to the entry list
//   (`&bank->GetPGroupEntry(..)->Entries`, read after the release exactly as retail) and its zero store survives.
typedef unsigned short wchar_fable;
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator!=(const wchar_fable* text) const;     // 0x0099B150
};
CWideString __fastcall operator+(const wchar_fable* left, const CWideString& right);   // 0x0099BFF0
class CCharStringData { public: char* PData; unsigned long StringLength; };
class CCharString { public:
    CCharStringData* PStringData;
    const char* GetData() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    CWideString ToWideString() const;                   // 0x0099E480
};
struct CCRC { static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
struct CRefCounted {
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
struct CRefCountedEntry : CRefVirtual, CRefCounted {
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template<class T> class CIVCountedPointer { public:
    T* Data;
    ~CIVCountedPointer() { if (Data) { Data->ReleaseTemporary(); Data = 0; } }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CTextEntry : public CRefCountedEntry { public: CWideString GetText() const; };          // 0x009C7A40
}
using NGameText::CTextEntry;
template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
namespace NGameText {
class CGroupEntry : public CRefCountedEntry { public:
    unsigned char _pad08[0x14];
    CRetailVector<long> Entries;                        // +0x1C
};
}
using NGameText::CGroupEntry;
class CBankFile { public:
    unsigned long GetEntryDataType(unsigned long entry) const;   // 0x009CC210
};
namespace NGameText {
class CDataBank : public CBankFile { public:
    unsigned long LookupValue(unsigned long crc) const; // 0x009CC410
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long entry, bool b) const;     // 0x009C9070
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long entry, bool b) const;   // 0x009C8890
};
}
using NGameText::CDataBank;
struct CEngine_Retail { unsigned char _pad[0x14]; CDataBank* TextBank; };
extern CEngine_Retail* g_pEngine;                       // 0x013B86A0
class CPlayerGui { public: void AddScreenMessage(const CWideString& text); };   // 0x00438AA0
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
struct CWorld_Retail { unsigned long* GetRandomSeed(); };   // 0x0049D810
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECT_FIRST, TEXT_GROUP_SELECT_RANDOM };
class CGameScriptInterface { public:
    virtual void AddScreenMessage(const CCharString& textTag, ETextGroupSelectionMethod method) const;
    CWorld_Retail* World;                               // +0x04
};

void CGameScriptInterface::AddScreenMessage(const CCharString& textTag, ETextGroupSelectionMethod method) const
{
    CWideString text;
    CDataBank* bank = g_pEngine->TextBank;
    long entry = bank->LookupValue(CCRC::Calc(0, textTag.GetData(), textTag.Length()));
    if (entry) {
        if (bank->GetEntryDataType(entry) == 0) {
            text = bank->GetPTextEntry(entry, true)->GetText();
        } else {
            const CRetailVector<long>* entries = &bank->GetPGroupEntry(entry, true)->Entries;
            unsigned long count = entries->size();
            if (count > 1 && method)
                GFRandom(count, World->GetRandomSeed());
        }
    } else
        text = L"(" + textTag.ToWideString();
    if (text != L"")
        g_pPlayerGui->AddScreenMessage(text);
}