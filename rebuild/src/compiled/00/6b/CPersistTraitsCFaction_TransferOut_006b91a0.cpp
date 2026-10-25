// CPersistTraits<CFaction>::TransferOut(CPersistContext& context, CCharString& out, const CFaction& faction) -- append the faction's text form: its quoted name, the ally count and each quoted ally, the enemy count and each quoted enemy, then the EndFaction marker
// Leaf: ?TransferOut@?$CPersistTraits@VCFaction@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B91A0, 511 bytes (+ int3 padding to 512), thiscall, `ret 0Ch` (`this` and
//   the context unused).
// Identity: the catalog's "Faction_ExportToString" is a descriptive auto label. FableWin.pdb lists
//   ?TransferOut@?$CPersistTraits@VCFaction@@@@QAEXAAVCPersistContext@@AAVCCharString@@ABVCFaction@@@Z beside
//   TransferIn (0x006B9610, this chunk); CFaction = {Name CCharString +0, AlliedFactions set<CCharString> +4,
//   EnemyFactions set<CCharString> +0x10} (Ego_r.pdb, 12-byte STLport sets). Callers 0x006BA05E and 0x006BAD41.
// Callees: operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600,
//   CCharString::operator+=(const CCharString&) 0x0099F0A0, ~CCharString 0x0099EAE0, GFIntToCharString 0x0099F830,
//   CCharString::operator+=(const char*) 0x0099F100. Strings: "Name \"", "NumberOfAllies ", "Ally \"",
//   "NumberOfEnemies ", "Enemy \"", "\";\r\n", ";\r\n", "EndFaction".
// Shape: one `out += a + b + c;` statement per line (the temporaries homed in the push-ecx slot and the dead argument
//   slots), each set walked with an inline `++it` (STLport _M_increment) against a re-read end().
class CCharString {
public:
    void* PData;
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);      // 0x0099F0A0
    CCharString& operator+=(const char* text);              // 0x0099F100
};
CCharString __fastcall operator+(const char* left, const CCharString& right);    // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);    // 0x0099F600
CCharString __fastcall GFIntToCharString(long value);                           // 0x0099F830

namespace std {
template <class T> struct less { };
struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
    const V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
};
template <class K, class C = less<K> > class set {
public:
    typedef _Rb_tree_iterator<K> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    C                   _M_key_compare;                     // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
};
}

class CFaction {
public:
    CCharString Name;                                       // +0x00
    std::set<CCharString> AlliedFactions;                   // +0x04
    std::set<CCharString> EnemyFactions;                    // +0x10
};

class CPersistContext;

template <class T> class CPersistTraits;
template <> class CPersistTraits<CFaction> {
public:
    void TransferOut(CPersistContext& context, CCharString& out, const CFaction& faction);
};

void CPersistTraits<CFaction>::TransferOut(CPersistContext& context, CCharString& out, const CFaction& faction)
{
    out += "Name \"" + faction.Name + "\";\r\n";
    out += "NumberOfAllies " + GFIntToCharString(faction.AlliedFactions.size()) + ";\r\n";
    for (std::set<CCharString>::const_iterator it = faction.AlliedFactions.begin(); it != faction.AlliedFactions.end(); ++it)
        out += "Ally \"" + *it + "\";\r\n";
    out += "NumberOfEnemies " + GFIntToCharString(faction.EnemyFactions.size()) + ";\r\n";
    for (std::set<CCharString>::const_iterator it = faction.EnemyFactions.begin(); it != faction.EnemyFactions.end(); ++it)
        out += "Enemy \"" + *it + "\";\r\n";
    out += "EndFaction";
}