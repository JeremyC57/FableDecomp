// CTCHeroOnlineScoreboard::ClearScoreboard() -- clear the downloaded clan scoreboard map and the clan list, reset the selected clan to the empty wide string, then clear the clan member list
// Leaf: ?ClearScoreboard@CTCHeroOnlineScoreboard@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00562E10, 84 bytes, thiscall, tail `jmp` to the ClanMembers list clear.
// Identity (proven): (1) the static console command at 0x00562EB0 is a bare `jmp` here (Ego_r ?ConsoleClearScoreboard@
//   CTCHeroOnlineScoreboard@@SIXAAV1@ABV?$vector@VCGenericVar@@...@Z: the scoreboard in ecx), and Ego_r's block order ClearScoreboard
//   0x005DA4E0 / Close 0x005DA530 / ConsoleClearScoreboard 0x005DA660 is retail's 0x00562E10 / 0x00562E70 / 0x00562EB0; the other
//   callers are 0x00562E70 (Close) and the scoreboard flow 0x00563141 / 0x00563439; (2) Ego_r.pdb ?ClearScoreboard@
//   CTCHeroOnlineScoreboard@@AAEXXZ (private), and its members are exactly what the Ego_r layout puts at the retail offsets:
//   DownloadedClanScoreboard +0x44, MyClans +0x38, SelectedClan +0x3C, ClanMembers +0x40.
// Callees: the DownloadedClanScoreboard tree's private _M_erase 0x00569A80 (inside the inline STLport map::clear()),
//   std::_List_base<CClanInfo>::clear 0x00566DC0 and std::_List_base<CClanMember>::clear 0x00566E10 (STLport 4.6 _list.c, out of
//   line; list::clear() is the inline `_Base::clear()`), CWideString::operator=(const CWideString&) 0x0099B7D0 (Ego_r
//   ??4CWideString@@QAEABV0@ABV0@@Z), CWideString::NullString 0x013BCA24 (?NullString@CWideString@@2V1@A, the queue's accepted spelling).
// Layout: the landed ctor 0x00562EC0 / dtor 0x00562FF0 (PDB CTCHeroOnlineScoreboard with 12-byte retail maps/vectors and 4-byte
//   STLport lists): PHttp +0x0C, PictureCache +0x10, LocalScoreboard +0x1C, ActivityInfo +0x28, SessionID +0x34, MyClans +0x38,
//   SelectedClan +0x3C, ClanMembers +0x40, DownloadedClanScoreboard +0x44, ScoreboardState +0x50 ... PConnectingMessage1 +0x64.
// Shape: STLport 4.6 _Rb_tree::clear() (count test, `_M_erase(_M_root())`, leftmost / root / rightmost / count reset through the
//   re-read header), the two list clears and the string assignment in source order. The map types are the landed map ctor
//   0x0056A580's (STLport stl/_tree.h text, value pair<const EOnlineScoreboardActivity, vector<pair<float, CWideString> > >).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14

class CWideStringData;
class CWideString {
public:
    ~CWideString();                                       // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    static CWideString NullString;                        // 0x013BCA24
    CWideStringData* PStringData;
};
class CCharStringData;
class CCharString {
public:
    ~CCharString();                                       // 0x0099EAE0
    CCharStringData* PStringData;
};
class CThing;

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    void clear() { _M_t.clear(); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // +0x0: the sentinel node
    void clear();                                         // _list.c, out of line
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    void clear() { _Base::clear(); }
};
}   // namespace std

namespace boost {
template <class T> class scoped_ptr {
public:
    T* px;
};
}
namespace LHNetworkLib {
class LHHttp2;
}
template <class T> class CCountedPointer;
class CTexture;
namespace NUISystem {
class CComponent;
}

class CTCBase {                                           // engine/CTCBase.h is flattened (vptr as a data member)
public:
    virtual ~CTCBase();                                   // 0x00686830
    CThing* ParentThing;                                  // +0x04
    bool    ToBeRemoved;                                  // +0x08
    bool    IsInGlobalUpdateTCs;                          // +0x09
};

class CTCHeroOnlineScoreboard : public CTCBase {          // vtable 0x0124DB34
public:
    enum EOnlineScoreboardActivity { };
    enum EScoreboardState { };
    enum ETaskState { };
    class CActivity { public: unsigned char _body[0x10]; };
    class CClanInfo {                                     // Ego_r.pdb, 0x14 bytes
    public:
        CWideString Name;                                 // +0x00
        long        ClanID;                               // +0x04
        CWideString Description;                          // +0x08
        CCharString HomePageURL;                          // +0x0C
        int         NumMembers;                           // +0x10
    };
    class CClanMember {                                   // Ego_r.pdb, 0x0C bytes
    public:
        CWideString Name;                                 // +0x00
        long        MemberID;                             // +0x04
        CWideString Description;                          // +0x08
    };

    boost::scoped_ptr<LHNetworkLib::LHHttp2>                   PHttp;                     // +0x0C
    std::map<long, CCountedPointer<CTexture> >                 PictureCache;              // +0x10
    std::map<EOnlineScoreboardActivity, float>                 LocalScoreboard;           // +0x1C
    std::vector<CActivity>                                     ActivityInfo;              // +0x28
    CCharString                                                SessionID;                 // +0x34
    std::list<CClanInfo>                                       MyClans;                   // +0x38
    CWideString                                                SelectedClan;              // +0x3C
    std::list<CClanMember>                                     ClanMembers;               // +0x40
    std::map<EOnlineScoreboardActivity, std::vector<std::pair<float, CWideString> > > DownloadedClanScoreboard;   // +0x44
    EScoreboardState                                           ScoreboardState;           // +0x50
    ETaskState                                                 TaskState;                 // +0x54
    NUISystem::CComponent*                                     PScoreboardScreen;         // +0x58
    NUISystem::CComponent*                                     PClanMenuEntry;            // +0x5C
    NUISystem::CComponent*                                     PConnectingMessage;        // +0x60
    NUISystem::CComponent*                                     PConnectingMessage1;       // +0x64
private:
    void ClearScoreboard();
};

void CTCHeroOnlineScoreboard::ClearScoreboard()
{
    DownloadedClanScoreboard.clear();
    MyClans.clear();
    SelectedClan = CWideString::NullString;
    ClanMembers.clear();
}