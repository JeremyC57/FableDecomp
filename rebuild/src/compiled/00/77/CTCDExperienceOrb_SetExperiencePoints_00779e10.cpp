// CTCDExperienceOrb::SetExperiencePoints(long points) -- store the orb's experience points and scale its appearance by the def's particle scale for the highest experience threshold not above the points (1.0 below the first threshold)
// Leaf: ?SetExperiencePoints@CTCDExperienceOrb
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00779E10, 128 bytes (`ret 4` through the tail jump).
// Name: FableWin.pdb `?SetExperiencePoints@CTCDExperienceOrb@@QAEXJ@Z`; the landed CTCDExperienceOrb::
//   CombineWithExperienceOrb 0x0077A130 calls it at this address, as do CTCDExperienceOrb::Create 0x0077A890 and
//   CGameScriptInterface::CreateExperienceOrb 0x0089FBB0 (landed). Label correction: catalog "CExperienceOrb_SetScale" ->
//   CTCDExperienceOrb::SetExperiencePoints.
// Callees: NGraphicAppearance::CTCGraphicAppearance::SetScale(float) 0x004BFA50 (tail jump).
// Layout: CTCDExperienceOrb ParentThing +0x04, PDef +0x0c, ExperiencePoints +0x10 (the landed CombineWithExperienceOrb
//   model); CExperienceDef::ParticleScalesPerExperiencePoints +0x2c (Ego_r.pdb, map<long, float>: STLport header pointer,
//   node count, key compare); CThing::PTCGraphicAppearance +0x64 (retail; PDB +0x74).
// Shape: a `for` over the map from begin() with `break` once the threshold exceeds the points; the scale lives in the
//   dead `points` argument slot and the end() test re-reads the def through `this` each pass.
namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template<class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                        // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0c
};
template<class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                                 // +0x10
};

template<class Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        }
        else {
            _Base_ptr y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};
template<class Value> struct _Rb_tree_const_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_const_iterator(_Base_ptr x) { _M_node = x; }
    const Value& operator*() const { return ((_Rb_tree_node<Value>*)_M_node)->_M_value_field; }
    const Value* operator->() const { return &((_Rb_tree_node<Value>*)_M_node)->_M_value_field; }
    _Rb_tree_const_iterator& operator++()
    {
        _M_increment();
        return *this;
    }
};
template<class Value>
inline bool operator!=(const _Rb_tree_const_iterator<Value>& x, const _Rb_tree_const_iterator<Value>& y)
{
    return x._M_node != y._M_node;
}

template<class Key, class T, class Compare = less<Key> > class map {
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                        // +0x00 (the allocator proxy's node pointer)
    unsigned int _M_node_count;                           // +0x04
    Compare _M_key_compare;                               // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};
}

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    void SetScale(float scale);                           // 0x004BFA50
};
}

class CThing {
public:
    unsigned char _pad_0x00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64 (retail)
};

class CExperienceDef {
public:
    unsigned char _pad_0x00[0x28];
    long OrbCombiningParticleEffect;                                  // +0x28
    std::map<long, float> ParticleScalesPerExperiencePoints;          // +0x2c
};

template<class T> class CDefPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
};

class CTCDExperienceOrb {
public:
    void* __vftable;                                      // +0x00 (CTCBase)
    CThing* ParentThing;                                  // +0x04
    bool ToBeRemoved;                                     // +0x08
    bool IsInGlobalUpdateTCs;                             // +0x09
    CDefPointer<CExperienceDef> PDef;                     // +0x0c
    long ExperiencePoints;                                // +0x10
    const CExperienceDef& GetDef() const { return *PDef.Object; }
    CThing* GetParent() const { return ParentThing; }
    void SetExperiencePoints(long points);
};

void CTCDExperienceOrb::SetExperiencePoints(long points)
{
    ExperiencePoints = points;
    float scale = 1.0f;
    for (std::map<long, float>::const_iterator it = GetDef().ParticleScalesPerExperiencePoints.begin();
         it != GetDef().ParticleScalesPerExperiencePoints.end(); ++it) {
        if (points < it->first)
            break;
        scale = it->second;
    }
    GetParent()->PTCGraphicAppearance->SetScale(scale);
}