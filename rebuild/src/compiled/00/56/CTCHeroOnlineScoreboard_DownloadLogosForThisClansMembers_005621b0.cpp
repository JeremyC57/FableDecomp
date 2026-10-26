// CTCHeroOnlineScoreboard::DownloadLogosForThisClansMembers() -- when idle, sends DOWNLOAD_MEMBER_LOGOS (user, session and the cached member logo list); then steps the transaction and, on success, decrypts the reply (busy unless it starts "SID:OK") and writes each (clan id, logo id, size, data) quadruple to SaveDir\ClanData\<id>\logo.<id>.jpg before dropping the connection
// Leaf: DownloadLogosForThisClansMembers
// Retail: Fable.exe @ 0x005621B0, 1088 bytes, `ret`. Ego_r.pdb `?DownloadLogosForThisClansMembers@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@XZ`.
// Byte-twin of DownloadMyClansLogos 0x00561980 except the cached-logo getter (GetCachedMemberLogosAsString 0x0055EA80) and the
// command literal; same source shape.
// Callees: as the landed DownloadMemberListForSelectedClan 0x00561DC0, plus GetCachedMemberLogosAsString 0x0055EA80, GetSaveDir
// 0x0041A5B0, operator+(const CWideString&, const wchar16*) 0x0099BF30, CWideString() 0x0099AED0, FormatWideString 0x0099BA70
// (cdecl varargs), CWideString::operator+= 0x0099B8D0 / 0x0099B940, CDiskFileWin32(path, access, flags) 0x0099AD80, Write
// 0x0099AA90 and the inline ~CDiskFileWin32 (vtable 0x0122D06C, Close 0x0099A920, ~CBase 0x0099A300).
// Layout: as the landed UploadPlayerScores 0x00561450 (PHttp +0x0c, SessionID +0x34, TaskState +0x54).
// Shape: the reply numbers go through the IMPLICIT const char* -> CCharString conversion (an explicit CCharString(..., -1)
// temporary passes the ctor result in ecx and reverses the request chain's temp slots); the data line is read with lines[i]
// and i is bumped after Write; the int -> wide text helper returns a named local (copied, no NRVO).
typedef unsigned short wchar16;
extern "C" void __cdecl free(void* block);
extern "C" unsigned int __cdecl strlen(const char* text);
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strlen)

class CWideString;
class CCharStringData { public: char* m_pString; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                   // 0x0099E4B0
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    CWideString ToWideString() const;                // 0x0099E480
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->m_pString; }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
long __fastcall GFCharStringToInt(const CCharString& text);                            // 0x0099E7F0
class CWideString { public:
    void* PStringData;
    CWideString();                                   // 0x0099AED0
    CWideString(const CWideString& other);           // 0x0099B720
    ~CWideString();                                  // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    const CWideString& operator+=(const wchar16* text);        // 0x0099B940
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};
class CUserProfileManager { public:
    unsigned char _pad[0x110];
    CWideString   SignedInUserName;                  // +0x110
    CWideString   SignedInPassword;                  // +0x114
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);   // 0x0099BF30
CWideString __fastcall GetSaveDir();                 // 0x0041A5B0
void __cdecl FormatWideString(CWideString* text, const wchar16* format, ...);       // 0x0099BA70
CUserProfileManager* GetUserProfileManager();        // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)

struct CStringAllocator { CStringAllocator() { } };
struct LHString {                                    // PDB std::basic_string<char> (12 bytes, malloc-backed)
    char* m_pStart;
    char* m_pFinish;
    char* m_pEndOfStorage;
    LHString(const char* text, const CStringAllocator& a = CStringAllocator())
        : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0)
    {
        Initialise(text, text + strlen(text), a);
    }
    LHString(char* start, char* finish, char* end) : m_pStart(start), m_pFinish(finish), m_pEndOfStorage(end) { }
    ~LHString() { if (m_pStart) free(m_pStart); }
    const char* c_str() const { return m_pStart; }
    void Initialise(const char* first, const char* last, const CStringAllocator& a);   // 0x005672D0
};
bool operator!=(const LHString& left, const char* right);    // 0x00566EA0

struct LHStringVector {                              // vector<std::string>
    LHString* m_pStart;
    LHString* m_pFinish;
    LHString* m_pEndOfStorage;
    LHStringVector() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    ~LHStringVector();                               // 0x00569940
    LHString& operator[](int i) { return m_pStart[i]; }
};

