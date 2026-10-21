// NUISystem::CTable::Initialise() -- re-initialise a table: base initialise, rebuild its sprites, drop the sprite map and both separation vectors, and re-parent every child
// Leaf: ?Initialise@CTable@NUISystem@@QAEXXZ (Ego_r.pdb UAEXXZ @ 0x005CB190; virtual, modelled non-virtual)
// Retail: Fable.exe @ 0x00550C60, 238 bytes, plain `ret`.
// Body: only when PHorizontalSeparations and PVerticalSeparations are both set: CChangingStateComponent::Initialise()
// (0x0052C730, non-virtual base call); virtual slot 142 (+0x238, 0x00551EA0); Sprites.clear() (STLport _Rb_tree::clear:
// _M_erase(root) 0x00553690, leftmost = rightmost = header, root = 0, count = 0); delete PHorizontalSeparations and
// delete PVerticalSeparations (inline ~vector<unsigned long>: free(_M_start) 0x00BFEA14 when set; operator delete
// 0x00BFE9BC), each pointer then zeroed; for every child i: Children[i]->slot 51 (+0xcc, 0x0052F330)(this).
// Names: slot 142 / slot 51 targets are not recovered (CTable's protected virtuals include ConstructSpritesToDraw).
// Layout: retail CComponent = Ego_r - 8 past the STLport map (Children +0xb0, Ego_r +0xb8; 8-byte CCountedPointer
// elements); retail CTable: Sprites +0x15c (Ego_r +0x18c), PHorizontalSeparations +0x168, PVerticalSeparations +0x16c
// (Ego_r +0x19c / +0x1a0). The generated engine headers carry Ego_r offsets, so a local layout is used.
// Opt: /O2 (16-aligned entry).
#include <stdlib.h>
namespace std {
struct _Rb_tree_node_base {
    int                 _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class K, class V> class map {
public:
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    int                 _M_key_compare;
    void _M_erase(_Rb_tree_node_base* x);                // 0x00553690 for <ETableSprites, CCountedPointer<CComponent>>
    void clear()
    {
        if (_M_node_count != 0)
        {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector() { if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> class CCountedPointer {
public:
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
};
namespace NUISystem {
enum ETableSprites { TABLE_SPRITE_0 };
class CComponent {
public:
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006(); virtual void _v007();
    virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023();
    virtual void _v024(); virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030(); virtual void _v031();
    virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039();
    virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047();
    virtual void _v048(); virtual void _v049(); virtual void _v050();
    virtual void ChildSlot51(CComponent* table);         // slot 51 (+0xcc; name not recovered)
    virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055(); virtual void _v056(); virtual void _v057(); virtual void _v058(); virtual void _v059();
    virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063(); virtual void _v064(); virtual void _v065(); virtual void _v066(); virtual void _v067();
    virtual void _v068(); virtual void _v069(); virtual void _v070(); virtual void _v071(); virtual void _v072(); virtual void _v073(); virtual void _v074(); virtual void _v075();
    virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079(); virtual void _v080(); virtual void _v081(); virtual void _v082(); virtual void _v083();
    virtual void _v084(); virtual void _v085(); virtual void _v086(); virtual void _v087(); virtual void _v088(); virtual void _v089(); virtual void _v090(); virtual void _v091();
    virtual void _v092(); virtual void _v093(); virtual void _v094(); virtual void _v095(); virtual void _v096(); virtual void _v097(); virtual void _v098(); virtual void _v099();
    virtual void _v100(); virtual void _v101(); virtual void _v102(); virtual void _v103(); virtual void _v104(); virtual void _v105(); virtual void _v106(); virtual void _v107();
    virtual void _v108(); virtual void _v109(); virtual void _v110(); virtual void _v111(); virtual void _v112(); virtual void _v113(); virtual void _v114(); virtual void _v115();
    virtual void _v116(); virtual void _v117(); virtual void _v118(); virtual void _v119(); virtual void _v120(); virtual void _v121(); virtual void _v122(); virtual void _v123();
    virtual void _v124(); virtual void _v125(); virtual void _v126(); virtual void _v127(); virtual void _v128(); virtual void _v129(); virtual void _v130(); virtual void _v131();
    virtual void _v132(); virtual void _v133(); virtual void _v134(); virtual void _v135(); virtual void _v136(); virtual void _v137(); virtual void _v138(); virtual void _v139();
    virtual void _v140(); virtual void _v141();
    virtual void TableSlot142();                         // slot 142 (+0x238; name not recovered)
    unsigned char                                 _pad004[0xb0 - 4];
    std::vector< CCountedPointer<CComponent> >   Children;   // +0xb0
};
class CChangingStateComponent : public CComponent {
public:
    void Initialise();                                   // 0x0052C730
};
class CTable : public CChangingStateComponent {
public:
    unsigned char                                          _pad0bc[0x15c - 0xbc];
    std::map<ETableSprites, CCountedPointer<CComponent> >  Sprites;                  // +0x15c
    std::vector<unsigned long>*                            PHorizontalSeparations;   // +0x168
    std::vector<unsigned long>*                            PVerticalSeparations;     // +0x16c
    void Initialise();
};
void CTable::Initialise()
{
    if (PHorizontalSeparations && PVerticalSeparations)
    {
        CChangingStateComponent::Initialise();
        TableSlot142();
        Sprites.clear();
        delete PHorizontalSeparations;
        PHorizontalSeparations = 0;
        delete PVerticalSeparations;
        PVerticalSeparations = 0;
        for (unsigned int i = 0; i < Children.size(); ++i)
            Children[i]->ChildSlot51(this);
    }
}
}