// Behaviour test for 006f4bc0 (STLport _Rb_tree::erase(iterator) of the UID -> CCombatantInfo map: unlink, destroy the value, free, count down).
#include <cstdio>
#include <cstdlib>
#include <new>
#if defined(_MSC_VER)
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

static int gSequenceReleases = 0;
static int gDefenderDtors = 0;

class CCombatSequenceBase;
template<class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
    ~CCountedPointer() { ++gSequenceReleases; }   // stand-in for the out-of-line release
};

class C2DVector { public: float X, Y; };

struct __false_type { __false_type() {} };
template<class T> inline void _Destroy(T* pointer) { pointer->~T(); }
template<class ForwardIterator> void __destroy_aux(ForwardIterator first, ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template<class ForwardIterator> inline void _Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template<class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            std::free(_M_start);
    }
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

class CThingCreatureBase;
class CAttackingCombatantInfo {
public:
    CThingCreatureBase* Attacked;
    long AttackerNumber;
    long CombatWheelRingIndex;
    bool PreferredAttackDollySet;
    long PreferredAttackDollyTimer;
    C2DVector PreferredAttackDollyVector;
    bool UsingCombatWheel;
    long CurrentSequence;
    vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;
};

class CDefendingCombatantInfo {
public:
    unsigned char _body[0x3c];
    ~CDefendingCombatantInfo() { ++gDefenderDtors; }   // stand-in for 0x006F48A0
};

class CCombatantInfo {
public:
    CThingCreatureBase* Parent;
    CAttackingCombatantInfo AttackerInfo;
    CDefendingCombatantInfo DefenderInfo;
};

typedef pair<const u64, CCombatantInfo> SCombatantByUID;

static void InitValue(SCombatantByUID* value, u64 key, int sequences)
{
    *(u64*)&value->first = key;
    CAttackingCombatantInfo& attacker = value->second.AttackerInfo;
    attacker.CombatSequences._M_start = sequences ? (CCountedPointer<CCombatSequenceBase>*)std::malloc(sequences * sizeof(CCountedPointer<CCombatSequenceBase>)) : 0;
    attacker.CombatSequences._M_finish = attacker.CombatSequences._M_start + sequences;
    attacker.CombatSequences._M_end_of_storage = attacker.CombatSequences._M_finish;
}

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;
};
typedef _Rb_tree_node<SCombatantByUID> SNode;

// stand-in for _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E: unlinks a LEAF z (enough for this test)
static _Rb_tree_node_base* RebalanceForErase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root, _Rb_tree_node_base*& leftmost,
                                             _Rb_tree_node_base*& rightmost)
{
    _Rb_tree_node_base* parent = z->_M_parent;
    if (root == z)
        root = 0;
    else if (parent->_M_left == z)
        parent->_M_left = 0;
    else
        parent->_M_right = 0;
    if (leftmost == z)
        leftmost = parent;
    if (rightmost == z)
        rightmost = parent;
    return z;
}

class CCombatantTree {
public:
    SNode* _M_header;
    unsigned int _M_node_count;
    bool _M_key_compare;
    static SNode*& _S_left(SNode* x) { return (SNode*&)(x->_M_left); }
    static SNode*& _S_right(SNode* x) { return (SNode*&)(x->_M_right); }
    static void _M_deallocate(SNode* x)
    {
        if (x)
            std::free(x);
    }
    CCombatantTree() : _M_header(0)
    {
        _M_header = (SNode*)std::malloc(sizeof(SNode));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    void erase(_Rb_tree_node_base* position)
    {
        SNode* y = (SNode*)RebalanceForErase(position, _M_header->_M_parent, _M_header->_M_left, _M_header->_M_right);
        _Destroy(&y->_M_value_field);
        _M_deallocate(y);
        --_M_node_count;
    }
    void _M_erase(SNode* x)
    {
        while (x != 0) {
            _M_erase(_S_right(x));
            SNode* y = _S_left(x);
            _Destroy(&x->_M_value_field);
            std::free(x);
            x = y;
        }
    }
};

static SNode* MakeNode(u64 key, int sequences)
{
    SNode* n = (SNode*)std::malloc(sizeof(SNode));
    n->_M_color = true;
    n->_M_parent = n->_M_left = n->_M_right = 0;
    InitValue(&n->_M_value_field, key, sequences);
    return n;
}

int main()
{
    CCombatantTree tree;
    SNode* root = MakeNode(20, 1);
    SNode* left = MakeNode(10, 2);
    root->_M_parent = tree._M_header;
    root->_M_left = left;
    left->_M_parent = root;
    tree._M_header->_M_parent = root;
    tree._M_header->_M_left = left;
    tree._M_header->_M_right = root;
    tree._M_node_count = 2;
    tree.erase(left);
    int ok = tree._M_node_count == 1 && root->_M_left == 0 && tree._M_header->_M_left == root && gDefenderDtors == 1 &&
             gSequenceReleases == 2;
    tree.erase(root);
    if (tree._M_node_count != 0 || tree._M_header->_M_parent != 0 || gDefenderDtors != 2 || gSequenceReleases != 3)
        ok = 0;
    if (!ok) {
        std::printf("006f4bc0_TEST FAIL\n");
        return 1;
    }
    std::printf("006f4bc0_TEST PASS\n");
    return 0;
}