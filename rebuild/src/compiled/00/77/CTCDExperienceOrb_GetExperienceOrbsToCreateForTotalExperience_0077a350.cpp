// CTCDExperienceOrb::GetExperienceOrbsToCreateForTotalExperience(long totalExperience, vector<long>& orbs) const -- split an experience total into orb values: repeatedly take the def's experience threshold just below the remaining total (lower_bound, stepped back unless at the first), at least 1, append it and subtract it until nothing remains
// Leaf: ?GetExperienceOrbsToCreateForTotalExperience@CTCDExperienceOrb
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077A350, 223 bytes (+1 int3), `ret 8`.
// Name: FableWin.pdb `?GetExperienceOrbsToCreateForTotalExperience@CTCDExperienceOrb@@ABEXJAAV?$vector@JV?$allocator@J@std@@@std@@@Z`
//   (private const); the one caller is CTCDExperienceOrb::Create(CThing&) 0x0077A890. Label correction: catalog
//   "SortedMap_FillFromSortedTree" -> CTCDExperienceOrb::GetExperienceOrbsToCreateForTotalExperience.
// Callees: vector<long>::_M_insert_overflow(pos, const long&, const __true_type&, 1, true) 0x00440990 (the landed
//   0x00BBAC20 push_back model).
// Layout: CTCDExperienceOrb PDef +0x0c (the landed CombineWithExperienceOrb model); CExperienceDef::
//   ParticleScalesPerExperiencePoints +0x2c (Ego_r.pdb, map<long, float>: the thresholds are its keys).
// Shape: `while (total > 0)` over the parameter (kept in ebx, its slot then homes the `1L` temporary of STLport's
//   `max(it->first, 1L)` and the push_back's empty tag); the map's lower_bound and the const_iterator `--` (STLport
//   `_Rb_global<bool>::_M_decrement`) are inlined; the orb value is a named local homed in the dead `orbs` slot.
namespace std {
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template<class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                        // +0x00 (red = false)
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
    static _Base_ptr _M_decrement(_Base_ptr node)
    {
        if (node->_M_color == false && node->_M_parent->_M_parent == node)
            node = node->_M_right;
        else if (node->_M_left != 0) {
            _Base_ptr y = node->_M_left;
            while (y->_M_right != 0)
                y = y->_M_right;
            node = y;
        }
        else {
            _Base_ptr y = node->_M_parent;
            while (node == y->_M_left) {
                node = y;
                y = y->_M_parent;
            }
            node = y;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
    void _M_decrement() { _M_node = _Rb_global_inst::_M_decrement(_M_node); }
};
template<class Value> struct _Rb_tree_const_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_const_iterator(_Base_ptr x) { _M_node = x; }
    const Value* operator->() const { return &((_Rb_tree_node<Value>*)_M_node)->_M_value_field; }
    _Rb_tree_const_iterator& operator--()
    {
        _M_decrement();
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
    typedef _Rb_tree_node<value_type>* _Link_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                        // +0x00 (the allocator proxy's node pointer)
    unsigned int _M_node_count;                           // +0x04
    Compare _M_key_compare;                               // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator lower_bound(const Key& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0) {
            if (!_M_key_compare(((_Link_type)x)->_M_value_field.first, k)) {
                y = x;
                x = x->_M_left;
            }
            else
                x = x->_M_right;
        }
        return const_iterator(y);
    }
};

struct __true_type { __true_type() { } };

template<class T> class vector {
public:
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    T* _M_end_of_storage;                                 // +0x08
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x00440990
    void _M_insert_overflow_00440990(T* pos, T x, unsigned int fill_len, bool atend);   // stand-in callee
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
#pragma auto_inline(off)
// Same-TU stand-in for the STLport COMDAT 0x00440990: it reads the value and never touches the tag or leaks either
// address, so VC7.1 keeps the orb value in esi across the call and homes the temporaries in dead argument slots.
template<class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len,
                                                        bool atend)
{
    _M_insert_overflow_00440990(pos, x, fill_len, atend);
}
#pragma auto_inline(on)
}
inline void* operator new(unsigned int, void* where) { return where; }

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

class CThing;

class CTCDExperienceOrb {
public:
    void* __vftable;                                      // +0x00 (CTCBase)
    CThing* ParentThing;                                  // +0x04
    bool ToBeRemoved;                                     // +0x08
    bool IsInGlobalUpdateTCs;                             // +0x09
    CDefPointer<CExperienceDef> PDef;                     // +0x0c
    long ExperiencePoints;                                // +0x10
private:
    const CExperienceDef& GetDef() const { return *PDef.Object; }
    void GetExperienceOrbsToCreateForTotalExperience(long totalExperience, std::vector<long>& orbs) const;
};

void CTCDExperienceOrb::GetExperienceOrbsToCreateForTotalExperience(long totalExperience, std::vector<long>& orbs) const
{
    while (totalExperience > 0) {
        std::map<long, float>::const_iterator it = GetDef().ParticleScalesPerExperiencePoints.lower_bound(totalExperience);
        if (it != GetDef().ParticleScalesPerExperiencePoints.begin())
            --it;
        long orbExperience = (std::max)(it->first, 1L);
        orbs.push_back(orbExperience);
        totalExperience -= orbExperience;
    }
}