namespace LHNetworkLib {
class LHHttp2 { public:
    ~LHHttp2();                                      // 0x008410D0
};
}
struct CHttpScopedPtr {
    LHNetworkLib::LHHttp2* px;
    void reset(LHNetworkLib::LHHttp2* p = 0)
    {
        LHNetworkLib::LHHttp2* old = px;
        px = p;
        delete old;
    }
};


namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A300
}; } using N_0099a300::CBase;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBase { public:                     // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                           // 0x0099A920 (slot 2)
    long        AccessType;                         // +0x04
    long        Position;                           // +0x08
    long        Length;                             // +0x0c
    void*       WinFileHandle;                      // +0x10
    bool        Opened;                             // +0x14
    CWideString Pathname;                           // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    void Write(const void* data, long size, CASuspendableProcess* process);   // 0x0099AA90
};
static inline CWideString IntToWideString(long value)
{
    CWideString text;
    FormatWideString(&text, L"%d", value);
    return text;
}

class CTCHeroOnlineScoreboard {
public:
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    void*          __vftable;                        // +0x00
    void*          Thing;                            // +0x04
    long           m_8;                              // +0x08
    CHttpScopedPtr PHttp;                            // +0x0c
    unsigned char  _pad10[0x34 - 0x10];
    CCharString    SessionID;                        // +0x34
    unsigned char  _pad38[0x54 - 0x38];
    long           TaskState;                        // +0x54
    ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const LHString& request, const CCharString& key, bool flag);   // 0x0055F710
    ENetworkSuccess UpdateTransaction();             // 0x0055FB80
    int GetDecryptedDocument(LHStringVector& lines, const CCharString& key);   // 0x00560BB0
    CCharString GetCachedMemberLogosAsString();      // 0x0055EA80
    CCharString GetSignedInUserName() { CWideString name = GetUserProfileManager()->SignedInUserName; return name.ConvertToCharString(); }
    __forceinline bool OpenRequest(const CCharString& request)
    {
        LHString text(request.c_str());
        CCharString key("THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM", -1);
        return ResetAndOpenHTTPAndPrepareRequest(text, key, false) == NETWORK_BUSY;
    }
    static CCharString GetHeader() { return CCharString("Fable1.5\r\nVersion 1\r\n", -1); }
    ENetworkSuccess DownloadLogosForThisClansMembers();
};

#pragma auto_inline(off)
__declspec(noinline) int CTCHeroOnlineScoreboard::GetDecryptedDocument(LHStringVector& lines, const CCharString& key)
{
    if (key.PStringData == 0)
        lines.m_pFinish = lines.m_pStart;
    return (int)(lines.m_pFinish - lines.m_pStart);
}
__declspec(noinline) LHStringVector::~LHStringVector()
{
    for (LHString* p = m_pStart; p != m_pFinish; ++p)
        p->~LHString();
    if (m_pStart)
        free(m_pStart);
}
__declspec(noinline) bool operator!=(const LHString& left, const char* right)
{
    return strcmp(left.c_str(), right) != 0;
}
__declspec(noinline) void LHString::Initialise(const char* first, const char* last, const CStringAllocator&)
{
    m_pStart = (char*)first;
    m_pFinish = (char*)last;
    m_pEndOfStorage = (char*)last;
}
#pragma auto_inline(on)

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::DownloadLogosForThisClansMembers()
{
    if (TaskState == 100 || TaskState == 101) {
        CCharString request = GetHeader() + CCharString("DOWNLOAD_MEMBER_LOGOS\r\n", -1) + GetSignedInUserName() + "\r\n" + SessionID + "\r\n" + GetCachedMemberLogosAsString();
        bool busy = OpenRequest(request);
        if (busy)
            return NETWORK_BUSY;
    }
    ENetworkSuccess result = UpdateTransaction();
    if (result == NETWORK_SUCCESS) {
        LHStringVector lines;
        int count;
        {
            CCharString key("THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM", -1);
            count = GetDecryptedDocument(lines, key);
        }
        if (count > 0) {
            if (lines[0] != "SID:OK")
                return NETWORK_BUSY;
            for (int i = 1; i < count; ) {
                long clanId = GFCharStringToInt(lines[i++].c_str());
                long logoId = GFCharStringToInt(lines[i++].c_str());
                long size = GFCharStringToInt(lines[i++].c_str());
                CWideString path = GetSaveDir() + L"ClanData\\";
                path += IntToWideString(clanId);
                path += L"\\logo.";
                path += IntToWideString(logoId);
                path += L".jpg";
                CDiskFileWin32 file(path, FILE_ACCESS_WRITE, 2);
                file.Write(lines[i].c_str(), size, 0);
                i++;
            }
        }
        PHttp.reset();
    }
    return result;
}