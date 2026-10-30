// CTools::GetThingsOfType<CThingBuilding>(list<const CThingBuilding*>& out, long type, const C3DVector& pos, float radius) -- collects the buildings of the given thing type within radius of pos: the integer box pos +/- radius is searched with an AND of an is-of-type filter and a within-radius filter
// Leaf: ??$GetThingsOfType@VCThingBuilding@@@CTools@NCameraModes@@QBE
// Grade: DIFFER(160v160) -- wave 2 sec3: PDB spellings applied (NCameraModes::CTools::GetThingsOfType<CThingBuilding> const, CFilter_Bind2<CThingFilter_IsOfType, CThingFilter_OverlappingSphere> via the inline GFFilter_Bind2, ctor signatures from Ego_r/FableWin); residue unchanged: the two Bind2 pointer stores scheduled before the pushes (retail after the first/second push). Tried: GFFilter_Bind2 temporary, named filter locals (worse)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00878390, length 160 bytes, `ret 10h`. Caller 0x008776BE passes
// (&list, 3, &pos, 10.0f).
// Callees: __ftol2 @ 0x00BFEA70 (x3), CThingSearchTools::GetAllThingsInRoughArea<CThingBuilding const,
// list<CThingBuilding const*>, AND filter> @ 0x008780D0 (catalog label "GetPBestThingInRoughArea<...>" is a
// propagation; its filter's operator() is 0x00877EB0, which tests the type through the first pointer
// (0x006A7100) and the squared distance against the radius through the second).
// Layout: as 0x00877D20 (CTools::CameraManager +0, CGameCameraManager::World +0x74, CWorld::PThingSearchTools +0x20).
// The filter is two pointers: to a CThingFilter_IsOfType {Type} (homed in the dead radius slot) and to a
// within-radius filter {C3DVector Pos; float Radius}. Filter class names are descriptive.
class C3DVector { public: float x, y, z; };
class C2DCoordI {
public:
    long x, y;
    C2DCoordI(long ax, long ay) : x(ax), y(ay) { }
};
class C2DBoxI {
public:
    C2DCoordI TopLeft;                             // +0x00
    C2DCoordI BottomRight;                         // +0x08
    C2DBoxI(const C2DCoordI& tl, const C2DCoordI& br) : TopLeft(tl), BottomRight(br) { }
};
class CThingBuilding;
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class list { public: void* _M_node; };
}
class CThingFilter_IsOfType {
public:
    unsigned long Type;
    CThingFilter_IsOfType(unsigned long type) : Type(type) { }      // ??0CThingFilter_IsOfType@@QAE@K@Z
};
class CThingFilter_OverlappingSphere {
public:
    C3DVector Pos;
    float     Radius;
    CThingFilter_OverlappingSphere(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) { }   // ??0..QAE@ABVC3DVector@@M@Z
};
template <class F1, class F2> class CFilter_Bind2 {
public:
    const F1* F1_;
    const F2* F2_;
    CFilter_Bind2(const F1& a, const F2& b) : F1_(&a), F2_(&b) { }
};
template <class F1, class F2> inline CFilter_Bind2<F1, F2> __fastcall GFFilter_Bind2(const F1& a, const F2& b)   // Ego_r ??$GFFilter_Bind2@..@@YI?AV?$CFilter_Bind2@..@@ABV..@0@Z
{
    return CFilter_Bind2<F1, F2>(a, b);
}
class CThingSearchTools {
public:
    template <class T, class C, class F> void GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& out) const;   // 0x008780D0
};
template <class T> class scoped_ptr {
public:
    T* ptr;
    T* operator->() const { return ptr; }
};
class CWorld {
public:
    unsigned char _pad00[0x20];
    scoped_ptr<CThingSearchTools> PThingSearchTools;   // +0x20
};
class CGameCameraManager {
public:
    unsigned char _pad00[0x74];
    CWorld* World;                                 // +0x74
};
namespace NCameraModes {
class CTools {
public:
    CGameCameraManager* CameraManager;             // +0x00
    template <class T> void GetThingsOfType(std::list<const T*>& out, long type, const C3DVector& pos, float radius) const;
};
template <class T>
void CTools::GetThingsOfType(std::list<const T*>& out, long type, const C3DVector& pos, float radius) const
{
    long x = (long)pos.x;
    long y = (long)pos.y;
    long r = (long)radius;
    C2DBoxI box(C2DCoordI(x - r, y - r), C2DCoordI(x, y));
    box.BottomRight.x += r;
    box.BottomRight.y += r;
    CameraManager->World->PThingSearchTools->GetAllThingsInRoughArea<T>(box,
        GFFilter_Bind2(CThingFilter_IsOfType(type), CThingFilter_OverlappingSphere(pos, radius)), out);
}
template void CTools::GetThingsOfType<CThingBuilding>(std::list<const CThingBuilding*>& out, long type, const C3DVector& pos, float radius) const;
}
