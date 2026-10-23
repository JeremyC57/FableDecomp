// CTCHeroOnlineScoreboard::DownloadScoreboardForSelectedClan() -- when idle, sends DOWNLOAD_SCOREBOARD (user, session, SelectedClan); then steps the transaction and, on success, decrypts the reply (busy unless it starts "SID:OK"), buckets each (activity, score, name, extra) line quadruple into one of 39 per-activity (score, name) vectors, reverses the lower-is-better buckets and copies every bucket into DownloadedClanScoreboard before dropping the connection
// Grade: DIFFER(1178v1178) -- frame, flow, callees and slots as retail; one residue: the two inlined std::string copies load start into eax and lea the tag into edx (retail edx / eax); tried 6 decl orders, named vs temp tag, const char*, aliases, forceinline _M_range_initialize (advisor), template stand-in, range ctor at call site (~25 grades)
// Leaf: DownloadScoreboardForSelectedClan
// Retail: Fable.exe @ 0x00563480, 1178 bytes, `ret`. Ego_r.pdb `?DownloadScoreboardForSelectedClan@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@XZ`.
// Callees: as the landed DownloadMemberListForSelectedClan 0x00561DC0, plus GFCharStringToFloat 0x0099E690 (__fastcall),
// std::string copy-range init 0x00567D00, vector<pair<float,CWideString>>() 0x00567EA0 (through the `vector constructor
// iterator' 0x004038C0), push_back 0x005699E0, operator= 0x00568D40, STLport __reverse(first, last, const
// random_access_iterator_tag&) 0x005671C0 (__fastcall), map<EOnlineScoreboardActivity, vector>::operator[] 0x0056A680,
// CWideString::operator= 0x0099B7D0.
// Layout: retail PHttp +0x0c, ActivityInfo +0x28 (16-byte CActivity, LowerIsBetter +0x0d), SessionID +0x34, SelectedClan
// +0x3c, DownloadedClanScoreboard +0x44 (12-byte STLport map), TaskState +0x54 (12-byte containers; the PDB view is 16).
typedef unsigned short wchar16;
extern "C" void __cdecl free(void* block);
extern "C" unsigned int __cdecl strlen(const char* text);
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strlen)

