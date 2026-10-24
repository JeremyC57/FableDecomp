// std::map<CCharString, MirroredCriminalStatus>::operator[](const CCharString& k) -- STLport subscript on the hero's per-region criminal records: lower_bound, insert (k, MirroredCriminalStatus()) when absent, return the record
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) @ 0x005812DF, 111 bytes, `ret 4`.
// Callees: _M_lower_bound(k) @ 0x0057FE4A (node pointer in eax), less<CCharString>::operator() @ 0x0042AAB3 (landed),
// CTCHeroStats::MirroredCriminalStatus::MirroredCriminalStatus() @ 0x0057A5DD (landed), pair<const CCharString,
// MirroredCriminalStatus>::pair(const CCharString&, const MirroredCriminalStatus&) @ 0x0057ED6C (out of line here:
// CCharString copy ctor + a 17-dword block), map::insert(iterator, const value_type&) @ 0x0058117E (the tree insert
// with a hint), ~CCharString @ 0x0099EAE0 (the value_type temporary's key; the record has no destructor).
// Callers: fourteen sites in the CTCHeroStats crime code (0x0057D9BF .. 0x0057EB40) and two at 0x0065C656/0x0065C744.
// Label: the catalog "DeserializeStdMap_WithPairs" is descriptive: the body is STLport 4.x `map::operator[]`
// verbatim, the same template as the landed map<CCharString, unsigned long>::operator[] 0x0042D131: the key_comp()
// temporary in the high byte of the dead `k` slot (`lea ecx,[ebp+0Bh]`, which needs the visible noinline functor
// body), the `_Tp()` temporary (0x44 bytes) and the value_type temporary (0x48 bytes) in the frame, the iterator
// argument rebuilt in its slot by the iterator copy ctor and the result returned through the dead `k` slot; the value
// is node + 0x14 (key CCharString at node + 0x10).
// The `_Tp()` default value matches only as a named local of the insert block (`T def; i = insert(i, value_type(k,
// def));`): it is re-addressed with `lea` after its out-of-line ctor and takes the frame slot above the pair
// temporary, and the insert result lands in the dead `k` slot. The bare `value_type(k, T())` temporary pushes the
// ctor's eax, swaps the two frame slots and gives the result its own word (105 v 111), with the forwarder or the
// direct insert alike.
// Model: MirroredCriminalStatus has the Ego_r.pdb layout (0x44 bytes, as the landed ctor 0x0057A5DD models it).
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};

class CCharString
{
public:
    CBasicString<char>* PData;                           // +0x00
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

struct CTCHeroStats {
    struct MirroredCriminalStatus {                      // 0x44 bytes (Ego_r.pdb)
        long  Crimes[10];                                // +0x00
        long  LastTimeSeenByGuard;                       // +0x28
        long  LastTimeExited;                            // +0x2c
        bool  ExitedUsingTeleporter;                     // +0x30
        float EntrancePosX;                              // +0x34
        float EntrancePosY;                              // +0x38
        float EntrancePosZ;                              // +0x3c
        float EntranceAngle;                             // +0x40
        MirroredCriminalStatus();                        // 0x0057A5DD
    };
};

namespace std {
template <class T> struct less {
    __declspec(noinline) bool operator()(const T& x, const T& y) const { return x < y; }   // 0x0042AAB3 for CCharString
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b);                      // 0x0057ED6C
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;              // 0x0057FE4A
    iterator insert(iterator position, const value_type& v);          // 0x0058117E
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first)) {
            T def;
            i = insert(i, value_type(k, def));
        }
        return (*i).second;
    }
};
template CTCHeroStats::MirroredCriminalStatus& map<CCharString, CTCHeroStats::MirroredCriminalStatus>::operator[](const CCharString&);
}