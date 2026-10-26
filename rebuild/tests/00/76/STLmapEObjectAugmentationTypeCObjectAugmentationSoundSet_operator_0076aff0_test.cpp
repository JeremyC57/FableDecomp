// Behaviour test for 0x0076AFF0 map<EObjectAugmentationType, CObjectAugmentationSoundSet>::operator[]: a missing key
// is inserted with a default sound set (not played, empty name, handle -1); an existing key returns its stored set.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int gStrCopies = 0;

class CCharString {
public:
    char* PStringData;
    CCharString(const char* s, long) { PStringData = new char[std::strlen(s) + 1]; std::strcpy(PStringData, s); }
    CCharString(const CCharString& o) { ++gStrCopies; PStringData = new char[std::strlen(o.PStringData) + 1]; std::strcpy(PStringData, o.PStringData); }
    ~CCharString() { delete[] PStringData; }
};

enum EObjectAugmentationType { AUG_NONE, AUG_FIRE, AUG_LIGHTNING, AUG_SILVER };

class CObjectAugmentationSoundSet { public:
    bool          Played;
    CCharString   Sound;
    unsigned int  Handle;
    CObjectAugmentationSoundSet() : Played(false), Sound("", -1), Handle(0xFFFFFFFFu) { }
};

template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};

struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

typedef pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> SSoundSetValue;
struct SSoundSetNode : _Rb_tree_node_base { SSoundSetValue _M_value_field; };

static int gRebalances = 0;
static void Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { ++gRebalances; }   // colour fix-up stand-in

struct CSoundSetMap {
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    _Rb_tree_node_base* insert_unique(_Rb_tree_node_base* hint, const SSoundSetValue& v)   // plain BST stand-in
    {
        SSoundSetNode* z = (SSoundSetNode*)std::malloc(sizeof(SSoundSetNode));
        new (&z->_M_value_field) SSoundSetValue(v);
        z->_M_left = z->_M_right = 0;
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        bool left = true;
        while (x) { y = x; left = v.first < ((SSoundSetNode*)x)->_M_value_field.first; x = left ? x->_M_left : x->_M_right; }
        z->_M_parent = y;
        if (y == _M_header) _M_header->_M_parent = z;
        else if (left) y->_M_left = z; else y->_M_right = z;
        ++_M_node_count;
        (void)hint;
        return z;
    }
    _Rb_tree_node_base* lower_bound(EObjectAugmentationType k)
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!(((SSoundSetNode*)x)->_M_value_field.first < k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    CObjectAugmentationSoundSet& operator[](const EObjectAugmentationType& k)
    {
        _Rb_tree_node_base* i = lower_bound(k);
        if (i == _M_header || k < ((SSoundSetNode*)i)->_M_value_field.first) {
            CObjectAugmentationSoundSet def;
            i = insert_unique(i, SSoundSetValue(k, def));
        }
        return ((SSoundSetNode*)i)->_M_value_field.second;
    }
};

int main()
{
    _Rb_tree_node_base header = { 0, 0, 0, 0 };
    CSoundSetMap m = { &header, 0 };
    CObjectAugmentationSoundSet& fire = m[AUG_FIRE];
    int ok = !fire.Played && fire.Handle == 0xFFFFFFFFu && fire.Sound.PStringData[0] == 0 && m._M_node_count == 1;
    fire.Played = true; fire.Handle = 7u;
    CObjectAugmentationSoundSet& silver = m[AUG_SILVER];
    if (&silver == &fire || m._M_node_count != 2) ok = 0;
    CObjectAugmentationSoundSet& again = m[AUG_FIRE];
    if (&again != &fire || !again.Played || again.Handle != 7u || m._M_node_count != 2) ok = 0;
    m[AUG_NONE].Handle = 3u;
    if (m[AUG_NONE].Handle != 3u || m._M_node_count != 3) ok = 0;
    if (!ok) { std::printf("0076aff0_TEST FAIL\n"); return 1; }
    std::printf("0076aff0_TEST PASS\n");
    return 0;
}