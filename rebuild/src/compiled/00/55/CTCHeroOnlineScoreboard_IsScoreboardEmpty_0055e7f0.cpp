// CTCHeroOnlineScoreboard::IsScoreboardEmpty() -- true when every activity's downloaded clan-score list in DownloadedClanScoreboard is empty (walks the map in order, false at the first non-empty vector)
// Leaf: IsScoreboardEmpty
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055E7F0, 94 bytes (96 with padding), plain `ret` (thiscall, no arguments, bool in al).
// Callers: CTCHeroOnlineScoreboard::DisplayScoreboardGUI 0x00563920 (call 0x00563A05, `this` in edi).
// Label correction: catalogued as "GUI_FindNonEmptyNode"; Ego_r.pdb `?IsScoreboardEmpty@CTCHeroOnlineScoreboard@@QAE_NXZ`.
// Layout: retail CTCHeroOnlineScoreboard with 12-byte STLport maps/vectors and 4-byte lists (landed ResetScoreboard
//   0x00562E70: ScoreboardState +0x50): DownloadedClanScoreboard (map<EOnlineScoreboardActivity, vector<pair<float,
//   CWideString> > >, PDB +0x60) at +0x44; nodes { colour, parent +4, left +8, right +0xc, key +0x10, the vector's
//   start +0x14 / finish +0x18 / end of storage +0x1c }; elements are 8 bytes. Local model (no engine header).
// Shape: STLport `for (it = begin(); it != end(); ++it) if (it->second.size() != 0) return false; return true;` --
//   `size() != 0` is `test (finish - start), ~7`; the iterator's ++ is STLport's inline in-order step written as the
//   static `_M_increment(node)` returning the node (as in __find 0x0052DEC0).
class CWideString {
public:
    void* PData;
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<float, CWideString> SScoreEntry;
struct CScoreVector {                           // STLport vector<pair<float, CWideString> > (12 bytes)
    SScoreEntry* _M_start;
    SScoreEntry* _M_finish;
    SScoreEntry* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    bool empty() const { return _M_start == _M_finish; }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
enum EOnlineScoreboardActivity { ACTIVITY_NONE };   // CTCHeroOnlineScoreboard::EOnlineScoreboardActivity
struct SScoreboardPair {
    EOnlineScoreboardActivity first;
    CScoreVector second;
};
struct SScoreboardNode : _Rb_tree_node_base {
    SScoreboardPair _M_value_field;
};
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
        return _M_node;
    }
    SScoreboardPair& operator*() const { return ((SScoreboardNode*)_M_node)->_M_value_field; }
    SScoreboardPair* operator->() const { return &((SScoreboardNode*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& y) const { return _M_node == y._M_node; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
};
struct CScoreboardMap {                         // STLport map<EOnlineScoreboardActivity, CScoreVector> (12 bytes)
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};
class CTCHeroOnlineScoreboard {
public:
    virtual void v00();
    unsigned char _pad04[0x40];                 // +0x04 owner, PHttp, PictureCache .. ClanMembers (not touched)
    CScoreboardMap DownloadedClanScoreboard;    // +0x44 (PDB +0x60)
    bool IsScoreboardEmpty();
};
bool CTCHeroOnlineScoreboard::IsScoreboardEmpty()
{
    for (CScoreboardMap::iterator it = DownloadedClanScoreboard.begin(); it != DownloadedClanScoreboard.end(); ++it)
        if (it->second.size() != 0)
            return false;
    return true;
}