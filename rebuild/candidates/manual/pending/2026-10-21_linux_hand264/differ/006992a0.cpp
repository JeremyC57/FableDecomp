// CGameCameraManager::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const -- camera debug drawing: in modes 1/3 let the current camera mode draw itself; in modes 2/3 mark every camera-point thing with a small sphere (and let its camera-point component draw), then draw each debug line from the camera position
// Grade: DIFFER(485v506) -- first draft: retail frame 0x64 vs 0x4c (the primitive record carries more stack temporaries: colour bytes and the end point pass through an 8-byte temp), register roles differ (esi this, edi info, ebp list node)
// Leaf: PreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) @ 0x006992A0, 506 bytes, `ret 4`.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes), the thing manager's GetAllThings 0x004ACD70 (TC 0x3e),
//   CThing::GetPosition 0x004C73D0, the debug-sphere helper 0x008764E0 (fastcall), the component-map LowerBound 0x0040F020
//   (TC 0x44); virtual slots: camera mode 5, component 11, renderer 20.
// Layout: local projection -- World +0x74 (thing manager +0x20), current mode +0x98 (position +0x54), debug mode +0x154,
//   debug-line list +0x15c; the prepare info keeps its renderer at +0x34.
#include <stdlib.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRGBColour { unsigned char B, G, R, A; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
template <class T> struct list {
    _List_node_base* _M_node;
    list()
    {
        _M_node = (_List_node_base*)malloc(sizeof(_List_node<T>));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        _List_node_base* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
};
struct CDebugLine { C3DVector End; CRGBColour Colour; };
struct CPrimitiveLine {
    long       Type;
    long       Flags;
    bool       A, B, C;
    C3DVector  Start;
    C3DVector  End;
    CRGBColour Colour;
    float      Width;
    float      Extra[3];
    bool       Visible;
    CPrimitiveLine(const C3DVector& start, const C3DVector& end, CRGBColour colour)
        : Type(6), Flags(0), A(false), B(false), C(false), Start(start), End(end), Colour(colour), Width(0.05f), Visible(true)
    {
        Extra[0] = 0.0f;
        Extra[1] = 0.0f;
        Extra[2] = 0.0f;
    }
};
struct CRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19();
    virtual void AddPrimitive(const CPrimitiveLine& prim);   // slot 20
};
struct CEnginePrimitivePrepareInfo { unsigned char Data[0x34]; CRenderer* Renderer; };
void __fastcall DrawDebugSphere(const CEnginePrimitivePrepareInfo& info, const C3DVector& pos, float radius);   // 0x008764E0
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    ComponentEntry* LowerBound(const long& key);         // 0x0040F020
};
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const;   // slot 11
};
struct CThing {
    unsigned char  Base[0x20];
    unsigned long  UsingComponents[9];                   // +0x20
    ComponentMap   Components;                           // +0x44
    const C3DVector& GetPosition() const;                // 0x004C73D0
};
struct CThingManager { void GetAllThings(const long& type, list<CThing*>& out); };   // 0x004ACD70
struct CWorld { unsigned char Data[0x20]; CThingManager* ThingManager; };
struct CCameraModeBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const;   // slot 5
    unsigned char Data[0x50];
    C3DVector     Position;                              // +0x54
};
template <class T, long TYPE> __forceinline T* GetComponent(CThing* thing)
{
    T* result;
    if (thing->UsingComponents[TYPE >> 5] & (1u << (TYPE & 31))) {
        ComponentMap* map = &thing->Components;
        ComponentEntry* it = map->LowerBound(TYPE);
        ComponentEntry* end = map->Finish;
        result = (T*)((it == end || it->Type > TYPE) ? end : it)->Component;
    }
    return result;
}
struct CGameCameraManager {
    virtual void v00();
    unsigned char      Data[0x70];
    CWorld*            World;                            // +0x74
    unsigned char      Data2[0x20];
    CCameraModeBase*   CurrentMode;                      // +0x98
    unsigned char      Data3[0xb8];
    long               DebugMode;                        // +0x154
    long               Spare;
    list<CDebugLine>   DebugLines;                       // +0x15c
    void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const;
};

void CGameCameraManager::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const
{
    if (DebugMode == 0)
        return;
    if (DebugMode == 1 || DebugMode == 3)
        CurrentMode->PreparePrimitivesForRendering(info);
    if (DebugMode == 2 || DebugMode == 3) {
        list<CThing*> things;
        World->ThingManager->GetAllThings(0x3e, things);
        for (_List_node_base* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next) {
            CThing* thing = ((_List_node<CThing*>*)node)->_M_data;
            DrawDebugSphere(info, thing->GetPosition(), 0.25f);
            if (thing->UsingComponents[2] & 0x10)
                GetComponent<CTCBase, 0x44>(thing)->PreparePrimitivesForRendering(info);
        }
        C3DVector camPos = CurrentMode->Position;
        for (_List_node_base* line = DebugLines._M_node->_M_next; line != DebugLines._M_node; line = line->_M_next) {
            const CDebugLine& debugLine = ((_List_node<CDebugLine>*)line)->_M_data;
            info.Renderer->AddPrimitive(CPrimitiveLine(camPos, debugLine.End, debugLine.Colour));
        }
    }
}
