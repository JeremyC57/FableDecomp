// CTCInGameMenu::VerifySignatureCached(const CWideString& filename) -- memoised save-file signature check: answer from the Signatures cache, else CUserProfileManager::VerifySignature and remember the result
// Leaf: ?VerifySignatureCached@CTCInGameMenu@@QAE_NABVCWideString@@@Z
// Retail: Fable.exe @ 0x0047ED0A, 108 bytes, `ret 4`. Ego_r.pdb names it ?VerifySignatureCached@CTCInGameMenu@@QAE_NABVCWideString@@@Z
// (egor_pdb_names.tsv) and FableWin.pdb carries the same symbol. The label CUserProfileManager::ValidateSaveFile is
// wrong: every caller is a CTCInGameMenu method (ConstructSaveGameFileDescription 0x0047EEE4, OpenInGameMenuPC 0x004804DE).
// Body: i = cache.find(filename) (STLport _M_find 0x004856CD); if i != end() return (*i).second (node +0x14). Else
// valid = VerifySignature(filename) (0x00409730, Ego_r.pdb ?VerifySignature@CUserProfileManager@@SI_NABVCWideString@@@Z,
// static __fastcall); cache.insert(make_pair(filename, valid)) -- the pair<CWideString,bool> temp (CWideString copy ctor
// 0x0099B720) is converted to value_type by the out-of-line pair converting ctor 0x00485125, then insert_unique
// 0x00485E5C (pair<iterator,bool> result in the frame); both CWideString temps die through 0x0099B510; return valid.
// Layout: CTCInGameMenu::Signatures, map<CWideString,bool>. Ego_r.pdb puts it at +0x100; retail has it at +0xe0. The twin
// NUISystem::CFrontEndManager::VerifySignatureCached 0x005957D9 is byte-identical except Signatures +0xc4 (PDB +0xd8).
// STLport _Rb_tree: header, node count, comparator; node value at +0x10.
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry). Retail inlines make_pair and the
// pair ctor (copy ctor straight into the frame pair, `valid` kept in bl). Under the size pragma VC7.1 calls both out of line,
// so they are __forceinline here. Plain inline gives 109v108, and the speed TU gives 116v108.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWideString {
public:
    void* m_pData;
    CWideString(const CWideString& s);                   // 0x0099B720
    ~CWideString();                                      // 0x0099B510
};
namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const;
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    __forceinline pair(const T1& a, const T2& b) : first(a), second(b) {}
    template <class U1, class U2> pair(const pair<U1, U2>& p);   // 0x00485125 for <CWideString,bool> -> <const CWideString,bool>
};
template <class T1, class T2> __forceinline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }
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
    bool operator!=(const _Rb_tree_base_iterator& y) const { return _M_node != y._M_node; }
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
    _Rb_tree_node_base* _M_find(const K& k) const;       // 0x004856CD
    pair<iterator, bool> insert_unique(const value_type& v);   // 0x00485E5C
    iterator find(const K& k) { return iterator(_M_find(k)); }
    iterator end() { return iterator(_M_header); }
    pair<iterator, bool> insert(const value_type& v) { return insert_unique(v); }
};
}
class CUserProfileManager {
public:
    static bool __fastcall VerifySignature(const CWideString& filename);   // 0x00409730
};
class CTCInGameMenu {
public:
    unsigned char _pad00[0xe0];
    std::map<CWideString, bool> Signatures;              // +0xe0 (Ego_r.pdb +0x100)
    bool VerifySignatureCached(const CWideString& filename);
};
bool CTCInGameMenu::VerifySignatureCached(const CWideString& filename)
{
    std::map<CWideString, bool>::iterator i = Signatures.find(filename);
    if (i != Signatures.end())
        return (*i).second;
    bool valid = CUserProfileManager::VerifySignature(filename);
    Signatures.insert(std::make_pair(filename, valid));
    return valid;
}