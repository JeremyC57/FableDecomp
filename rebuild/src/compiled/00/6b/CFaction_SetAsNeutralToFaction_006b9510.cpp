// CFaction::SetAsNeutralToFaction(const CFaction& faction) -- drop the faction from both the allied and the enemy set
// Leaf: ?SetAsNeutralToFaction@CFaction@@QAEXABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B9510, 29 bytes (+ padding), thiscall `ret 4`.
// Identity (FINDINGS chunk 386 sibling): Ego_r.pdb `?SetAsNeutralToFaction@CFaction@@QAEXABV1@@Z` (name set) -- the argument is the
//   other CFaction, whose Name (CCharString) sits at +0, so `faction.Name` is the pointer itself; its only callers are CGameScriptInterface::SetFactionAsNeutralToFaction 0x008908C0 (two calls, both factions).
// Was landed as a NUISystem::CSpellContainerList::AddChild stand-in (two `CountedPtr::Assign` calls).
// Callees: STLport _Rb_tree<CCharString, CCharString, _Identity, less, allocator>::erase(const key_type&) @ 0x004BB270
//   (the erased count dropped) -- set::erase inline.
// Layout: CFaction from Ego_r.pdb is 36 bytes with 16-byte sets; the shipped STLport set is 12 bytes (_M_header,
//   _M_node_count, _M_key_compare), so EnemyFactions sits at +0x10 (PDB +0x14). No engine header exists for CFaction.
// Replaces a VC6-era _Tree set model (retail STL is STLport) and a descriptive name/signature.
class CCharString {
public:
    void* PStringData;                            // +0x0 CCharStringData*
};

namespace std {
template <class T> struct less {};
template <class T> struct _Identity {};
template <class T> class allocator {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
struct _Rb_tree_node_base;
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};
template <class K, class V, class KoV, class Cmp, class A>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    typedef unsigned int size_type;
    _Rb_tree_node_base* _M_header;                // +0x0
    size_type           _M_node_count;            // +0x4
    Cmp                 _M_key_compare;           // +0x8
    pair<iterator, bool> insert_unique(const V& v);   // 0x004AC380
    size_type erase(const K& k);                       // 0x004BB270
};
template <class K, class Cmp = less<K>, class A = allocator<K> >
class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, Cmp, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::size_type size_type;
    _Rep_type _M_t;
    pair<iterator, bool> insert(const K& x) { return _M_t.insert_unique(x); }
    size_type erase(const K& x) { return _M_t.erase(x); }
};
}

class CFaction {
public:
    CCharString                Name;              // +0x00
    std::set<CCharString>      AlliedFactions;    // +0x04
    std::set<CCharString>      EnemyFactions;     // +0x10
    void SetAsNeutralToFaction(const CFaction& faction);
};

void CFaction::SetAsNeutralToFaction(const CFaction& faction)
{
    AlliedFactions.erase(faction.Name);
    EnemyFactions.erase(faction.Name);
}
