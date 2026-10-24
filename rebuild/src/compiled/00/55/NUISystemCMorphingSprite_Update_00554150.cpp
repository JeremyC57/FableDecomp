// NUISystem::CMorphingSprite::Update(float deltaTime) -- advance the morph, run the sprite update, then trim the rendered-handle list to the frames still in use and restart the count
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00554150, 136 bytes (`ret 4`).
// Callees: own virtual slot 150 (+0x258, UpdateMorph(float)), NUISystem::CChangingStateComponent::Update(float)
// @ 0x0052C7E0 (landed; CSprite does not override it), each handle's virtual slot 0 (scalar deleting destructor, `delete`),
// free @ 0x00BFEA14 (the list node).
// Label: CMorphingSprite vtable 0x0124A49C slot 1 (rtti-vtables.tsv); Ego_r.pdb names ?Update@CMorphingSprite@NUISystem@@UAEXM@Z
// and ?UpdateMorph@CMorphingSprite@NUISystem@@MAEXM@Z.
// Layout: Ego_r.pdb CMorphingSprite RenderCount +0x1b4, Handles (list<CEnginePrimitiveHandle*>) +0x1b8, GraphicIndices
// (map) +0x1c4, SpriteGraphicIndex +0x1d4; retail own members sit 0x30 lower and the STLport list is 4 bytes, the map 12:
// RenderCount +0x184, Handles +0x188, GraphicIndices +0x18c, SpriteGraphicIndex +0x198.
// Shape: `if (RenderCount > 0)` (signed) guards a loop that recounts the list (STLport size() walks the nodes) and compares
// it unsigned with RenderCount - 1; each pass deletes the front handle (null-tested virtual deleting destructor with 1)
// and pops the front node (unlink, free). Both counters are then zeroed from the edi zero.
extern "C" void __cdecl free(void* block);       // 0x00BFEA14

class CEnginePrimitiveHandle {
public:
    virtual ~CEnginePrimitiveHandle();             // slot 0
};

namespace NUISystem {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct SHandleNode : _List_node_base {
    CEnginePrimitiveHandle* _M_data;               // +0x08
};
class CHandleList {                                // STLport list<CEnginePrimitiveHandle*> (4 bytes)
public:
    SHandleNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    CEnginePrimitiveHandle*& front() { return ((SHandleNode*)_M_node->_M_next)->_M_data; }
    void pop_front()
    {
        _List_node_base* p = _M_node->_M_next;
        _List_node_base* next = p->_M_next;
        _List_node_base* prev = p->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(p);
    }
};

class CChangingStateComponent {
public:
    virtual void v000();
    virtual void Update(float deltaTime);          // slot 1; 0x0052C7E0
};
class CSprite : public CChangingStateComponent {
};
class CMorphingSprite : public CSprite {
public:
    virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145();
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void UpdateMorph(float deltaTime);     // slot 150 (+0x258)
    unsigned char _pad04[0x184 - 4];
    long          RenderCount;                     // +0x184
    CHandleList   Handles;                         // +0x188
    unsigned char GraphicIndices[0xc];             // +0x18c map<unsigned long, unsigned long>
    unsigned long SpriteGraphicIndex;              // +0x198
    virtual void Update(float deltaTime);
};

void CMorphingSprite::Update(float deltaTime)
{
    UpdateMorph(deltaTime);
    CSprite::Update(deltaTime);
    if (RenderCount > 0) {
        while (Handles.size() > (unsigned long)(RenderCount - 1)) {
            delete Handles.front();
            Handles.pop_front();
        }
    }
    RenderCount = 0;
    SpriteGraphicIndex = 0;
}
}