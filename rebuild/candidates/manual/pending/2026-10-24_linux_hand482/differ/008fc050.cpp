// NCameraModes::CPhysicsForceModelBase::UpdateScan(CPhysicsCollisionPointsObject& points, list& meshes, const C3DVector& centre, long flags, float radius) -- the mesh-list instantiation of the rolling collision scan: for each scan ring due this frame, drops the oldest point once the ring holds its quota, casts from the centre to the sphere point at (theta, frame phase) against the mesh objects (the list overload of GetPointIntersection, inlined) and appends the hit position, or the sphere point when nothing is hit
// Grade: DIFFER(935v984) -- 1 grade (the 876v876 body of the CWorldMap instantiation 0x008FC430 with the list overload of GetPointIntersection inlined); retail's frame is 0xC0 (ours 0xA8) and it keeps this in esi without a spill, the frame number in ebx and the ring list in edi, where VC7.1 here homes this at [esp+5Ch] and swaps esi/ebx/edi
// Leaf: ??$UpdateScan@V?$list@VCPhysicsMeshObject@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FC050, length 984 bytes, thiscall, `ret 14h`.
// Identity: PDB ??$UpdateScan@V?$list@VCPhysicsMeshObject@@V?$allocator@VCPhysicsMeshObject@@@std@@@std@@@CPhysicsForceModelBase@
//   NCameraModes@@IAEXAAVCPhysicsCollisionPointsObject@1@AAV?$list@VCPhysicsMeshObject@@V?$allocator@VCPhysicsMeshObject@@@std@@@std@@
//   ABVC3DVector@@JM@Z; called from the landed DoThingEvasion 0x008FAF70 and GetCollisionPoints<list<CPhysicsMeshObject> >
//   0x008FC7E0. Catalog "DoThingEvasion_BuildParticleTrail" is descriptive.
// Callees: CWorld::GetFrame 0x0049D870 (static __fastcall), __ftol2 0x00BFEA70, free 0x00BFEA14 / malloc 0x00BFEA0E (list
//   nodes), CPhysicsMeshObject::GetPointIntersectionFromTo 0x00A12450 (from the inlined GetPointIntersection, whose
//   out-of-line copy is the landed 0x008FBCB0). Constants and layout as the CWorldMap instantiation 0x008FC430 (same body).
#include <stddef.h>

#pragma intrinsic(sin, cos)
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
extern "C" void* __cdecl malloc(size_t size);                  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                         // 0x00BFEA14
inline void* operator new(size_t, void* p) { return p; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
extern const C3DVector g_XAxis;                                // 0x013893B8
extern const C3DVector g_YAxis;                                // 0x013893C4
extern const C3DVector g_ZAxis;                                // 0x013893D0
extern const float g_ScanThetaEpsilon;                         // 0x0129BA3C (0.0001f, not folded by retail)

class CWorld {
public:
    static long __fastcall GetFrame();                         // 0x0049D870
};
class CWorldMap;

class CMeshIntersectionData {
public:
    C3DVector IntersectionPosition;                            // +0x00
    C3DVector Normal;                                          // +0x0C
    float     DistanceSquared;                                 // +0x18
    CMeshIntersectionData() { }
};

class CPhysicsMeshObject {                                     // 0x40 bytes
public:
    virtual void v00();
    void* Mesh;                                                // +0x04
    float PositionMatrix[12];                                  // +0x08
    float Scale;                                               // +0x38
    float OO_Scale;                                            // +0x3C
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A12450
};

namespace std {
template <class _Tp> class allocator { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;                                  // +0x00
    _List_node_base* _M_prev;                                  // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                               // +0x08
};

template <class _Tp> struct _List_iterator {
    typedef _List_iterator<_Tp> _Self;
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _Self& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _M_node->_M_next; return *this; }
    _Self operator++(int) { _Self __tmp = *this; _M_node = _M_node->_M_next; return __tmp; }
    _Self& operator--() { _M_node = _M_node->_M_prev; return *this; }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                                   // STLport list, 4 bytes in retail
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_node<_Tp> _Node;
    typedef size_t size_type;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    size_type size() const
    {
        size_type __n = 0;
        for (_List_node_base* __p = _M_node->_M_next; __p != _M_node; __p = __p->_M_next)
            ++__n;
        return __n;
    }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        free(__n);
        return iterator(__next_node);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    void pop_front() { erase(begin()); }
};
}   // namespace std

