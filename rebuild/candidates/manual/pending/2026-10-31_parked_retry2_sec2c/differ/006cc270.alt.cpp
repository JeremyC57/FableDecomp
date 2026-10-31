// CTCCreatureNavigation::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- debug-draw the creature's route while it is following one: a green ground line per route segment with a blue 0.4 marker at each waypoint, a red line from the creature to the first waypoint (with its marker), and green lines for the current path's point pairs
// Alt (retry wave 2 sec2c): DIFFER(989v1085)/245 -- the marker lines reuse the end vector (no top temporaries): frame now 0xc4 as retail, but the body shrinks (retail keeps a separate top copy somewhere else)
// Grade: DIFFER(1035v1085) -- first draft: frame 0xd0 v 0xc4 (route-node copy and line slots), the C3DVector(x, y, 1000) build order (retail stores X/Y words then the 1000 constant after the pushes); flow, colours and callees as retail
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CC270 (vtable slot 11), 1088 bytes, `ret 4`, no EH frame.
// Callees: CThingCreatureBase::GetHeightZBelowAt 0x00666D40, CEnginePrimitiveLine ctor 0x004C3090 (inline at the last site),
// CIEngine::AddPrimitive (slot 20, +0x50), CThing::GetPosition 0x004C73D0, the navigation state's GetPathPoints (slot 17, +0x44),
// free 0x00BFEA14. Globals: g_bShowNavigationDebug 0x013BAB20, NAV_MODE_FOLLOW_ROUTE (const 3) 0x0129CBA4, debug height
// offset 0x0137FE44, 0.4f 0x0125A318.
// Layout: NavState +0x0c (mode byte +0x10, route list<SRoutePoint> head +0x8c: point X +8, Y +0xc).
extern "C" void __cdecl free(void* p);                       // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
};
struct CEnginePrimitive { };
struct CEnginePrimitiveLine : CEnginePrimitive {
    long          m_type;                                    // +0x00
    long          m_interpolation;                           // +0x04
    bool          m_flag8, m_flag9, m_flagA;                 // +0x08
    C3DVector     m_start;                                   // +0x0C
    C3DVector     m_end;                                     // +0x18
    CRGBColour    m_colour;                                  // +0x24
    float         m_width;                                   // +0x28
    unsigned long m_flags;                                   // +0x2C
    long          m_blendMode;                               // +0x30
    long          m_blendOp;                                 // +0x34
    bool          m_depthTest;                               // +0x38
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour,
                         float width, long blendMode, unsigned long flags, bool depthTest, long interpolation,
                         long blendOp);                      // 0x004C3090
};
struct CIEngine {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19)
#undef VF
    virtual void AddPrimitive(const CEnginePrimitive& primitive);   // slot 20 (+0x50)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine* Engine;                                        // +0x34
};
extern bool g_bShowNavigationDebug;                          // 0x013BAB20
extern const unsigned long NAV_MODE_FOLLOW_ROUTE;            // 0x0129CBA4
extern float g_NavigationDebugHeight;                        // 0x0137FE44
struct CThingCreatureBase {
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    float GetHeightZBelowAt(const C3DVector& pos, bool a, bool b);   // 0x00666D40
};
struct SRouteNode {
    SRouteNode* Next;
    SRouteNode* Prev;
    float X, Y;                                              // +0x08
};
struct CPointVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPointVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPointVector() { if (_M_start) free(_M_start); }
};
struct CNavigationState {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
#undef VF
    virtual void GetPathPoints(CPointVector* points);        // slot 17 (+0x44)
    unsigned char _pad04[0x10 - 0x04];
    unsigned char Mode;                                      // +0x10
    unsigned char _pad11[0x8c - 0x11];
    SRouteNode* Route;                                       // +0x8c list head
    unsigned long RouteSize() const
    {
        unsigned long n = 0;
        for (SRouteNode* p = Route->Next; p != Route; p = p->Next)
            ++n;
        return n;
    }
};
struct CTCCreatureNavigation {
    virtual void v00();
    CThingCreatureBase* ParentThing;                         // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CNavigationState* NavState;                              // +0x0c
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCCreatureNavigation::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (!g_bShowNavigationDebug)
        return;
    CNavigationState* state = NavState;
    if (state->Mode != NAV_MODE_FOLLOW_ROUTE)
        return;
    if (state->RouteSize() == 0)
        return;
    for (SRouteNode* it = state->Route->Next; it != state->Route; it = it->Next) {
        SRouteNode* next = it->Next;
        if (next == state->Route)
            break;
        C3DVector from(it->X, it->Y, 1000.0f);
        from.Z = ParentThing->GetHeightZBelowAt(from, false, true) + g_NavigationDebugHeight;
        C3DVector to(next->X, next->Y, 1000.0f);
        to.Z = ParentThing->GetHeightZBelowAt(to, false, true) + g_NavigationDebugHeight;
        CEnginePrimitiveLine line(0, from, to, CRGBColour(0, 255, 0, 255), 0.05f, 0, 0, true, 0, 0);
        info.Engine->AddPrimitive(line);
        line.m_start = to;
        to.Z += 0.4f;
        line.m_end = to;
        line.m_colour = CRGBColour(0, 0, 255, 255);
        info.Engine->AddPrimitive(line);
    }
    C3DVector here(ParentThing->GetPosition().X, ParentThing->GetPosition().Y, 1000.0f);
    here.Z = ParentThing->GetHeightZBelowAt(here, false, true) + g_NavigationDebugHeight;
    C3DVector first(state->Route->Next->X, state->Route->Next->Y, 1000.0f);
    first.Z = ParentThing->GetHeightZBelowAt(first, false, true) + g_NavigationDebugHeight;
    CEnginePrimitiveLine line(0, here, first, CRGBColour(255, 0, 0, 255), 0.05f, 0, 0, true, 0, 0);
    info.Engine->AddPrimitive(line);
    line.m_start = first;
    first.Z += g_NavigationDebugHeight;
    line.m_end = first;
    line.m_colour = CRGBColour(255, 0, 0, 255);
    info.Engine->AddPrimitive(line);
    CPointVector points;
    state->GetPathPoints(&points);
    for (C3DVector* p = points._M_start; p != points._M_finish; ) {
        C3DVector a(p->X, p->Y, 1000.0f);
        a.Z = ParentThing->GetHeightZBelowAt(a, false, true) + g_NavigationDebugHeight;
        ++p;
        C3DVector b(p->X, p->Y, 1000.0f);
        b.Z = ParentThing->GetHeightZBelowAt(b, false, true) + g_NavigationDebugHeight;
        info.Engine->AddPrimitive(CEnginePrimitiveLine(0, a, b, CRGBColour(0, 255, 0, 255), 0.05f, 0, 0, true, 0, 0));
        ++p;
    }
}
