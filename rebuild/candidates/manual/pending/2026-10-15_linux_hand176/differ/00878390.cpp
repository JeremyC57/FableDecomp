// CTools::GetThingsOfType<CThingBuilding>(list<const CThingBuilding*>& out, long type, const C3DVector& pos, float radius) -- collects the buildings of the given thing type within radius of pos: the integer box pos +/- radius is searched with an AND of an is-of-type filter and a within-radius filter
// Leaf: GetThingsOfType
// Grade: DIFFER(160v160) -- scheduling only: retail stores the AND filter's two pointers ([esp+14h], [esp+1Ch]) after the first/second push, ours before the pushes (tried: four-long box ctor, box temporary in the call, two-vector box ctor, top-left box + `+= r` bottom-right (this form, the closest))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00878390, length 160 bytes, `ret 10h`. Caller 0x008776BE passes
// (&list, 3, &pos, 10.0f).
// Callees: __ftol2 @ 0x00BFEA70 (x3), CThingSearchTools::GetAllThingsInRoughArea<CThingBuilding const,
// list<CThingBuilding const*>, AND filter> @ 0x008780D0 (catalog label "GetPBestThingInRoughArea<...>" is a
// propagation; its filter's operator() is 0x00877EB0, which tests the type through the first pointer
// (0x006A7100) and the squared distance against the radius through the second).
// Layout: as 0x00877D20 (CTools::CameraManager +0, CGameCameraManager::World +0x74, CWorld::PThingSearchTools +0x20).
// The filter is two pointers: to a CThingFilter_IsOfType {Type} (homed in the dead radius slot) and to a
// within-radius filter {C3DVector Pos; float Radius}. Filter class names are descriptive.
struct C3DVector { float x, y, z; };
struct C2DVectorI {
    long x, y;
    C2DVectorI(long ax, long ay) : x(ax), y(ay) { }
};
struct C2DBoxI {
    C2DVectorI TopLeft;                            // +0x00
    C2DVectorI BottomRight;                        // +0x08
    C2DBoxI(const C2DVectorI& tl, const C2DVectorI& br) : TopLeft(tl), BottomRight(br) { }
};
struct CThing;
struct CThingGameObject;
struct CThingFilter_None { };
template <class T> struct CIntelligentPointer { void* m_vtable; T* m_pItem; };
struct CThingSearchTools {
    template <class T, class C, class F> void GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& out);   // 0x008780D0 (<CThingBuilding const, list, AND filter>)
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* get() const { return ptr; }
    T* operator->() const { return ptr; }
};
struct CWorld {
    unsigned char _pad00[0x20];
    scoped_ptr<CThingSearchTools> PThingSearchTools;   // +0x20
};
struct CCamera {
    C3DVector WorldPosition;                       // +0x00
    unsigned char _rest[0x38 - 12];
};
struct CGameCameraManager {
    void*   m_vtable;                              // +0x00
    CCamera Camera;                                // +0x04
    CCamera OldCamera;                             // +0x3c
    CWorld* World;                                 // +0x74
};
struct CThingBuilding;
template <class T> struct list {                   // STLport list<T>: the sentinel node pointer
    void* _M_node;
};
struct CThingFilter_IsOfType {
    long Type;
    CThingFilter_IsOfType(long type) : Type(type) { }
};
struct CThingFilter_WithinRadius {
    C3DVector Pos;
    float     Radius;
    CThingFilter_WithinRadius(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) { }
};
template <class A, class B> struct CThingFilter_And {
    const A* PA;
    const B* PB;
    CThingFilter_And(const A& a, const B& b) : PA(&a), PB(&b) { }
};
struct CTools {
    CGameCameraManager* CameraManager;             // +0x00
    template <class T> void GetThingsOfType(list<const T*>& out, long type, const C3DVector& pos, float radius);
};
template <class T>
void CTools::GetThingsOfType(list<const T*>& out, long type, const C3DVector& pos, float radius)
{
    long x = (long)pos.x;
    long y = (long)pos.y;
    long r = (long)radius;
    C2DBoxI box(C2DVectorI(x - r, y - r), C2DVectorI(x, y));
    box.BottomRight.x += r;
    box.BottomRight.y += r;
    CameraManager->World->PThingSearchTools->GetAllThingsInRoughArea<const T>(box,
        CThingFilter_And<CThingFilter_IsOfType, CThingFilter_WithinRadius>(CThingFilter_IsOfType(type), CThingFilter_WithinRadius(pos, radius)), out);
}
template void CTools::GetThingsOfType<CThingBuilding>(list<const CThingBuilding*>& out, long type, const C3DVector& pos, float radius);
