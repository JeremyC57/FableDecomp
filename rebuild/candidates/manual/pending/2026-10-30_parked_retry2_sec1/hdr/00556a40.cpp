// CRedefinerList::RefreshScriptThings(const CArray<CRedefinition>& redefinitions) -- for every redefinition (skipping the first type-0x38 entry and the kinds/values that never get a widget) whose type the source's widget-type map knows, create a front-end component for it, initialise it and queue it; then attach every scripted thing of the source's thing map to the queued components it belongs to; add the queued components in the source's display order, then any left over, setting each one's font and clear colour as it is added
// Grade: DIFFER(1441v1457) -- all four phases and both switches in shape; retail keeps the `first` flag in memory
//   (frame 0x1c vs 0x18) with the redefinition array in ebx, caches the type key in ebp across the first widget-type
//   push, and decrements the source ref-count through a register copy. Five shape variants tried.
// Leaf: RefreshScriptThings
// Retail: Fable.exe (TLC, VC7.1) @ 0x00556A40, 1386 bytes (+ switch tables, int3 padded to 1472), `ret 4`.
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041D21B, widget-type map find
//   0x005577A0, CRedefinerList::GetScriptThingComponent 0x00556930, CRedefinerList::AddComponent 0x00533CC0,
//   counted-pointer deleter 0x00428B10, malloc 0x00BFEA0E / free 0x00BFEA14, operator new 0x00BFEA1A / delete 0x00BFE9BC.
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int bytes);
extern "C" void __cdecl free(void* p);
inline void* operator new(unsigned int, void* p) throw() { return p; }
struct CThing;

void __fastcall DeleteComponentData(void* data);   // 0x00428B10

