// Behaviour test for 0x00769980 _Rb_tree::_M_create_node: a fresh node holds a copy of the (augmentation
// type, sound set) value -- key, Played flag, deep-copied sound name, handle.
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

struct CObjectAugmentationSoundSet {
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

struct CSoundSetTree {
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    SSoundSetNode* _M_create_node(const SSoundSetValue& __x)
    {
        SSoundSetNode* __tmp = (SSoundSetNode*)std::malloc(sizeof(SSoundSetNode));
        new (&__tmp->_M_value_field) SSoundSetValue(__x);
        return __tmp;
    }
};

int main()
{
    CSoundSetTree tree = { 0, 0 };
    CObjectAugmentationSoundSet set;
    set.Played = true; set.Handle = 42u;
    SSoundSetValue v(AUG_LIGHTNING, set);
    int before = gStrCopies;
    SSoundSetNode* n = tree._M_create_node(v);
    int ok = n != 0 && n->_M_value_field.first == AUG_LIGHTNING && n->_M_value_field.second.Played
          && n->_M_value_field.second.Handle == 42u && gStrCopies == before + 1
          && n->_M_value_field.second.Sound.PStringData != v.second.Sound.PStringData;
    n->_M_value_field.~SSoundSetValue();
    std::free(n);
    if (!ok) { std::printf("00769980_TEST FAIL\n"); return 1; }
    std::printf("00769980_TEST PASS\n");
    return 0;
}