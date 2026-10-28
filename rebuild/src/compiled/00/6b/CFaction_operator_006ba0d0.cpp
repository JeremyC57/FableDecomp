// CFaction::operator=(const CFaction& other) -- copy assignment of a faction: its name, then the allied and enemy faction-name sets
// Leaf: ??4CFaction@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BA0D0, 45 bytes, __fastcall `ret 4`, returns `*this`.
// Identity (label correction): the catalog's "pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>" is
//   propagated. Both callers sit in CPersistContext::Transfer of a CFaction (0x006BABD0: the switch arm that assigns the
//   default value, `value = default`). Ego_r.pdb CFaction: Name +0 (CCharString), AlliedFactions, EnemyFactions
//   (set<CCharString>); with TLC's 12-byte STLport sets the members sit at +0, +4, +0x10 (0x1C bytes).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, the set's _Rb_tree::operator= @ 0x006B9E00 (twice: the
//   header-node clear / _M_copy / leftmost-rightmost walk of STLport 4.6).
// Shape: the compiler-generated memberwise assignment; set::operator= (`_M_t = __x._M_t; return *this;`) is inline, so
//   only the tree assignment is called. The address-of below makes VC7.1 emit the implicit `??4` out of line.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace std {
template <class _Tp> struct less {};
template <class _Tp> class allocator {};
template <class _Tp> struct _Identity {};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    void*         _M_header;                            // +0x00 (_STLP_alloc_proxy over the header node)
    unsigned int  _M_node_count;                        // +0x04
    _Compare      _M_key_compare;                       // +0x08
    _Rb_tree& operator=(const _Rb_tree& __x);           // 0x006B9E00
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                     // +0x00 (12 bytes)
    set<_Key, _Compare, _Alloc>& operator=(const set<_Key, _Compare, _Alloc>& __x) { _M_t = __x._M_t; return *this; }
};
}

class CFaction {
public:
    CCharString             Name;                       // +0x00
    std::set<CCharString>  AlliedFactions;             // +0x04
    std::set<CCharString>  EnemyFactions;              // +0x10
};

CFaction& (CFaction::*g_pAssignFaction)(const CFaction&) = &CFaction::operator=;