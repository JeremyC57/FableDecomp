// CTCDivineWrath::CancelMove() -- cancel the spell: re-arm every creature hit last frame (ResetCreatureHitState on a copy of its pointer), then finish the move
// Leaf: ?CancelMove@CTCDivineWrath@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078F820, 203 bytes (+ int3 padding to 208), __fastcall, plain `ret`,
//   8-aligned frame (`push ebp ; mov ebp,esp ; and esp,-8`). Byte twin of CTCUnholyPower::CancelMove 0x00794D90 (only the two callees differ).
// Identity: Ego_r.pdb `?CancelMove@CTCDivineWrath@@QAEXXZ` (donor 0x00795500, 0xD0 bytes as here); its callees are the landed
//   CTCDivineWrath::ResetCreatureHitState 0x0078E900 and CTCDivineWrath::FinishMove 0x0078F6E0; three external callers. The
//   catalog's "CTCDivineWrath_CancelAbilities" is descriptive.
// Callees: CBaseIntelligentPointer() 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (two inline copy constructors of
//   CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8), CTCDivineWrath::ResetCreatureHitState(CIntelligentPointer<
//   CThingCreatureBase>) 0x0078E900 (by value, destroyed by the callee), ~CBaseIntelligentPointer 0x00A01C10,
//   CTCDivineWrath::FinishMove() 0x0078F6E0.
// Layout: Ego_r.pdb CTCDivineWrath::LastFrameCreaturesHit at +0x7c, retail +0x78 (the 12-byte STLport map<CThingCreatureBase*,
//   PerCreatureInfo>: header pointer, count, empty comparator); node value: key +0x10, PerCreatureInfo +0x14 whose first
//   member is ActualCreature (CIntelligentPointer<CThingCreatureBase>).
// Shape: `end()` in a named local (spilled to the aligned frame: ebp is the frame pointer), the loop rotated with `++it`
//   the inlined STLport `_Rb_global<bool>::_M_increment`; per entry a named copy of ActualCreature is copied again into
//   the by-value argument and destroyed after the call. The `and esp,-8` frame needs `__declspec(align(8))` on the loop
//   copy (a plain copy, a default-constructed local assigned, or a temporary argument all give an ebp-allocated `end`
//   and no aligned frame; the temporary is also elided into the argument slot).
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0123E7D8 (the copy's)
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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

template <class K, class V> struct pair {
    K first;
    V second;
};

struct PerCreatureInfo {
    CIntelligentPointer<CThingCreatureBase> ActualCreature;   // +0x00
    long                                    FramesToGetHit;   // +0x08
    CIntelligentPointer<CThing>             Effect;           // +0x0c
    unsigned long                           DefaultChannel;   // +0x14 CEnginePrimitiveThemeChannelDesc
};

struct CCreatureInfoMap {                                // STLport map<CThingCreatureBase*, PerCreatureInfo> (12 bytes)
    typedef pair<CThingCreatureBase* const, PerCreatureInfo> value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                       // +0x10 (key) / +0x14 (info)
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        iterator& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;

    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};

class CTCDivineWrath {
public:
    unsigned char    _pad00[0x78];
    CCreatureInfoMap LastFrameCreaturesHit;              // +0x78

    void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> creature);   // 0x0078E900
    void FinishMove();                                   // 0x0078F6E0
    void CancelMove();
};

void CTCDivineWrath::CancelMove()
{
    CCreatureInfoMap::iterator end = LastFrameCreaturesHit.end();
    for (CCreatureInfoMap::iterator it = LastFrameCreaturesHit.begin(); it != end; ++it) {
        __declspec(align(8)) CIntelligentPointer<CThingCreatureBase> creature = (*it).second.ActualCreature;
        ResetCreatureHitState(creature);
    }
    FinishMove();
}