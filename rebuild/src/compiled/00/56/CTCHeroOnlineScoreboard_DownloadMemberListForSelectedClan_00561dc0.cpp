// CTCHeroOnlineScoreboard::DownloadMemberListForSelectedClan() -- when idle, sends DOWNLOAD_MEMBER_LIST for the signed-in user, session and SelectedClan; then steps the transaction and, on success, decrypts the reply (busy unless it starts "SID:OK") and appends one CClanMember per (name, id, description) line triple to ClanMembers before dropping the connection
// Leaf: DownloadMemberListForSelectedClan
// Retail: Fable.exe @ 0x00561DC0, 1008 bytes, `ret`. Ego_r.pdb `?DownloadMemberListForSelectedClan@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@XZ`
// (the manifest label DownloadLogosForThisClansMembers is propagated; the request literal is DOWNLOAD_MEMBER_LIST and
// GlobalUpdate 0x00564570 runs it as state 6, right after ChooseAClan).
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, CWideString(const CWideString&) 0x0099B720 / () 0x0099AED0 /
// ~ 0x0099B510 / operator= 0x0099B7D0, ConvertToCharString 0x0099B2C0, CCharString(const char*, long) 0x0099EBF0 / ()
// 0x0099E4B0 / ~ 0x0099EAE0, ToWideString 0x0099E480, operator+ 0x0099F570 / 0x0099F600 (__fastcall), the std::string range
// init 0x005672D0 (malloc-backed; freed through the CRT free thunk 0x00BFEA14), ResetAndOpenHTTPAndPrepareRequest 0x0055F710,
// UpdateTransaction 0x0055FB80, GetDecryptedDocument 0x00560BB0, operator!=(const std::string&, const char*) 0x00566EA0 (the
// landed std::operator!= row: true on a mismatch), ~vector<std::string> 0x00569940,
// ReplaceDoubleApostrophes 0x0055ECB0 (__fastcall, std::string by value), GFCharStringToInt 0x0099E7F0,
// list<CClanMember>::push_back 0x00567FC0, LHHttp2::~LHHttp2 0x008410D0, operator delete 0x00BFE9BC.
// Layout: as the landed UploadPlayerScores 0x00561450 (PHttp +0x0c, TaskState +0x54), SessionID +0x34, SelectedClan +0x3c, ClanMembers +0x40.
// CClanMember (Ego_r.pdb, 0x0c): Name, MemberID, Description.
// Shape: the request is one chain GetHeader() + CCharString(command) + GetSignedInUserName() + "\r\n" + SessionID +
// "\r\n" + SelectedClan.ConvertToCharString() + "\r\n" (the two inline by-value getters are hoisted before the pushes);
// the reply lines are consumed with `lines[i++]` three times per member, the id through an implicit const char* ->
// CCharString conversion. Same-TU noinline stand-ins (GetDecryptedDocument, ~vector<std::string>,
// ReplaceDoubleApostrophes, operator!=, and std::string's range init with the allocator parameter unnamed, which lets the
// allocator temporary share the busy flag's byte) keep the locals from escaping as in retail.
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
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};
class CUserProfileManager { public:
    unsigned char _pad[0x110];
    CWideString   SignedInUserName;                  // +0x110
    CWideString   SignedInPassword;                  // +0x114
};
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
LHString __fastcall ReplaceDoubleApostrophes(const LHString& text);   // 0x0055ECB0

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

struct CClanMember {                                 // CTCHeroOnlineScoreboard::CClanMember (0x0c)
    CWideString Name;
    long        MemberID;
    CWideString Description;
    CClanMember() : MemberID(-1) { }
};
struct CMemberList {                                 // std::list<CClanMember> (retail: one node pointer)
    void* _M_node;
    void push_back(const CClanMember& x);            // 0x00567FC0
};

class CTCHeroOnlineScoreboard {
public:
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    void*          __vftable;                        // +0x00
    void*          Thing;                            // +0x04
    long           m_8;                              // +0x08
    CHttpScopedPtr PHttp;                            // +0x0c
    unsigned char  _pad10[0x34 - 0x10];
    CCharString    SessionID;                        // +0x34
    void*          MyClans;                          // +0x38
    CWideString    SelectedClan;                     // +0x3c
    CMemberList    ClanMembers;                      // +0x40
    unsigned char  _pad44[0x54 - 0x44];
    long           TaskState;                        // +0x54
    ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const LHString& request, const CCharString& key, bool flag);   // 0x0055F710
    ENetworkSuccess UpdateTransaction();             // 0x0055FB80
    int GetDecryptedDocument(LHStringVector& lines, const CCharString& key);   // 0x00560BB0
    CCharString GetSignedInUserName() { CWideString name = GetUserProfileManager()->SignedInUserName; return name.ConvertToCharString(); }
    __forceinline bool OpenRequest(const CCharString& request)
    {
        LHString text(request.c_str());
        CCharString key("THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM", -1);
        return ResetAndOpenHTTPAndPrepareRequest(text, key, false) == NETWORK_BUSY;
    }
    static CCharString GetHeader() { return CCharString("Fable1.5\r\nVersion 1\r\n", -1); }
    ENetworkSuccess DownloadMemberListForSelectedClan();
};

#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line callees that receive the reply vector's address (it never escapes).
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
__declspec(noinline) LHString __fastcall ReplaceDoubleApostrophes(const LHString& text)
{
    return LHString(text.m_pStart, text.m_pFinish, text.m_pEndOfStorage);
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

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::DownloadMemberListForSelectedClan()
{
    if (TaskState == 100 || TaskState == 101) {
        CCharString request = GetHeader() + CCharString("DOWNLOAD_MEMBER_LIST\r\n", -1) + GetSignedInUserName() + "\r\n" + SessionID + "\r\n" + SelectedClan.ConvertToCharString() + "\r\n";
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
                LHString name = ReplaceDoubleApostrophes(lines[i++]);
                long id = GFCharStringToInt(lines[i++].c_str());
                LHString description = ReplaceDoubleApostrophes(lines[i++]);
                CClanMember member;
                member.Name = CCharString(name.c_str(), -1).ToWideString();
                member.MemberID = id;
                if (description != "NO_DESCRIPTION")
                    member.Description = CCharString(description.c_str(), -1).ToWideString();
                ClanMembers.push_back(member);
            }
        }
        PHttp.reset();
    }
    return result;
}