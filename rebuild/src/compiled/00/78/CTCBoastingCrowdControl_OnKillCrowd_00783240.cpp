// CTCBoastingCrowdControl::OnKillCrowd(CThing& hero) -- forget the boasting crowd recorded for this hero: find the hero's UID in the hero -> crowd-thing map and erase that entry
// Leaf: ?OnKillCrowd@CTCBoastingCrowdControl@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783240, 99 bytes (+ int3 padding to 112), __fastcall `ret 4`.
// Identity: Ego_r.pdb `?OnKillCrowd@CTCBoastingCrowdControl@@QAEXAAVCThing@@@Z` (the only CTCBoastingCrowdControl member
//   taking just a CThing&); its caller 0x00784C7C fetches interface 0x73 (CTCBoastingCrowdControl) and passes a
//   GetPItem() thing. Same map and find as the landed CTCBoastingCrowdControl::GetCrowdGroup 0x00783730.
// Callees: map<unsigned __int64, CIntelligentPointer<CThing>>::find @ 0x00782E90 (node pointer in eax), STLport
//   `_Rb_global<bool>::_Rebalance_for_erase(z, root&, leftmost&, rightmost&)` @ 0x0042A34E (__cdecl; catalogued as the
//   propagated CMemoryAllocatorVariableSize::Erase), ~CBaseIntelligentPointer @ 0x00A01C10, free @ 0x00BFEA14.
// Layout: PDB CTCBoastingCrowdControl: Active +0x0c, HeroCrowdThings +0x10 (STLport map: header pointer, count);
//   node value pair<const unsigned __int64, CIntelligentPointer<CThing>> at +0x10 (key) / +0x18 (pointer); retail
//   CThing UniqueID +0x18.
// Shape: `it = HeroCrowdThings.find(hero.GetUID()); if (it != end()) HeroCrowdThings.erase(it);` -- the by-value UID
//   getter result is copied into the frame and its address passed to find; STLport's `erase(iterator)` is inlined:
//   rebalance with the header's parent/left/right addresses, destroy the node's value (only the intelligent pointer has
//   a destructor), `deallocate` = `if (p) free(p)`, then `--_M_node_count`.
extern "C" void __cdecl free(void* block);               // 0x00BFEA14

class CThing {
public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                           // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };

struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                            _Rb_tree_node_base*& leftmost,
                                                            _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};

template <class K, class V> struct pair {
    K first;
    V second;
};

template <class T> inline void _Destroy(T* p) { p->~T(); }

struct CHeroCrowdMap {                                   // STLport map<unsigned __int64, CIntelligentPointer<CThing>>
    typedef pair<const unsigned __int64, CIntelligentPointer<CThing> > value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                       // +0x10
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;

    _Rb_tree_node_base* find(const unsigned __int64& key);   // 0x00782E90 (the iterator's node comes back in eax)
    _Rb_tree_node_base* end() { return _M_header; }
    void erase(_Rb_tree_node_base* pos)
    {
        _Node* y = static_cast<_Node*>(_Rb_global::_Rebalance_for_erase(pos, _M_header->_M_parent,
                                                                         _M_header->_M_left, _M_header->_M_right));
        _Destroy(&y->_M_value_field);
        if (y)
            free(y);
        --_M_node_count;
    }
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
};

class CTCBoastingCrowdControl : public CTCBase {
public:
    bool          Active;                                // +0x0c
    CHeroCrowdMap HeroCrowdThings;                       // +0x10
    bool          CrowdSizesSuppressed;                  // +0x18

    void OnKillCrowd(CThing& hero);
};

void CTCBoastingCrowdControl::OnKillCrowd(CThing& hero)
{
    _Rb_tree_node_base* it = HeroCrowdThings.find(hero.GetUID());
    if (it != HeroCrowdThings.end())
        HeroCrowdThings.erase(it);
}