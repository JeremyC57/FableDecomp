// UnholyPowerCreatureMap::operator[](CThingCreatureBase* const& key) -- STLport map<CThingCreatureBase*, CTCUnholyPower::PerCreatureInfo>::operator[]: lower_bound the creature, insert a default per-creature record when absent, return the record
// Leaf: ??A
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798330, 211 bytes (+ int3 padding to 224), __fastcall `ret 4`. Byte
//   twin of 0x007934F0 (the other spell's TU).
// Identity: Ego_r.pdb names `CTCUnholyPower::PerCreatureInfo` and the map<CThingCreatureBase*, PerCreatureInfo> member
//   LastFrameCreaturesHit (the CTCDivineWrath twin's donor ??A is 0x0079DE20); eight callers in CTCUnholyPower's TU. The catalog's "IcicleAssault_FindInsertionPoint" is descriptive.
// Callees: CBaseIntelligentPointer() 0x00A01B10 (x2), CTCUnholyPower::PerCreatureInfo copy constructor 0x00793A80
//   (out of line; catalogued as "SIcicleRecord_0078e4a0" for the DivineWrath one), STLport `_Rb_tree::insert_unique(
//   iterator, const value_type&)` 0x00797F40 (iterator by value, returned through a hidden pointer), ~CBaseIntelligentPointer
//   0x00A01C10 (x4).
// Layout: node: color +0, parent +4, left +8, right +0xc, key +0x10, PerCreatureInfo +0x14 (Ego_r.pdb: ActualCreature
//   CIntelligentPointer<CThingCreatureBase> +0, FramesToGetHit +8, Effect CIntelligentPointer<CThing> +0xc, DefaultChannel
//   CEnginePrimitiveThemeChannelDesc +0x14 -- its constructor zeroes the first three bytes).
// Shape: STLport `iterator i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first)) i = insert(i, value_type(k,
//   _Tp())); return (*i).second;` -- `_M_lower_bound` inlined (unsigned pointer compare, `jb` to the right child), the
//   default record and the pair temporaries built in the frame (key copied first, the record copied by the out-of-line
//   copy constructor), the insert result homed in the dead key slot, and the four intelligent pointers destroyed in
//   reverse order.
class CBaseObject;
class CThing;
class CThingCreatureBase;

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0;
    unsigned char ChannelIndex1;
    unsigned char ChannelBlendFactor;
    unsigned char Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};

struct PerCreatureInfo {
    CIntelligentPointer<CThingCreatureBase> ActualCreature;   // +0x00 (vtable 0x0123E7D8)
    long                                    FramesToGetHit;   // +0x08
    CIntelligentPointer<CThing>             Effect;           // +0x0c (vtable 0x01238C6C)
    CEnginePrimitiveThemeChannelDesc        DefaultChannel;   // +0x14
    PerCreatureInfo() { }
    PerCreatureInfo(const PerCreatureInfo& other);       // 0x00793A80
};

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

struct UnholyPowerCreatureMap {                                // STLport map<CThingCreatureBase*, PerCreatureInfo> (12 bytes)
    typedef CThingCreatureBase* key_type;
    typedef pair<CThingCreatureBase* const, PerCreatureInfo> value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                       // +0x10 key, +0x14 info
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        iterator(const iterator& o) : _M_node(o._M_node) { }
        value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator==(const iterator& o) const { return _M_node == o._M_node; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    less<key_type>      _M_key_compare;

    static const key_type& _S_key(_Rb_tree_node_base* x) { return static_cast<_Node*>(x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const key_type& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator lower_bound(const key_type& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    less<key_type> key_comp() const { return _M_key_compare; }
    iterator insert_unique(iterator position, const value_type& v);   // 0x00797F40
    iterator insert(iterator position, const value_type& v) { return insert_unique(position, v); }

    PerCreatureInfo& operator[](const key_type& k);
};

PerCreatureInfo& UnholyPowerCreatureMap::operator[](const key_type& k)
{
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first))
        i = insert(i, value_type(k, PerCreatureInfo()));
    return (*i).second;
}