// Behaviour test for CMeshDataBank::FindLipSyncAnimation (0x00A26530): the animation's lip-sync table is searched by
// phoneme name (shared string block or equal text), the first hit's animation index is returned, 0 otherwise or
// when the animation has no table.
#include <cstdio>
#include <cstring>

struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};

long Compare_00411570(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
};
template <class _Value>
inline bool operator!=(const _Rb_tree_iterator<_Value>& __x, const _Rb_tree_iterator<_Value>& __y) { return __x._M_node != __y._M_node; }
template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;
    _Rb_tree_node_base* _M_find(const _Key& __k) const               // stand-in: linear scan of the header's left chain
    {
        for (_Rb_tree_node_base* n = _M_header->_M_left; n != _M_header && n != 0; n = n->_M_right)
            if (((_Rb_tree_node<value_type>*)n)->_M_value_field.first == __k) return n;
        return _M_header;
    }
    const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
    const_iterator end() const { return const_iterator(_M_header); }
};
}

template <class T> class CArray {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
};

class CMeshDataBank {
public:
    class CLipSyncEntry {
    public:
        CCharString   PhonemeName;
        unsigned long AnimationIndex;
    };
    std::map<CCharString, CArray<CLipSyncEntry> > LipSyncAnimations;
    unsigned long FindLipSyncAnimation(const CCharString& animation, const CCharString& phoneme) const;
};

unsigned long CMeshDataBank::FindLipSyncAnimation(const CCharString& animation, const CCharString& phoneme) const
{
    std::map<CCharString, CArray<CLipSyncEntry> >::const_iterator it = LipSyncAnimations.find(animation);
    if (it != LipSyncAnimations.end()) {
        const CArray<CLipSyncEntry>& entries = it->second;
        for (unsigned long i = 0; i < entries.size(); ++i) {
            const CLipSyncEntry& entry = entries[i];
            if (entry.PhonemeName == phoneme)
                return entry.AnimationIndex;
        }
    }
    return 0;
}

static CCharStringData MakeData(const char* s) { CCharStringData d; d.m_pData = s; d.m_length = (unsigned long)std::strlen(s); return d; }

typedef std::_Rb_tree_node<std::map<CCharString, CArray<CMeshDataBank::CLipSyncEntry> >::value_type> SNode;

int main()
{
    int ok = 1;
    CCharStringData talk = MakeData("talk"), shout = MakeData("shout"), whisper = MakeData("whisper");
    CCharStringData aa = MakeData("AA"), ee = MakeData("EE"), oh = MakeData("OH"), oo = MakeData("OO"), ee2 = MakeData("EE"), xx = MakeData("XY");
    CMeshDataBank::CLipSyncEntry talkEntries[3];
    talkEntries[0].PhonemeName.PStringData = &aa; talkEntries[0].AnimationIndex = 11;
    talkEntries[1].PhonemeName.PStringData = &ee; talkEntries[1].AnimationIndex = 12;
    talkEntries[2].PhonemeName.PStringData = &oh; talkEntries[2].AnimationIndex = 13;
    unsigned char raw[2][sizeof(SNode)];
    SNode* n1 = (SNode*)raw[0]; SNode* n2 = (SNode*)raw[1];
    std::_Rb_tree_node_base header;
    header._M_left = n1; n1->_M_right = n2; n2->_M_right = &header;
    const_cast<CCharString&>(n1->_M_value_field.first).PStringData = &talk;
    n1->_M_value_field.second._M_start = talkEntries; n1->_M_value_field.second._M_finish = talkEntries + 3;
    const_cast<CCharString&>(n2->_M_value_field.first).PStringData = &shout;
    n2->_M_value_field.second._M_start = 0; n2->_M_value_field.second._M_finish = 0;     // empty table
    CMeshDataBank bank;
    bank.LipSyncAnimations._M_header = &header; bank.LipSyncAnimations._M_node_count = 2;

    CCharString a; a.PStringData = &talk;
    CCharString p; p.PStringData = &ee;                    // same string block
    if (bank.FindLipSyncAnimation(a, p) != 12) ok = 0;
    p.PStringData = &ee2;                                  // equal text, different block
    if (bank.FindLipSyncAnimation(a, p) != 12) ok = 0;
    p.PStringData = &oh;
    if (bank.FindLipSyncAnimation(a, p) != 13) ok = 0;
    p.PStringData = &oo;                                   // same length, different text
    if (bank.FindLipSyncAnimation(a, p) != 0) ok = 0;
    p.PStringData = &xx;
    if (bank.FindLipSyncAnimation(a, p) != 0) ok = 0;
    p.PStringData = 0;                                     // null block
    if (bank.FindLipSyncAnimation(a, p) != 0) ok = 0;
    CCharString s; s.PStringData = &shout; p.PStringData = &aa;
    if (bank.FindLipSyncAnimation(s, p) != 0) ok = 0;      // empty table
    CCharString w; w.PStringData = &whisper;
    if (bank.FindLipSyncAnimation(w, p) != 0) ok = 0;      // unknown animation

    if (!ok) { std::printf("00a26530_TEST FAIL\n"); return 1; }
    std::printf("00a26530_TEST PASS\n");
    return 0;
}