// CTCHeroOnlineScoreboard::CreateStringFromMyScores() -- builds the upload document: header lines, the signed-in user name and session ID, then per locally recorded activity its id, "LIB"/"HIB" (lower/higher is better) and the best score to 6 digits, one item per line
// Leaf: CreateStringFromMyScores
// Retail: Fable.exe @ 0x0055F480, 549 bytes, `ret 4` (hidden CCharString result), `and esp,-8` frame (double argument).
// Ego_r.pdb `?CreateStringFromMyScores@CTCHeroOnlineScoreboard@@AAE?AVCCharString@@XZ`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 / ~ 0x0099EAE0, operator+(const
// CCharString&, const char*) 0x0099F600 and (const CCharString&, const CCharString&) 0x0099F570 (__fastcall),
// CCharString::operator+= 0x0099F0A0, CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, CWideString(const
// CWideString&) 0x0099B720 / ~ 0x0099B510, CWideString::ConvertToCharString 0x0099B2C0, GFIntToCharString 0x0099F830,
// map<EOnlineScoreboardActivity,float>::operator[] 0x005692B0, CCharString::FormatDouble(long, double) 0x009A08B0.
// Layout: retail LocalScoreboard +0x1c (12-byte STLport map, as landed UpdateScoreForActivity 0x0055F3B0), ActivityInfo
// +0x28 (vector of 16-byte CActivity, LowerIsBetter at +0x0d), SessionID +0x34; map node key +0x10.
// Shape: GetSignedInUserName is the inline by-value getter (named CWideString copy, then ConvertToCharString); the map
// walk uses STLport's inline iterator increment; the key is copied into a named local whose address operator[] takes.
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);           // 0x0099B720
    ~CWideString();                                  // 0x0099B510
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};
class CUserProfileManager { public:
    unsigned char _pad[0x110];
    CWideString   SignedInUserName;                  // +0x110
    CWideString   SignedInPassword;                  // +0x114
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};

enum EOnlineScoreboardActivity { ONLINE_SCOREBOARD_ACTIVITY_0 };

struct SScoreNode {
    long                      _M_color;
    SScoreNode*               _M_parent;
    SScoreNode*               _M_left;
    SScoreNode*               _M_right;
    EOnlineScoreboardActivity first;                 // +0x10
    float                     second;                // +0x14
};
struct CScoreMap {                                   // std::map<EOnlineScoreboardActivity, float> (12 bytes)
    SScoreNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    float& operator[](const EOnlineScoreboardActivity& key);       // 0x005692B0
};
static inline SScoreNode* Increment(SScoreNode* node)
{
    if (node->_M_right != 0) {
        node = node->_M_right;
        while (node->_M_left != 0)
            node = node->_M_left;
    } else {
        SScoreNode* y = node->_M_parent;
        while (node == y->_M_right) {
            node = y;
            y = y->_M_parent;
        }
        if (node->_M_right != y)
            node = y;
    }
    return node;
}

struct CActivity {                                   // CTCHeroOnlineScoreboard::CActivity (16 bytes)
    unsigned char _pad00[0x0d];
    bool          LowerIsBetter;                     // +0x0d
    unsigned char _pad0e[2];
};
struct CActivityVector {
    CActivity* _M_start;
    CActivity* _M_finish;
    CActivity* _M_end_of_storage;
    CActivity& operator[](unsigned long n) { return _M_start[n]; }
};

class CTCHeroOnlineScoreboard {
public:
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x1c - 0x04];
    CScoreMap       LocalScoreboard;                 // +0x1c
    CActivityVector ActivityInfo;                    // +0x28
    CCharString     SessionID;                       // +0x34
    CCharString GetSignedInUserName() { CWideString name = CUserProfileManager::Get().SignedInUserName; return name.ConvertToCharString(); }
    CCharString CreateStringFromMyScores();
};

CCharString CTCHeroOnlineScoreboard::CreateStringFromMyScores()
{
    CCharString result = operator+("Fable1.5\r\nVersion 1\r\n", "UPLOAD_SCORES\r\n");
    result += GetSignedInUserName() + "\r\n" + SessionID + "\r\n";
    for (SScoreNode* it = LocalScoreboard._M_header->_M_left; it != LocalScoreboard._M_header; it = Increment(it)) {
        long activity = it->first;
        result += GFIntToCharString(activity) + "\r\n";
        result += CCharString(ActivityInfo[activity].LowerIsBetter ? "LIB" : "HIB", -1) + "\r\n";
        result += CCharString::FormatDouble(6, LocalScoreboard[(EOnlineScoreboardActivity)activity]) + "\r\n";
    }
    return result;
}