template <class T> struct CListNode {
    CListNode* _M_next;
    CListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CList {                   // STLport list<T> over the malloc allocator
    CListNode<T>* _M_node;
    CList()
    {
        _M_node = (CListNode<T>*)malloc(sizeof(CListNode<T>));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~CList()
    {
        clear();
        free(_M_node);
    }
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& front() { return _M_node->_M_next->_M_data; }
    void push_back(const T& value)
    {
        CListNode<T>* position = _M_node;
        CListNode<T>* node = (CListNode<T>*)malloc(sizeof(CListNode<T>));
        new (&node->_M_data) T(value);
        CListNode<T>* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
    }
    void remove(const T& value)
    {
        CListNode<T>* first = _M_node->_M_next;
        while (first != _M_node) {
            CListNode<T>* next = first->_M_next;
            if (value == first->_M_data) {
                CListNode<T>* prev = first->_M_prev;
                prev->_M_next = next;
                next->_M_prev = prev;
                free(first);
            }
            first = next;
        }
    }
    void clear()
    {
        CListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            CListNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CRedefinition {                              // 0x1c bytes
    long Type;
    long Kind;
    long Value;
    long Unused;
    long Index;
    long Pad[2];
};
struct CComponentVT {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void SetColour(const CRGBColour& colour, long a, long b);   // slot 38 (+0x98)
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void Init();   // slot 43 (+0xac)
    virtual void v044(); virtual void v045(); virtual void v046();
    virtual void SetFont(long font, float scale);   // slot 47 (+0xbc)
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149();
    virtual void Setup(const CRedefinition& redefinition, const long* widgetType);   // slot 150 (+0x258)
};
struct CComponent : CComponentVT {
    char        Pad[0x19c - 4];
    CList<long> ScriptTypes;                        // +0x19c
};
struct CManager { CComponent* CreateComponent(long type); };   // 0x0041D21B
struct CFrontEndManager : CManager { static CFrontEndManager* __fastcall GetInstance(); };   // 0x0041E5F2
struct CMapNode {                                   // STLport _Rb_tree_node<pair<const long, T>>
    long      Colour;
    CMapNode* _M_parent;
    CMapNode* _M_left;
    CMapNode* _M_right;
    long      Key;
    long      Value;
};
struct CMapIterator { CMapNode* _M_node; };
struct CWidgetTypeMap {
    CMapNode* _M_header;
    void find(CMapIterator* result, const long* key);   // 0x005577A0
};
struct CRedefinerSourceVT {
    virtual void v000();
    virtual void Destroy();   // slot 1 (+0x4)
};
struct CRedefinerSource : CRedefinerSourceVT {
    long           RefCount;                        // +0x04
    char           Pad0[0x1a4 - 8];
    long           ComponentType;                   // +0x1a4
    CWidgetTypeMap WidgetTypes;                     // +0x1a8
    char           Pad1[0x1b4 - 0x1ac];
    CMapNode*      ThingMapHeader;                  // +0x1b4 map<long, CThing*>
    char           Pad2[0x1c0 - 0x1b8];
    long*          OrderStart;                      // +0x1c0
    long*          OrderFinish;                     // +0x1c4
};
static __forceinline long DecrementCount(long* value)
{
    long result = *value;
    --result;
    *value = result;
    return result;
}
struct CSourcePtr {
    CRedefinerSource* Data;
    ~CSourcePtr()
    {
        CRedefinerSource* data = Data;
        if (data) {
            if (DecrementCount(&data->RefCount) == 0)
                data->Destroy();
            Data = 0;
        }
    }
};
struct CRedefinitionArray { CRedefinition* _M_start; CRedefinition* _M_finish; CRedefinition* _M_end_of_storage; };
struct CRedefinerListVT {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CSourcePtr GetSource();   // slot 108 (+0x1b0)
};
struct CRedefinerList : CRedefinerListVT {
    char Pad[0x14c - 4];
    long Font;                                      // +0x14c
    void AddComponent(const CCountedPointer<CComponent>& component);   // 0x00533CC0
    CComponent* GetScriptThingComponent(CThing* thing, CListNode<CComponent*>** cursor, long type);   // 0x00556930
    void RefreshScriptThings(const CRedefinitionArray& redefinitions);
};

static __forceinline CMapNode* NextNode(CMapNode* node)
{
    if (node->_M_right != 0) {
        node = node->_M_right;
        while (node->_M_left != 0)
            node = node->_M_left;
    }
    else {
        CMapNode* parent = node->_M_parent;
        while (node == parent->_M_right) {
            node = parent;
            parent = parent->_M_parent;
        }
        if (node->_M_right != parent)
            node = parent;
    }
    return node;
}

void CRedefinerList::RefreshScriptThings(const CRedefinitionArray& redefinitions)
{
    CList<CComponent*> pending;
    CListNode<CComponent*>* cursor = pending._M_node;
    CSourcePtr source = GetSource();
    bool first = true;
    for (const CRedefinition* it = redefinitions._M_start; it != redefinitions._M_finish; ++it) {
        if (first && it->Type == 0x38) {
            first = false;
            continue;
        }
        switch (it->Kind) {
        case 0:
        case 1:
            continue;
        case 2:
            if (it->Value == 1)
                continue;
            break;
        case 3:
            if (it->Index == 0)
                continue;
            if (it->Index > 3 && it->Index <= 5)
                continue;
            break;
        }
        long type = it->Type;
        CWidgetTypeMap& widgetTypes = source.Data->WidgetTypes;
        CMapIterator found;
        widgetTypes.find(&found, &type);
        if (found._M_node == widgetTypes._M_header)
            continue;
        CComponent* component = CFrontEndManager::GetInstance()->CreateComponent(source.Data->ComponentType);
        if (component == 0)
            continue;
        component->Init();
        component->Setup(*it, &found._M_node->Value);
        pending.push_back(component);
    }
    for (CMapNode* node = source.Data->ThingMapHeader->_M_left; node != source.Data->ThingMapHeader; node = NextNode(node)) {
        CComponent* component;
        if (node->Key == 0x3c || node->Key == 0x72) {
            if ((component = GetScriptThingComponent((CThing*)node->Value, &cursor, 10)) != 0)
                component->ScriptTypes.push_back(node->Key);
            if ((component = GetScriptThingComponent((CThing*)node->Value, &cursor, 11)) != 0)
                component->ScriptTypes.push_back(node->Key);
            if ((component = GetScriptThingComponent((CThing*)node->Value, &cursor, 12)) != 0)
                component->ScriptTypes.push_back(node->Key);
            if ((component = GetScriptThingComponent((CThing*)node->Value, &cursor, 13)) != 0)
                component->ScriptTypes.push_back(node->Key);
        }
        else {
            long type = 0;
            switch (node->Key) {
            case 0x25: case 0x42: type = 10; break;
            case 0x26: case 0x43: type = 11; break;
            case 0x27: case 0x44: type = 12; break;
            case 0x28: case 0x45: type = 13; break;
            }
            if ((component = GetScriptThingComponent((CThing*)node->Value, &cursor, type)) != 0)
                component->ScriptTypes.push_back(node->Key);
        }
    }
    for (long* order = source.Data->OrderStart; order != source.Data->OrderFinish; ++order) {
        for (CListNode<CComponent*>* node = pending._M_node->_M_next; node != pending._M_node; ) {
            CComponent* component = node->_M_data;
            node = node->_M_next;
            if (*order == component->ScriptTypes.front()) {
                CCountedPointer<CComponent> added(component);
                AddComponent(added);
                added->SetFont(Font, 0.2f);
                added->SetColour(CRGBColour(0, 0, 0, 0), 0, 0);
                pending.remove(component);
            }
        }
    }
    while (!pending.empty()) {
        CComponent* component = pending.front();
        CCountedPointer<CComponent> added(component);
        AddComponent(added);
        added->SetFont(Font, 0.2f);
        added->SetColour(CRGBColour(0, 0, 0, 0), 0, 0);
        pending.remove(component);
    }
}
