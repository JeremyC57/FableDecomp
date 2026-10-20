#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
static char g_log[512]; static int g_frees, g_destroyed; static void* g_erasedRoot;
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CThing;

struct CCharString {
    void* PStringData;
    ~CCharString() { LOG("str"); }
};
struct CWideString {
    void* PStringData;
    ~CWideString() { LOG("wstr"); }
};

struct __false_type { __false_type() {} };
template <class T> void FASTCALL __destroy_aux(T* first, T* last, const __false_type&) { g_destroyed = (int)(last - first); LOG("vec"); }

template <class P> struct CAllocProxy {                 // STLport _STLP_alloc_proxy<P, ...> over an empty allocator
    P _M_data;
    void deallocate(P p) { if (p) { ++g_frees; std::free(p); } }
};
template <class T> struct CVectorBase {                 // STLport _Vector_base, 12 bytes
    T*              _M_start;
    T*              _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    ~CVectorBase() { _M_end_of_storage.deallocate(_M_start); }
};
template <class T> struct vector : CVectorBase<T> {
    ~vector() { __destroy_aux(this->_M_start, this->_M_finish, __false_type()); }
};

struct SRbTreeNodeBase {                                // STLport _Rb_tree_node_base
    bool             _M_color;                          // +0x00
    SRbTreeNodeBase* _M_parent;                         // +0x04
    SRbTreeNodeBase* _M_left;                           // +0x08
    SRbTreeNodeBase* _M_right;                          // +0x0c
};
template <int SIZE> struct CTreeBase {                  // STLport _Rb_tree_base: the header in an alloc proxy
    CAllocProxy<SRbTreeNodeBase*> _M_header;
    ~CTreeBase() { _M_header.deallocate(_M_header._M_data); }
};
struct SLess { };
template <int SIZE> struct CMap : CTreeBase<SIZE> {     // map<...> (_Rb_tree), 12 bytes
    unsigned long _M_node_count;
    SLess         _M_key_compare;
    void _M_erase(SRbTreeNodeBase* x) { g_erasedRoot = x; LOG(SIZE == 0x20 ? "erase20" : SIZE == 0x18 ? "erase18" : "erase1c"); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(this->_M_header._M_data->_M_parent);
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    ~CMap() { clear(); }
};

struct SListNodeBase {                                  // STLport _List_node_base
    SListNodeBase* _M_next;
    SListNodeBase* _M_prev;
};
template <int SIZE> struct CList {                      // STLport list (_List_base: the sentinel in an alloc proxy), 4 bytes
    CAllocProxy<SListNodeBase*> _M_node;
    void clear() { LOG(SIZE == 0x14 ? "clear14" : "clear1c"); }
    ~CList()
    {
        clear();
        _M_node.deallocate(_M_node._M_data);
    }
};

namespace LHNetworkLib {
struct LHHttp2 {
    ~LHHttp2() { LOG("http"); }
};
}
struct CHttpScopedPtr {                                 // scoped_ptr<LHNetworkLib::LHHttp2>
    LHNetworkLib::LHHttp2* px;
    ~CHttpScopedPtr() { delete px; }
};

struct CTCBase {
    virtual ~CTCBase() { LOG("base"); }
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    void RemoveFromGlobalTCs() { LOG("remove"); }
};

struct CComponent;
struct CActivity { unsigned char _body[0x10]; };

struct CTCHeroOnlineScoreboard : CTCBase {              // vtable 0x0124DB34
    CHttpScopedPtr    PHttp;                            // +0x0c
    CMap<0x1c>        PictureCache;                     // +0x10 map<long, CCountedPointer<CTexture> >
    CMap<0x18>        LocalScoreboard;                  // +0x1c map<EOnlineScoreboardActivity, float>
    vector<CActivity> ActivityInfo;                     // +0x28
    CCharString       SessionID;                        // +0x34
    CList<0x1c>       MyClans;                          // +0x38 list<CClanInfo>
    CWideString       SelectedClan;                     // +0x3c
    CList<0x14>       ClanMembers;                      // +0x40 list<CClanMember>
    CMap<0x20>        DownloadedClanScoreboard;         // +0x44 map<EOnlineScoreboardActivity, vector<pair<float, CWideString> > >
    long              ScoreboardState;                  // +0x50
    long              TaskState;                        // +0x54
    CComponent*       PScoreboardScreen;                // +0x58
    CComponent*       PClanMenuEntry;                   // +0x5c
    CComponent*       PConnectingMessage;               // +0x60
    CComponent*       PConnectingMessage1;              // +0x64
    virtual ~CTCHeroOnlineScoreboard();
};

CTCHeroOnlineScoreboard::~CTCHeroOnlineScoreboard()
{
    RemoveFromGlobalTCs();
}

template <int SIZE> static void InitMap(CMap<SIZE>& m, unsigned long count, SRbTreeNodeBase* root)
{
    m._M_header._M_data = (SRbTreeNodeBase*)std::malloc(sizeof(SRbTreeNodeBase));
    m._M_header._M_data->_M_parent = root; m._M_header._M_data->_M_left = root; m._M_header._M_data->_M_right = root;
    m._M_node_count = count;
}
template <int SIZE> static void InitList(CList<SIZE>& l)
{
    l._M_node._M_data = (SListNodeBase*)std::malloc(sizeof(SListNodeBase));
    l._M_node._M_data->_M_next = l._M_node._M_data->_M_prev = l._M_node._M_data;
}
int main()
{
    void* raw = std::malloc(sizeof(CTCHeroOnlineScoreboard));
    CTCHeroOnlineScoreboard* s = new (raw) CTCHeroOnlineScoreboard;
    SRbTreeNodeBase root20, root1c;
    InitMap(s->DownloadedClanScoreboard, 2, &root20);
    InitMap(s->LocalScoreboard, 0, 0);
    InitMap(s->PictureCache, 5, &root1c);
    InitList(s->MyClans); InitList(s->ClanMembers);
    s->ActivityInfo._M_start = (CActivity*)std::malloc(3 * sizeof(CActivity));
    s->ActivityInfo._M_finish = s->ActivityInfo._M_start + 3; s->ActivityInfo._M_end_of_storage._M_data = s->ActivityInfo._M_finish;
    s->PHttp.px = new LHNetworkLib::LHHttp2;
    s->~CTCHeroOnlineScoreboard();
    if (std::strcmp(g_log, "remove;erase20;clear14;wstr;clear1c;str;vec;erase1c;http;base;") || g_frees != 6 || g_destroyed != 3 || g_erasedRoot != &root1c) { std::printf("00562ff0_TEST FAIL\n"); return 1; }
    /* empty scoped pointer and empty vector: no delete, no storage free */
    g_log[0] = 0; g_frees = 0;
    s = new (raw) CTCHeroOnlineScoreboard;
    InitMap(s->DownloadedClanScoreboard, 0, 0); InitMap(s->LocalScoreboard, 0, 0); InitMap(s->PictureCache, 0, 0);
    InitList(s->MyClans); InitList(s->ClanMembers);
    s->ActivityInfo._M_start = s->ActivityInfo._M_finish = 0; s->PHttp.px = 0;
    s->~CTCHeroOnlineScoreboard();
    if (std::strcmp(g_log, "remove;clear14;wstr;clear1c;str;vec;base;") || g_frees != 5 || g_destroyed != 0) { std::printf("00562ff0_TEST FAIL\n"); return 1; }
    std::free(raw);
    std::printf("00562ff0_TEST PASS\n");
    return 0;
}
