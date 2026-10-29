// CTCRegionDisplay::SetCurrentAreaName(const CCharString&, float, bool, bool, bool) -- look the region tag up in the text data bank (text entry, first entry of a group, or the tag itself) and show it, unless it is already showing
// Leaf: SetCurrentAreaName
// Retail: Fable.exe (TLC, VC7.1) @ 0x00639BA0, 658 bytes, `ret 14h` (the CWideString overload 0x00639A90 is landed).
// Callees: CCharString::operator!=(const char*) const 0x0099E960 (catalog "CCharString::NotEqual"), CWideString() 0x0099AED0
//   (catalog "CCharString" -- the known CWideString/CCharString default ctor name conflict), CRC32 CCRC::Calc 0x004014A0 (fastcall),
//   CDataBank::LookupValue 0x009CC410, CBankFile::GetEntryDataType 0x009CC210, NGameText::CDataBank::GetPGroupEntry
//   0x009C9070 (the text entry), CDataBank::GetPGroupEntry 0x009C8890 (the group), the entry's text getter 0x009C7A40,
//   CCharString::ToWideString 0x0099E480, CWideString::operator=(const CWideString&) 0x0099B7D0, operator=(const wchar_t*)
//   0x0099B800, ~CWideString 0x0099B510, MemCmp_Unsigned16 0x00403C60, CTCBase::GetWorld 0x00686A70, CWorld::GetWorldFrame
//   0x0049D870 (static, called through GetWorld()).
// Layout: Ego_r.pdb CTCRegionDisplay as the landed 0x00639A90; the world's data bank at +0x14 of 0x013B86A0.
#include <stddef.h>
struct CStringData {
    char* Chars;                                            // +0x00
    long  Length;                                           // +0x04
};
class CCharString { public:
    CStringData* PStringData;
    bool operator!=(const char* text) const;                // 0x0099E960
    class CWideString ToWideString() const;                // 0x0099E480
    long GetLength() const { return !PStringData ? 0 : PStringData->Length; }
    const char* GetData() const { return !PStringData ? "" : PStringData->Chars; }
};
int __cdecl MemCmp_Unsigned16(const unsigned short* a, const unsigned short* b, unsigned long count);   // 0x00403C60
class CWideStringData { public:
    unsigned short* Begin;                                  // +0x00
    unsigned short* End;                                    // +0x04
};
class CWideString { public:
    CWideStringData* m_pData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    const CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator=(const wchar_t* text);      // 0x0099B800
    __forceinline bool operator==(const CWideString& other) const
    {
        if (m_pData == other.m_pData)
            return true;
        if (!m_pData || !other.m_pData)
            return false;
        const unsigned short* otherBegin = other.m_pData->Begin;
        const unsigned short* begin = m_pData->Begin;
        long length = m_pData->End - begin;
        if (length != other.m_pData->End - otherBegin)
            return false;
        if (MemCmp_Unsigned16(begin, otherBegin, length) != 0)
            return false;
        return true;
    }
};
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0

struct CCounted {
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CULongVector {
    unsigned long* first; unsigned long* last; unsigned long* end;
    const unsigned long& front() const { return *first; }
};
namespace NGameText {
class CGroupEntry : public CCounted { public:
    unsigned char _pad08[0x1c - 0x08];
    CULongVector Entries;                                   // +0x1c
};
class CTextEntry : public CCounted { public:
    CWideString GetText() const;                            // 0x009C7A40
};
}
using NGameText::CGroupEntry;
using NGameText::CTextEntry;
template <class T> class CIVCountedPointer { public:
    T* Data;
    __forceinline ~CIVCountedPointer()
    {
        if (Data) {
            Data->Release();
            Data = 0;
        }
    }
    T* operator->() const { return Data; }
};
class CBankFile { public:
    unsigned long GetEntryDataType(unsigned long id) const; // 0x009CC210
};
namespace NGameText {
class CDataBank : public CBankFile { public:
    unsigned long LookupValue(unsigned long crc) const;     // 0x009CC410
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long id, bool flag) const;   // 0x009C8890
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long id, bool flag) const;   // 0x009C9070
};
}
using NGameText::CDataBank;
struct CWorldGlobal {
    unsigned char _pad00[0x14];
    CDataBank* PDataBank;                                   // +0x14
};
extern CWorldGlobal* g_pWorld;                              // 0x013B86A0

class CThing;
class CWorld { public:
    static long __fastcall GetFrame();                            // 0x0049D870
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    unsigned char _pad0a[2];
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCRegionDisplay : public CTCBase { public:
    CWideString   CurrentAreaName;                          // +0x0c
    CWideString   CurrentAreaNameExtra;                     // +0x10
    long          FrameNameChanged;                         // +0x14
    long          CurrentAlpha;                             // +0x18
    long          LastAlpha;                                // +0x1c
    float         DisplayTime;                              // +0x20
    bool          TimeOut;                                  // +0x24
    bool          InfoDisplay;                              // +0x25
    bool          UpdateWhilePaused;                        // +0x26
    void SetCurrentAreaName(const CCharString& tag, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused);
};

void CTCRegionDisplay::SetCurrentAreaName(const CCharString& tag, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused)
{
    if (tag != "") {
        CWideString text;
        {
            CDataBank* bank = g_pWorld->PDataBank;
            unsigned long id = bank->LookupValue(CCRC::Calc(0, tag.GetData(), tag.GetLength()));
            if (id > 0) {
                if (bank->GetEntryDataType(id) == 0)
                    text = bank->GetPTextEntry(id, true)->GetText();
                else {
                    const CULongVector& entries = bank->GetPGroupEntry(id, true)->Entries;
                    text = bank->GetPTextEntry(entries.front(), true)->GetText();
                }
            } else
                text = tag.ToWideString();
        }
        if (infoDisplay && text == CurrentAreaName)
            return;
        CurrentAreaName = text;
        CurrentAreaNameExtra = L"";
        FrameNameChanged = GetWorld().GetFrame();
        LastAlpha = 0;
    } else {
        CurrentAreaName = L"";
        CurrentAreaNameExtra = L"";
        CurrentAlpha = 0;
    }
    if (displayTime == -1.0f)
        displayTime = 5.0f;
    if (showNow)
        CurrentAlpha = LastAlpha = 255;
    DisplayTime = displayTime;
    TimeOut = displayTime != 0.0f;
    InfoDisplay = infoDisplay;
    UpdateWhilePaused = updateWhilePaused;
}