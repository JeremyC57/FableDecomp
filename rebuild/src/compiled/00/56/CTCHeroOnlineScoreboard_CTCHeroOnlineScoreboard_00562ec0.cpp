// CTCHeroOnlineScoreboard::CTCHeroOnlineScoreboard(CThing* thing) -- online-scoreboard component constructor: no HTTP connection, empty picture cache / local scoreboard / activity table / clan lists / downloaded scoreboard, idle states (task state 100), no screens; sets the two static scoreboard strings, registers the component and fills the activity table
// Leaf: ??0CTCHeroOnlineScoreboard
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00562EC0, length 291 bytes (`ret 4`). Vtable 0x0124DB34 (CTCHeroOnlineScoreboard).
// Label correction: the manifest calls this "CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite" (propagated); the
// vtable store, the CTCBase(CThing*) base call and the tail call of CTCHeroOnlineScoreboard::InitialiseActivityInfo pin it.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (map headers 0x1c/0x18/0x20, list nodes 0x1c/0x14),
// CCharString::CCharString() @ 0x0099E4B0, CWideString::CWideString() @ 0x0099AED0, CCharString::operator=(const char*) @ 0x0099EFE0
// (on the statics @ 0x013B8AF0 / 0x013B8AF4: "fableclangw.lionhead.com" @ 0x0124DB14, "/index.php" @ 0x0124DB08), CTCBase member @ 0x00686ED0 (manifest
// "std::list::push_back" is propagated; descriptive RegisterComponent), InitialiseActivityInfo @ 0x00562710 (labelled there).
// Layout: PDB CTCHeroOnlineScoreboard with retail 12-byte maps/vectors and 4-byte lists (as the landed ResetScoreboard 0x00562E70):
// PHttp +0x0c, PictureCache +0x10, LocalScoreboard +0x1c, ActivityInfo +0x28, SessionID +0x34, MyClans +0x38, SelectedClan +0x3c,
// ClanMembers +0x40, DownloadedClanScoreboard +0x44, ScoreboardState +0x50, TaskState +0x54, the four component pointers +0x58..+0x64.
// Shape: the STLport container models of the landed CTCObjectAugmentations ctor 0x007673C0 (_Rb_tree_base allocating its header in
// the body, _Rb_tree zeroing the count then initialising the header through the re-read member; _Vector_base with the alloc proxy);
// the STLport list constructor builds its sentinel in a local before storing it; one `xor ebx,ebx` zero register throughout.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
};

template <class P> struct CAllocProxy {                 // STLport _STLP_alloc_proxy<P, ...> over an empty allocator
    P _M_data;
    CAllocProxy(P p) : _M_data(p) { }
};
template <class T> struct vector {                      // STLport vector: _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct SRbTreeNodeBase {                                // STLport _Rb_tree_node_base
    bool             _M_color;                          // +0x00
    SRbTreeNodeBase* _M_parent;                         // +0x04
    SRbTreeNodeBase* _M_left;                           // +0x08
    SRbTreeNodeBase* _M_right;                          // +0x0c
};
template <int SIZE> struct STreeNode : SRbTreeNodeBase {
    unsigned char _M_value_field[SIZE - 0x10];
};
template <class N> struct CNodeAllocProxy {
    N* _M_data;
    CNodeAllocProxy(N* p) : _M_data(p) { }
    N* allocate(unsigned int n) { return n != 0 ? (N*)malloc(n * sizeof(N)) : 0; }
};
template <int SIZE> struct CTreeBase {                  // STLport _Rb_tree_base
    CNodeAllocProxy< STreeNode<SIZE> > _M_header;
    CTreeBase() : _M_header(0) { _M_header._M_data = _M_header.allocate(1); }
};
struct SLess { };
template <int SIZE> struct CMap : CTreeBase<SIZE> {     // map<...>, 12 bytes
    unsigned long _M_node_count;
    SLess         _M_key_compare;
    CMap() : _M_node_count(0)
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};

struct SListNodeBase {                                  // STLport _List_node_base
    SListNodeBase* _M_next;
    SListNodeBase* _M_prev;
};
template <int SIZE> struct SListNode : SListNodeBase {
    unsigned char _M_data[SIZE - 8];
};
template <int SIZE> struct CList {                      // STLport list (_List_base: the sentinel in an alloc proxy), 4 bytes
    CNodeAllocProxy< SListNode<SIZE> > _M_node;
    CList() : _M_node(0)
    {
        SListNode<SIZE>* node = _M_node.allocate(1);
        node->_M_next = node;
        node->_M_prev = node;
        _M_node._M_data = node;
    }
};

class CTCBase { public:
    CTCBase(CThing* thing);                             // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

namespace LHNetworkLib { class LHHttp2; }
struct CHttpScopedPtr {                                 // scoped_ptr<LHNetworkLib::LHHttp2>
    LHNetworkLib::LHHttp2* px;
    CHttpScopedPtr() : px(0) { }
};
struct CComponent;
struct CActivity;

extern CCharString g_ScoreboardServer;                  // 0x013B8AF0 (descriptive: the clan web host)
extern CCharString g_ScoreboardPage;                    // 0x013B8AF4 (descriptive: the script path)
extern bool        g_bScoreboardInitialised;            // 0x013B8AE0

class CTCHeroOnlineScoreboard : public CTCBase { public:              // vtable 0x0124DB34
    CHttpScopedPtr   PHttp;                             // +0x0c
    CMap<0x1c>       PictureCache;                      // +0x10 map<long, CCountedPointer<CTexture> >
    CMap<0x18>       LocalScoreboard;                   // +0x1c map<EOnlineScoreboardActivity, float>
    vector<CActivity> ActivityInfo;                     // +0x28
    CCharString      SessionID;                         // +0x34
    CList<0x1c>      MyClans;                           // +0x38 list<CClanInfo>
    CWideString      SelectedClan;                      // +0x3c
    CList<0x14>      ClanMembers;                       // +0x40 list<CClanMember>
    CMap<0x20>       DownloadedClanScoreboard;          // +0x44 map<EOnlineScoreboardActivity, vector<pair<float, CWideString> > >
    long             ScoreboardState;                   // +0x50
    long             TaskState;                         // +0x54
    CComponent*      PScoreboardScreen;                 // +0x58
    CComponent*      PClanMenuEntry;                    // +0x5c
    CComponent*      PConnectingMessage;                // +0x60
    CComponent*      PConnectingMessage1;               // +0x64
    CTCHeroOnlineScoreboard(CThing* thing);
    virtual ~CTCHeroOnlineScoreboard();
    void InitialiseActivityInfo();                      // 0x00562710
};

CTCHeroOnlineScoreboard::CTCHeroOnlineScoreboard(CThing* thing)
    : CTCBase(thing), ScoreboardState(0), TaskState(100), PScoreboardScreen(0), PClanMenuEntry(0), PConnectingMessage(0),
      PConnectingMessage1(0)
{
    g_ScoreboardServer = "fableclangw.lionhead.com";
    g_ScoreboardPage = "/index.php";
    if (!g_bScoreboardInitialised)
        g_bScoreboardInitialised = true;
    AddToGlobalTCs();
    InitialiseActivityInfo();
}