namespace NCameraModes {

class CPhysicsCollisionPointsObject {                          // retail 0x24 bytes
public:
    std::list<C3DVector> Points;                               // +0x00
    std::list<std::pair<float, std::list<C3DVector> > > SmartPointList;   // +0x04
    std::list<std::pair<float, long> > NumTestsPerTheta;       // +0x08
    long  ScanDensity;                                         // +0x0C
    float StartTheta;                                          // +0x10
    float DeltaTheta;                                          // +0x14
    long  DebugGraphicsRed;                                    // +0x18
    long  DebugGraphicsGreen;                                  // +0x1C
    long  DebugGraphicsBlue;                                   // +0x20
};

class CPhysicsForceModelBase {
public:
    virtual void v00();
    void*                         LinkedState;                 // +0x04
    CPhysicsCollisionPointsObject Things;                      // +0x08
    CPhysicsCollisionPointsObject Themes;                      // +0x2C
    CPhysicsCollisionPointsObject Ground;                      // +0x50
    C3DVector                     LastAcceleration;            // +0x74
    C3DVector                     ScanCenter;                  // +0x80
    C3DVector                     AverageCamPos;               // +0x8C
    float                         ScanRadSquared;              // +0x98
    long                          TriggerFrame;                // +0x9C

    bool GetPointIntersection(const CWorldMap& map, long flags, const C3DVector& from, const C3DVector& to, C3DVector& out)
    {
        return false;
    }
    bool GetPointIntersection(std::list<CPhysicsMeshObject>& meshes, long flags, const C3DVector& from, const C3DVector& to,
                              C3DVector& out)
    {
        CMeshIntersectionData data;
        for (std::list<CPhysicsMeshObject>::iterator it = meshes.begin(); it != meshes.end(); ) {
            if ((it++)->GetPointIntersectionFromTo(from, to, data)) {
                out = data.IntersectionPosition;
                return true;
            }
        }
        return false;
    }
    template <class T>
    void UpdateScan(CPhysicsCollisionPointsObject& points, T& container, const C3DVector& centre, long flags, float radius);
};

template <class T>
void CPhysicsForceModelBase::UpdateScan(CPhysicsCollisionPointsObject& points, T& container, const C3DVector& centre,
                                        long flags, float radius)
{
    unsigned long frame = CWorld::GetFrame();
    long density = points.ScanDensity;
    long step = frame % density;
    TriggerFrame = frame;
    float phase = (float)step / density;
    if (TriggerFrame < frame + points.ScanDensity)
    {
        for (float theta = points.StartTheta; theta <= g_ScanThetaEpsilon + 0.5f; theta += points.DeltaTheta)
        {
            long count = (long)((1.0f - (theta - 0.25f) * (theta - 0.25f) * 14.3f) * points.ScanDensity);
            if (frame % (points.ScanDensity / count) != 0)
                continue;

            std::list<std::pair<float, std::list<C3DVector> > >::iterator it = points.SmartPointList.begin();
            bool found = false;
            while (it != points.SmartPointList.end() && !found)
            {
                float ringTheta = (*it++).first;
                if (ringTheta > theta - g_ScanThetaEpsilon && ringTheta < g_ScanThetaEpsilon + theta)
                    found = true;
            }
            --it;
            std::list<C3DVector>& ring = (*it).second;
            if (ring.size() == (unsigned long)count)
                ring.pop_front();

            C3DVector end = centre + g_YAxis * radius * cos(phase * 6.2831854820251465) * sin(theta * 6.2831854820251465)
                                   + g_XAxis * radius * sin(phase * 6.2831854820251465) * sin(theta * 6.2831854820251465)
                                   + g_ZAxis * radius * cos(theta * 6.2831854820251465);
            C3DVector point = end;
            GetPointIntersection(container, flags, centre, end, point);
            ring.push_back(point);
        }
    }
}

template void CPhysicsForceModelBase::UpdateScan<std::list<CPhysicsMeshObject> >(CPhysicsCollisionPointsObject&,
                                                                                  std::list<CPhysicsMeshObject>&,
                                                                                  const C3DVector&, long, float);

}   // namespace NCameraModes