struct CWideString;
struct CCharStringData { char* m_pString; };
struct CCharString {
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
float __fastcall GFCharStringToFloat(const CCharString& text);                         // 0x0099E690
struct CWideString {
    void* PStringData;
    CWideString();                                   // 0x0099AED0
    CWideString(const CWideString& other);           // 0x0099B720
    ~CWideString();                                  // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};
struct CUserProfileManager {
    unsigned char _pad[0x110];
    CWideString   SignedInUserName;                  // +0x110
    CWideString   SignedInPassword;                  // +0x114
};
CUserProfileManager* GetUserProfileManager();        // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)

struct CStringAllocator { CStringAllocator() { } };
struct forward_iterator_tag { forward_iterator_tag() { } };
struct LHString {                                    // PDB std::basic_string<char> (12 bytes, malloc-backed)
    char* m_pStart;
    char* m_pFinish;
    char* m_pEndOfStorage;
    LHString(const char* text, const CStringAllocator& a = CStringAllocator())
        : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0)
    {
        Initialise(text, text + strlen(text), a);
    }
    LHString(const LHString& other)
    {
        forward_iterator_tag tag;
        char* first = other.m_pStart;
        char* last = other.m_pFinish;
        m_pStart = 0;
        m_pFinish = 0;
        m_pEndOfStorage = 0;
        InitialiseRange(first, last, tag);
    }
    ~LHString() { if (m_pStart) free(m_pStart); }
    const char* c_str() const { return m_pStart; }
    void Initialise(const char* first, const char* last, const CStringAllocator& a);   // 0x005672D0
    void InitialiseRange(char* first, char* last, const forward_iterator_tag& tag);          // 0x00567D00
};
bool operator==(const LHString& left, const char* right);    // 0x00566EA0

struct LHStringVector {                              // vector<std::string>
    LHString* m_pStart;
    LHString* m_pFinish;
    LHString* m_pEndOfStorage;
    LHStringVector() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    ~LHStringVector();                               // 0x00569940
    LHString& operator[](int i) { return m_pStart[i]; }
};

namespace LHNetworkLib {
struct LHHttp2 {
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

struct CScorePair {                                  // std::pair<float, CWideString>
    float       first;
    CWideString second;
    CScorePair() : first(0), second() { }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
void __fastcall ReverseScores(CScorePair* first, CScorePair* last, const random_access_iterator_tag& tag);   // 0x005671C0
inline void DestroyRange(CScorePair* first, CScorePair* last)
{
    for (; first != last; ++first)
        first->~CScorePair();
}
struct CScoreVector {                                // vector<pair<float, CWideString>> (12 bytes)
    CScorePair* m_pStart;
    CScorePair* m_pFinish;
    CScorePair* m_pEndOfStorage;
    CScoreVector();                                  // 0x00567EA0
    ~CScoreVector()
    {
        DestroyRange(m_pStart, m_pFinish);
        if (m_pStart)
            free(m_pStart);
    }
    void push_back(const CScorePair& x);             // 0x005699E0
    CScoreVector& operator=(const CScoreVector& other);   // 0x00568D40
    bool empty() const { return m_pStart == m_pFinish; }
};

struct CCharString;
struct CActivity {                                   // CTCHeroOnlineScoreboard::CActivity (16 bytes)
    CCharString   Name;
    CCharString   Description;
    CCharString   UnitOfMeasurement;
    unsigned char NumDecPlacesToShow;
    bool          LowerIsBetter;
};
struct CActivityVector {
    CActivity* _M_start;
    CActivity* _M_finish;
    CActivity* _M_end_of_storage;
    CActivity& operator[](unsigned long n) { return _M_start[n]; }
};
enum EOnlineScoreboardActivity { ACTIVITY_COUNT = 39 };
struct CScoreboardMap {                              // map<EOnlineScoreboardActivity, vector<pair<float, CWideString>>> (12 bytes)
    void* _M_header[3];
    CScoreVector& operator[](const EOnlineScoreboardActivity& key);   // 0x0056A680
};

class CTCHeroOnlineScoreboard {
public:
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    void*          __vftable;                        // +0x00
    void*          Thing;                            // +0x04
    long           m_8;                              // +0x08
    CHttpScopedPtr PHttp;                            // +0x0c
    unsigned char  _pad10[0x28 - 0x10];
    CActivityVector ActivityInfo;                    // +0x28
    CCharString    SessionID;                        // +0x34
    void*          MyClans;                          // +0x38
    CWideString    SelectedClan;                     // +0x3c
    void*          ClanMembers;                      // +0x40
    CScoreboardMap DownloadedClanScoreboard;         // +0x44
    long           ScoreboardState;                  // +0x50
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
    ENetworkSuccess DownloadScoreboardForSelectedClan();
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
__declspec(noinline) bool operator==(const LHString& left, const char* right)
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

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::DownloadScoreboardForSelectedClan()
{
    if (TaskState == 100 || TaskState == 101) {
        CCharString request = GetHeader() + CCharString("DOWNLOAD_SCOREBOARD\r\n", -1) + GetSignedInUserName() + "\r\n" + SessionID + "\r\n" + SelectedClan.ConvertToCharString() + "\r\n";
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
            if (lines[0] == "SID:OK")
                return NETWORK_BUSY;
            CScoreVector scores[ACTIVITY_COUNT];
            for (int i = 1; i < count; ) {
                long activity = GFCharStringToInt(lines[i++].c_str());
                float score = GFCharStringToFloat(lines[i++].c_str());
                LHString name = lines[i++];
                LHString extra = lines[i++];
                CScorePair entry;
                entry.first = score;
                entry.second = CCharString(name.c_str(), -1).ToWideString();
                scores[activity].push_back(entry);
            }
            for (long a = 0; a < ACTIVITY_COUNT; ++a) {
                if (!scores[a].empty() && ActivityInfo[a].LowerIsBetter)
                    ReverseScores(scores[a].m_pStart, scores[a].m_pFinish, random_access_iterator_tag());
                DownloadedClanScoreboard[(EOnlineScoreboardActivity)a] = scores[a];
            }
        }
        PHttp.reset();
    }
    return result;
}
