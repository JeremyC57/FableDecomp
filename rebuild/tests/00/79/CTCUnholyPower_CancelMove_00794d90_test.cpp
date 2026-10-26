#include <cstdio>

// Model: a hand-built STLport red-black tree of three creatures; CancelMove must hand each creature (in key order) to
// ResetCreatureHitState as a by-value copy, leave no extra live pointer copies behind, then call FinishMove once.
class CThingCreatureBase { public: int Id; };

static int g_liveCopies = 0;

struct CBaseIntelligentPointer {
    CThingCreatureBase* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_liveCopies; }
    ~CBaseIntelligentPointer() { --g_liveCopies; }
    CThingCreatureBase* GetPItem() const { return m_pItem; }
    void SetPItem(CThingCreatureBase* item) { m_pItem = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
struct PerCreatureInfo {
    CIntelligentPointer<CThingCreatureBase> ActualCreature;
    long FramesToGetHit;
};
struct _Node : _Rb_tree_node_base {
    CThingCreatureBase* Key;
    PerCreatureInfo     Info;
};
struct CCreatureInfoMap {
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        _Node& operator*() const { return *static_cast<_Node*>(_M_node); }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        iterator& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    };
    _Rb_tree_node_base* _M_header;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};

class CTCUnholyPower {
public:
    CCreatureInfoMap LastFrameCreaturesHit;
    int Seen[8];
    int NumSeen;
    int Finished;
    void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> creature) { Seen[NumSeen++] = creature.GetPItem()->Id; }
    void FinishMove() { ++Finished; }
    void CancelMove();
};

void CTCUnholyPower::CancelMove()
{
    CCreatureInfoMap::iterator end = LastFrameCreaturesHit.end();
    for (CCreatureInfoMap::iterator it = LastFrameCreaturesHit.begin(); it != end; ++it) {
        CIntelligentPointer<CThingCreatureBase> creature = (*it).Info.ActualCreature;
        ResetCreatureHitState(creature);
    }
    FinishMove();
}

int main()
{
    CThingCreatureBase c[3];
    _Node header, n[3];
    for (int i = 0; i < 3; ++i) {
        c[i].Id = 10 * (i + 1);
        n[i].Key = &c[i];
        n[i].Info.ActualCreature.SetPItem(&c[i]);
        n[i]._M_left = n[i]._M_right = 0;
    }
    // root n[1] with children n[0] (left) and n[2] (right); header: parent = root, left = leftmost, right = rightmost
    header._M_parent = &n[1];
    header._M_left = &n[0];
    header._M_right = &n[2];
    n[1]._M_parent = &header;
    n[1]._M_left = &n[0];
    n[1]._M_right = &n[2];
    n[0]._M_parent = &n[1];
    n[2]._M_parent = &n[1];
    CTCUnholyPower power;
    power.LastFrameCreaturesHit._M_header = &header;
    power.NumSeen = 0;
    power.Finished = 0;
    int before = g_liveCopies;
    power.CancelMove();
    bool ok = power.NumSeen == 3 && power.Seen[0] == 10 && power.Seen[1] == 20 && power.Seen[2] == 30
              && power.Finished == 1 && g_liveCopies == before;
    // an empty map: only FinishMove
    _Node empty;
    empty._M_parent = 0;
    empty._M_left = empty._M_right = &empty;
    power.LastFrameCreaturesHit._M_header = &empty;
    power.CancelMove();
    ok = ok && power.NumSeen == 3 && power.Finished == 2;
    if (!ok) {
        std::printf("00794d90_TEST FAIL\n");
        return 1;
    }
    std::printf("00794d90_TEST PASS\n");
    return 0;
}