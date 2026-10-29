// CTCHeroOnlineScoreboard::GetCachedMemberLogosAsString() -- "<member count>\r\n" then, per member in ClanMembers, "<id>\r\n<cached logo extension or 0>\r\n" (extension from the first SaveDir\MemberData\<id>\logo.* file)
// Leaf: GetCachedMemberLogosAsString
// Flags: /GS
// Retail: Fable.exe @ 0x0055EA80, 560 bytes, `ret 4` (hidden CCharString result), /GS cookie (the find data holds a
// wchar_t[260] name). Ego_r.pdb `?GetCachedMemberLogosAsString@CTCHeroOnlineScoreboard@@AAE?AVCCharString@@XZ`
// (the manifest labels this row GetCachedClanLogosAsString, propagated from the 0x0055E850 twin; it walks ClanMembers).
// Callees: CCharString() 0x0099E4B0, GFIntToCharString 0x0099F830 (__fastcall hidden result), CCharString::operator+=
// (const CCharString&) 0x0099F0A0 / (const char*) 0x0099F100, ~CCharString 0x0099EAE0, GetSaveDir 0x0041A5B0 (__fastcall
// hidden CWideString result), operator+(const CWideString&, const wchar_t*) 0x0099BF30, ~CWideString 0x0099B510,
// CCharString::ToWideString 0x0099E480, CWideString::operator+= 0x0099B8D0 / 0x0099B940, CCharString(const char*, long)
// 0x0099EBF0, CAFile::StartFileScan 0x00999760, CWideString(const wchar_t*) 0x0099B6B0, CWideString::GetLength 0x0099B220,
// CWideString::ConvertToCharString 0x0099B2C0, CCharString::GetSubString 0x0099EC70, CCharString::operator= 0x0099EFB0,
// CCharString(const CCharString&) 0x0099EC30.
// Layout: retail MyClans (STLport list, one node pointer) +0x38, ClanMembers +0x40, TaskState +0x54 (Ego_r.pdb +0x44 /
// +0x54 / +0x74 less the 16->12-byte map/vector and 12->4-byte list shrink; landed UploadPlayerScores has TaskState +0x54).
// CClanMember's id is its second dword (node +0x0c).
// Shape: list size() is the inline node-count walk; CCharString::GetLength is inline (`data ? data->Length : 0`).
#include <stddef.h>
typedef unsigned short wchar16;

class CCharStringData { public:
    char* m_pString;
    long  Length;
};
class CWideString;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                   // 0x0099E4B0
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    const CCharString& operator+=(const char* text);       // 0x0099F100
    CWideString ToWideString() const;                // 0x0099E480
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString(const wchar16* text);                // 0x0099B6B0
    ~CWideString();                                  // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    const CWideString& operator+=(const wchar16* text);        // 0x0099B940
    long GetLength() const;                          // 0x0099B220
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);   // 0x0099BF30
CCharString __fastcall GFIntToCharString(long value);                              // 0x0099F830
CWideString __fastcall GetSaveDir();                                               // 0x0041A5B0

struct CFindFileData {
    wchar16       FileName[260];
    unsigned long Attributes;
};
class CAFile {
public:
    class CFileScanInfo {
    public:
        bool Stopped;                                   // +0x00
        long SearchHandle;                              // +0x04
    };
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar16* firstFile);   // 0x00999760
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CList {
    SListNode<T>* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SListNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};

class CTCHeroOnlineScoreboard {
public:
    class CClanInfo { public:
        CWideString Name;
        long        ID;
    };
    class CClanMember { public:
        CWideString Name;
        long        ID;
    };
    unsigned char           _pad00[0x38];
    CList<CClanInfo>        MyClans;                 // +0x38
    CWideString             SelectedClan;            // +0x3c
    CList<CClanMember>      ClanMembers;             // +0x40
    CCharString GetCachedMemberLogosAsString();
};

CCharString CTCHeroOnlineScoreboard::GetCachedMemberLogosAsString()
{
    CCharString result;
    result += GFIntToCharString(ClanMembers.size());
    result += "\r\n";
    for (SListNode<CClanMember>* it = ClanMembers._M_node->_M_next; it != ClanMembers._M_node; it = it->_M_next) {
        result += GFIntToCharString(it->_M_data.ID);
        result += "\r\n";
        CWideString path = GetSaveDir() + (const wchar16*)L"MemberData\\";
        path += GFIntToCharString(it->_M_data.ID).ToWideString();
        path += (const wchar16*)L"\\logo.*";
        CCharString extension("0", -1);
        CAFile::CFileScanInfo handle;
        CFindFileData data;
        if (CAFile::StartFileScan(path, handle, data.FileName)) {
            CWideString name(data.FileName);
            long start = path.GetLength() - 1;
            CCharString file = name.ConvertToCharString().GetSubString(start, name.GetLength());
            extension = file.GetSubString(0, file.GetLength() - 4);
        }
        result += extension;
        result += "\r\n";
    }
    return result;
}