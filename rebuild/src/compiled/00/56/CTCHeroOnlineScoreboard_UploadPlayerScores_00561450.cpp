// CTCHeroOnlineScoreboard::UploadPlayerScores() -- when idle or beginning, opens the HTTP request carrying this hero's score string (busy if that is still pending); then steps the transaction and, on success, checks the decrypted reply starts with "SID:OK" (else drops the connection)
// Leaf: UploadPlayerScores
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00561450, 363 bytes (`ret`). Ego_r.pdb
// `?UploadPlayerScores@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@XZ`.
// Callees: CreateStringFromMyScores @ 0x0055F480 (name from the PDB method list; the by-value CCharString getter),
// the reply string's range init @ 0x005672D0 (malloc-backed 12-byte std::basic_string<char>), CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, ResetAndOpenHTTPAndPrepareRequest(const std::string&, const CCharString&, bool)
// @ 0x0055F710, UpdateTransaction @ 0x0055FB80, GetDecryptedDocument(vector<std::string>&, const CCharString&) @ 0x00560BB0,
// operator==(const std::string&, const char*) @ 0x00566EA0, ~vector<std::string> @ 0x00569940, LHHttp2::~LHHttp2 @
// 0x008410D0, operator delete @ 0x00BFE9BC; the string's inline dtor frees through the CRT free thunk @ 0x00BFEA14.
// Layout: PHttp (scoped_ptr<LHHttp2>) +0x0c, TaskState +0x54, as in the landed UpdateTransaction 0x0055FB80.
// Shape: the request is opened through a force-inlined helper taking the score string by const reference (descriptive name,
// not a PDB method): the by-value CreateStringFromMyScores result is read through the returned pointer (`mov eax,[eax]`)
// and outlives the helper's locals, so the destructors run key, request string, score string before `if (busy) return
// busy` (flag in bl, zero in ebp). Each key is a named local (built before the argument pushes; the reply key in its own
// block so it dies before `count > 0`). The allocator has a user ctor (an empty aggregate default arg stores a byte);
// "" is the `data == 0 ? "" : data->str` form (constant loaded before the branch); strlen is the /Oi intrinsic.
extern "C" void __cdecl free(void* block);
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)

struct CCharStringData { char* m_pString; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->m_pString; }
};

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
    ~LHString() { if (m_pStart) free(m_pStart); }
    void Initialise(const char* first, const char* last, const CStringAllocator& a);   // 0x005672D0
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

struct CTCHeroOnlineScoreboard {
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    void*          __vftable;                        // +0x00
    void*          ParentThing;                      // +0x04
    long           m_8;                              // +0x08
    CHttpScopedPtr PHttp;                            // +0x0c
    unsigned char  _pad10[0x54 - 0x10];
    long           TaskState;                        // +0x54
    CCharString CreateStringFromMyScores();          // 0x0055F480
    ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const LHString& request, const CCharString& key, bool flag);   // 0x0055F710
    ENetworkSuccess UpdateTransaction();             // 0x0055FB80
    int GetDecryptedDocument(LHStringVector& lines, const CCharString& key);   // 0x00560BB0
    ENetworkSuccess UploadPlayerScores();
    __forceinline bool OpenUploadRequest(const CCharString& scores)
    {
        LHString request(scores.c_str());
        CCharString key("THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM", -1);
        return ResetAndOpenHTTPAndPrepareRequest(request, key, false) == NETWORK_BUSY;
    }
};

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::UploadPlayerScores()
{
    if (TaskState == 100 || TaskState == 101) {
        bool busy = OpenUploadRequest(CreateStringFromMyScores());
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
        if (count > 0 && lines[0] == "SID:OK")
            return NETWORK_BUSY;
        PHttp.reset();
    }
    return result;
}