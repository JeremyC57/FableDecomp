// CTCHeroOnlineScoreboard::~CTCHeroOnlineScoreboard() -- unregister from the global TC list, then destroy the members in reverse (downloaded clan scoreboard map, clan member list, selected clan, clan list, session id, activity table, local scoreboard and picture cache maps, the HTTP connection), then the CTCBase base
// Leaf: ??1CTCHeroOnlineScoreboard
// Retail: Fable.exe (TLC, VC7.1) @ 0x00562FF0, 320 bytes, `ret`. Called from the deleting destructor 0x0056A743; vtable 0x0124DB34
//   (CTCHeroOnlineScoreboard, rtti-vtables.tsv).
// Label correction: catalogued as ~CPatchTesselationEdgeStrip (propagated); the vtable store pins CTCHeroOnlineScoreboard.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (the body), the three map `_M_erase` instantiations 0x00569A80 (+0x44),
//   0x00566110 (+0x1C), 0x00567980 (+0x10), the two list `clear` instantiations 0x00566E10 (+0x40) and 0x00566DC0 (+0x38),
//   ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, the `/Gr` range destroy 0x00567350 (`__fastcall`, first in ecx, last in
//   edx, the `__false_type` tag by const reference), LHNetworkLib::LHHttp2::~LHHttp2 0x008410D0, operator delete 0x00BFE9BC,
//   free 0x00BFEA14 (STLport node/header/storage), CTCBase::~CTCBase 0x00686830. All the catalogue labels on these callees
//   are propagated names of other classes.
// Layout: the landed ctor 0x00562EC0 (PDB CTCHeroOnlineScoreboard with 12-byte retail maps/vectors and 4-byte lists).
// Shape: STLport 4.x container destructors inline: _Rb_tree::clear() (count test, `_M_erase(root)`, leftmost / root /
//   rightmost / count reset through the re-read header) then the header deallocate; list: out-of-line clear, then the
//   sentinel free; vector: `__destroy_aux(first, last, __false_type())` then the storage free; scoped_ptr: `delete px`.
//   The `__false_type` temporary is the top byte of the `push ecx` frame word.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct CThing;

struct CCharString {
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};

struct __false_type { __false_type() {} };
template <class T> void __fastcall __destroy_aux(T* first, T* last, const __false_type&);   // 0x00567350 for CActivity

template <class P> struct CAllocProxy {                 // STLport _STLP_alloc_proxy<P, ...> over an empty allocator
    P _M_data;
    void deallocate(P p) { if (p) free(p); }
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
    void _M_erase(SRbTreeNodeBase* x);                  // out of line, one per map
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
    void clear();                                       // out of line, one per list
    ~CList()
    {
        clear();
        _M_node.deallocate(_M_node._M_data);
    }
};

namespace LHNetworkLib {
struct LHHttp2 {
    ~LHHttp2();                                         // 0x008410D0
};
}
struct CHttpScopedPtr {                                 // scoped_ptr<LHNetworkLib::LHHttp2>
    LHNetworkLib::LHHttp2* px;
    ~CHttpScopedPtr() { delete px; }
};

struct CTCBase {
    virtual ~CTCBase();                                 // 0x00686830
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    void RemoveFromGlobalTCs();                         // 0x00686DF0
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
