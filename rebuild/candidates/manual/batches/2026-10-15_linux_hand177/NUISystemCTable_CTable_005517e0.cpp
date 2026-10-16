// NUISystem::CTable::CTable(long owner) -- CChangingStateComponent(owner), the three CTable vtables, an empty Sprites map and no separation vectors, then GetDataFromDef()
// Leaf: ??0CTable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005517E0, length 119 bytes, `ret 4`.
// Label correction: the catalog's "CViewport::CViewport" is propagated: the vtables 0x0124A224 / 0x0124A1FC / 0x0124A1F4 are
// NUISystem::CTable (rtti-vtables.tsv).
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50 (manifest "CDraggableInto"),
// malloc @ 0x00BFEA0E (the map's 0x1c-byte header node), NUISystem::CTable::GetDataFromDef @ 0x00551340.
// Layout (retail = Ego_r.pdb CTable minus 0x30, as the landed CZoomingComponent 0x00546B00): Sprites
// map<ETableSprites, CCountedPointer<CComponent> > +0x15c (header, count +0x160, compare +0x164), PHorizontalSeparations
// +0x168, PVerticalSeparations +0x16c. Shape: the STLport _Rb_tree base/derived ctor pair of the landed 0x00E1A800 model.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
namespace NUISystem {
struct CComponentInterface {                       // primary vtable at +0
    virtual void v000();
};
struct CObserver_Retail {                          // vtable at +4, then its 0x10 bytes of state
    virtual void ProcessEvent();
    unsigned char _body[0x10];
};
struct CEventSinkInterface {                       // vtable at +0x18
    virtual void v0();
};
struct CChangingStateComponent : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x15C bytes
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long owner);           // 0x0052CC50
};
struct _Rb_tree_node_base {
    char                _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template <unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
struct CULongVector;
struct CTable : CChangingStateComponent {          // vtables 0x0124A224 / 0x0124A1FC / 0x0124A1F4
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v0();
    CRetailMap<0x1c> Sprites;                      // +0x15c map<ETableSprites, CCountedPointer<CComponent> >
    CULongVector*    PHorizontalSeparations;       // +0x168
    CULongVector*    PVerticalSeparations;         // +0x16c
    CTable(long owner);
    void GetDataFromDef();                         // 0x00551340
};
CTable::CTable(long owner)
    : CChangingStateComponent(owner)
    , Sprites()
    , PHorizontalSeparations(0)
    , PVerticalSeparations(0)
{
    GetDataFromDef();
}
}
