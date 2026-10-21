#include <cstdio>
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

#include <stdio.h>
void NUISystem::CComponent::_v000() {}
void NUISystem::CComponent::_v001() {}
void NUISystem::CComponent::_v002() {}
void NUISystem::CComponent::_v003() {}
void NUISystem::CComponent::_v004() {}
void NUISystem::CComponent::_v005() {}
void NUISystem::CComponent::_v006() {}
void NUISystem::CComponent::_v007() {}
void NUISystem::CComponent::_v008() {}
void NUISystem::CComponent::_v009() {}
void NUISystem::CComponent::_v010() {}
void NUISystem::CComponent::_v011() {}
void NUISystem::CComponent::_v012() {}
void NUISystem::CComponent::_v013() {}
void NUISystem::CComponent::_v014() {}
void NUISystem::CComponent::_v015() {}
void NUISystem::CComponent::_v016() {}
void NUISystem::CComponent::_v017() {}
void NUISystem::CComponent::_v018() {}
void NUISystem::CComponent::_v019() {}
void NUISystem::CComponent::_v020() {}
void NUISystem::CComponent::_v021() {}
void NUISystem::CComponent::_v022() {}
void NUISystem::CComponent::_v023() {}
void NUISystem::CComponent::_v024() {}
void NUISystem::CComponent::_v025() {}
void NUISystem::CComponent::_v026() {}
void NUISystem::CComponent::_v027() {}
void NUISystem::CComponent::_v028() {}
void NUISystem::CComponent::_v029() {}
void NUISystem::CComponent::_v030() {}
void NUISystem::CComponent::_v031() {}
void NUISystem::CComponent::_v032() {}
void NUISystem::CComponent::_v033() {}
void NUISystem::CComponent::_v034() {}
void NUISystem::CComponent::_v035() {}
void NUISystem::CComponent::_v036() {}
void NUISystem::CComponent::_v037() {}
void NUISystem::CComponent::_v038() {}
void NUISystem::CComponent::_v039() {}
void NUISystem::CComponent::_v040() {}
void NUISystem::CComponent::_v041() {}
void NUISystem::CComponent::_v042() {}
void NUISystem::CComponent::_v043() {}
void NUISystem::CComponent::_v044() {}
void NUISystem::CComponent::_v045() {}
void NUISystem::CComponent::_v046() {}
void NUISystem::CComponent::_v047() {}
void NUISystem::CComponent::_v048() {}
void NUISystem::CComponent::_v049() {}
void NUISystem::CComponent::_v050() {}
void NUISystem::CComponent::_v052() {}
void NUISystem::CComponent::_v053() {}
void NUISystem::CComponent::_v054() {}
void NUISystem::CComponent::_v055() {}
void NUISystem::CComponent::_v056() {}
void NUISystem::CComponent::_v057() {}
void NUISystem::CComponent::_v058() {}
void NUISystem::CComponent::_v059() {}
void NUISystem::CComponent::_v060() {}
void NUISystem::CComponent::_v061() {}
void NUISystem::CComponent::_v062() {}
void NUISystem::CComponent::_v063() {}
void NUISystem::CComponent::_v064() {}
void NUISystem::CComponent::_v065() {}
void NUISystem::CComponent::_v066() {}
void NUISystem::CComponent::_v067() {}
void NUISystem::CComponent::_v068() {}
void NUISystem::CComponent::_v069() {}
void NUISystem::CComponent::_v070() {}
void NUISystem::CComponent::_v071() {}
void NUISystem::CComponent::_v072() {}
void NUISystem::CComponent::_v073() {}
void NUISystem::CComponent::_v074() {}
void NUISystem::CComponent::_v075() {}
void NUISystem::CComponent::_v076() {}
void NUISystem::CComponent::_v077() {}
void NUISystem::CComponent::_v078() {}
void NUISystem::CComponent::_v079() {}
void NUISystem::CComponent::_v080() {}
void NUISystem::CComponent::_v081() {}
void NUISystem::CComponent::_v082() {}
void NUISystem::CComponent::_v083() {}
void NUISystem::CComponent::_v084() {}
void NUISystem::CComponent::_v085() {}
void NUISystem::CComponent::_v086() {}
void NUISystem::CComponent::_v087() {}
void NUISystem::CComponent::_v088() {}
void NUISystem::CComponent::_v089() {}
void NUISystem::CComponent::_v090() {}
void NUISystem::CComponent::_v091() {}
void NUISystem::CComponent::_v092() {}
void NUISystem::CComponent::_v093() {}
void NUISystem::CComponent::_v094() {}
void NUISystem::CComponent::_v095() {}
void NUISystem::CComponent::_v096() {}
void NUISystem::CComponent::_v097() {}
void NUISystem::CComponent::_v098() {}
void NUISystem::CComponent::_v099() {}
void NUISystem::CComponent::_v100() {}
void NUISystem::CComponent::_v101() {}
void NUISystem::CComponent::_v102() {}
void NUISystem::CComponent::_v103() {}
void NUISystem::CComponent::_v104() {}
void NUISystem::CComponent::_v105() {}
void NUISystem::CComponent::_v106() {}
void NUISystem::CComponent::_v107() {}
void NUISystem::CComponent::_v108() {}
void NUISystem::CComponent::_v109() {}
void NUISystem::CComponent::_v110() {}
void NUISystem::CComponent::_v111() {}
void NUISystem::CComponent::_v112() {}
void NUISystem::CComponent::_v113() {}
void NUISystem::CComponent::_v114() {}
void NUISystem::CComponent::_v115() {}
void NUISystem::CComponent::_v116() {}
void NUISystem::CComponent::_v117() {}
void NUISystem::CComponent::_v118() {}
void NUISystem::CComponent::_v119() {}
void NUISystem::CComponent::_v120() {}
void NUISystem::CComponent::_v121() {}
void NUISystem::CComponent::_v122() {}
void NUISystem::CComponent::_v123() {}
void NUISystem::CComponent::_v124() {}
void NUISystem::CComponent::_v125() {}
void NUISystem::CComponent::_v126() {}
void NUISystem::CComponent::_v127() {}
void NUISystem::CComponent::_v128() {}
void NUISystem::CComponent::_v129() {}
void NUISystem::CComponent::_v130() {}
void NUISystem::CComponent::_v131() {}
void NUISystem::CComponent::_v132() {}
void NUISystem::CComponent::_v133() {}
void NUISystem::CComponent::_v134() {}
void NUISystem::CComponent::_v135() {}
void NUISystem::CComponent::_v136() {}
void NUISystem::CComponent::_v137() {}
void NUISystem::CComponent::_v138() {}
void NUISystem::CComponent::_v139() {}
void NUISystem::CComponent::_v140() {}
void NUISystem::CComponent::_v141() {}
static int g_order[8]; static int g_n;
static NUISystem::CComponent* g_parentOf[4]; static NUISystem::CComponent* g_kids[4]; static int g_nkids;
void NUISystem::CComponent::ChildSlot51(NUISystem::CComponent* t) { for (int i = 0; i < 4; ++i) if (g_kids[i] == this) g_parentOf[i] = t; }
void NUISystem::CComponent::TableSlot142() { g_order[g_n++] = 2; }
void NUISystem::CChangingStateComponent::Initialise() { g_order[g_n++] = 1; }
static int g_erased; static std::_Rb_tree_node_base* g_erasedRoot;
template <class K, class V> void std::map<K, V>::_M_erase(std::_Rb_tree_node_base* x) { ++g_erased; g_erasedRoot = x; g_order[g_n++] = 3; }
static NUISystem::CTable g_t;
static std::_Rb_tree_node_base g_hdr, g_root;
static CCountedPointer<NUISystem::CComponent> g_child[3];
static NUISystem::CComponent g_c[3];
static std::vector<unsigned long>* MakeVec(int n)
{
    std::vector<unsigned long>* v = new std::vector<unsigned long>;
    v->_M_start = n ? (unsigned long*)malloc(n * sizeof(unsigned long)) : 0; v->_M_finish = v->_M_start + n; v->_M_end_of_storage = v->_M_finish;
    return v;
}
int main()
{
    for (int i = 0; i < 3; ++i) { g_child[i].Object = &g_c[i]; g_kids[i] = &g_c[i]; }
    g_t.Children._M_start = g_child; g_t.Children._M_finish = g_child + 3;
    g_hdr._M_parent = &g_root; g_t.Sprites._M_header = &g_hdr; g_t.Sprites._M_node_count = 2;
    g_t.PHorizontalSeparations = MakeVec(3);
    g_t.PVerticalSeparations = 0;
    g_t.Initialise();                      // one pointer missing: nothing
    bool ok = g_n == 0 && g_t.PHorizontalSeparations != 0 && g_parentOf[0] == 0;
    g_t.PVerticalSeparations = MakeVec(0);
    g_t.Initialise();
    ok = ok && g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3 && g_erasedRoot == &g_root
        && g_hdr._M_left == &g_hdr && g_hdr._M_right == &g_hdr && g_hdr._M_parent == 0 && g_t.Sprites._M_node_count == 0
        && g_t.PHorizontalSeparations == 0 && g_t.PVerticalSeparations == 0
        && g_parentOf[0] == &g_t && g_parentOf[1] == &g_t && g_parentOf[2] == &g_t;
    g_t.PHorizontalSeparations = MakeVec(1); g_t.PVerticalSeparations = MakeVec(2);
    g_t.Initialise();                      // empty map: no erase
    ok = ok && g_erased == 1 && g_n == 5 && g_t.PVerticalSeparations == 0;
    g_t.Children._M_start = 0; g_t.Children._M_finish = 0;
    printf(ok ? "00550c60_TEST PASS\n" : "00550c60_TEST FAIL\n");
    return ok ? 0 : 1;
}