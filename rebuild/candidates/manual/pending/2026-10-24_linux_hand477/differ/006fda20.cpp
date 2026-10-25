// Grade: DIFFER(841v819) -- LTCG lane gap: retail keeps the local list<CThingGameObject*> sentinel in ebx straight after malloc (no `_M_node = 0` pre-store) and reuses it after GetAllThingsInRoughArea 0x006FE7C0 without reloading the list object, then recycles the argument slot for the GetPosition reference: only LTCG mod-ref knowledge that the same-TU template never rewrites `things._M_node` allows that. The lane keeps the list in memory (zero pre-store, reloads, ebp spilled inside the loop, frame 0xb8 v 0xa4). Probe c, which passes `things.end()` by const reference so the list never escapes, reaches 819v819 and leaves only stack-slot order and the camera-position copy base (`lea eax,[esi+4]`), which confirms the gap. Tried: 4 grades (CThing* list 841v819, + 4-byte polymorphic base and STLport 4.6 insert 841v819, end-iterator 819v819, the PDB-faithful form 841v819, this file).
// CGameCameraBase::IsThingCollision() -- gather the things near where the camera points that the camera-to-target line passes through: their positions go to LocalThingPosList, the hit points to DrawCoordList (the building being entered first), true when any mesh was hit
// Leaf: ?IsThingCollision@CGameCameraBase@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FDA20, 819 bytes, __thiscall, plain `ret`.
// PDB: Ego_r.pdb ?IsThingCollision@CGameCameraBase@@AAE_NXZ (private, bool()). Catalog "CGameCameraBase_TracePhysics" is descriptive.
// Callees: malloc 0x00BFEA0E (sentinel, nodes), __ftol2 0x00BFEA70 (x2), CThingSearchTools::GetAllThingsInRoughArea
//   <CThingGameObject, list<CThingGameObject*>, CFilter_Bind2<CThingFilter_MinAllowedRadius, CThingFilter_OverlappingSphere> >
//   0x006FE7C0 (Ego_r.pdb names the instantiation), free 0x00BFEA14, CThing::GetPosition 0x004C73D0, physics slot 93 (+0x174),
//   CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (by value), CPhysicsMeshObject::GetPointIntersectionFromTo 0x00A12450
//   (writes a CMeshIntersectionData), ~CPhysicsMeshObject 0x00A127D0.
// Layout: retail CGameCameraBase has 4-byte STLport lists: DrawCoordList +0xA8, LocalThingPosList +0xAC, BuildingUID +0xB8
//   (the engine header follows the 12-byte debug lists, so a local retail model is used); CThing UID +0x18, physics +0x60.
// Shape: the box is (long)PointingAt +-5 via __ftol2; the filter binds a 0.5 min radius and a 5.0 sphere round PointingAt;
//   both member lists are cleared inline (member reloaded per node); per thing the position is appended, and when the
//   physics has a mesh GoingIntoBuilding = BuildingUID > 0 (`ja`/`jbe`), the camera-to-target hit gets a 0/5 lift and is
//   pushed to the front when the thing is the building (UID compare), else to the back; the result byte is returned.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;                       // Ego_r.pdb
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node<_Tp>* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) {}
    _Tp& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = (_List_node<_Tp>*)_M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(), 0)
    {
        _M_node._M_data = _M_node.allocate(1);
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear()
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp> iterator;
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) {}
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    _List_node<_Tp>* _M_create_node(const _Tp& __x)
    {
        _List_node<_Tp>* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _List_node<_Tp>* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

class CMeshIntersectionData {                       // Ego_r.pdb, 0x1c
public:
    C3DVector IntersectionPosition;                 // +0x00
    C3DVector Normal;                               // +0x0C
    float DistanceSquared;                          // +0x18
};
class CPhysicsMesh;
class CPhysicsMeshObject {                          // retail 0x40 bytes (vptr, CIVCountedPointer Mesh, CMatrix3x4, Scale, OO_Scale)
public:
    void* __vftable;
    CPhysicsMesh* Mesh;
    float PositionMatrix[12];
    float Scale;
    float OO_Scale;
    ~CPhysicsMeshObject();                          // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& intersection) const;   // 0x00A12450
};

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual const void* GetPhysicsMeshInfo() const; // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};

class CThing {
public:
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                           // +0x18
    unsigned char _pad20[0x60 - 0x20];
    CTCPhysicsBase* PPhysics;                       // +0x60
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

class CThingFilter_MinAllowedRadius {               // Ego_r.pdb; operator() 0x006A7410: the thing's physics radius (+0x34) above MinRadius
public:
    float MinRadius;
    CThingFilter_MinAllowedRadius(float minRadius) : MinRadius(minRadius) {}
};
class CThingFilter_OverlappingSphere {              // Ego_r.pdb
public:
    C3DVector SpherePos;
    float Radius;
    CThingFilter_OverlappingSphere(const C3DVector& spherePos, float radius) : SpherePos(spherePos), Radius(radius) {}
};
template <class TFilter1, class TFilter2> class CFilter_Bind2 {   // Ego_r.pdb: F1 +0, F2 +4
public:
    const TFilter1* F1;
    const TFilter2* F2;
    CFilter_Bind2(const TFilter1& f1, const TFilter2& f2) : F1(&f1), F2(&f2) {}
};
class CThingGameObject;

class CThingSearchTools {
public:
    template <class TThing, class TList, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TList& things) const;   // 0x006FE7C0 (<CThingGameObject, list<CThingGameObject*>, CFilter_Bind2<MinAllowedRadius, OverlappingSphere> >)
};
class CWorld {
public:
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;           // +0x20
};

class CCamera {                                     // retail 0x38
public:
    C3DVector Position;                             // +0x00
    unsigned char _pad0C[0x38 - 0x0c];
};

class CCameraInterface {                            // the 4-byte polymorphic base at +0 (Ego_r.pdb `_padding_`)
public:
    virtual void v00();
};
class CGameCameraBase : public CCameraInterface {
public:
    CCamera InternalCam;                            // +0x04
    CCamera LastInternalCam;                        // +0x3C
    C3DVector LastPointingAt;                       // +0x74
    C3DVector PointingAt;                           // +0x80
    CWorld* World;                                  // +0x8C
    float Theta;                                    // +0x90
    float Phi;                                      // +0x94
    float Rad;                                      // +0x98
    float ThetaFixed;                               // +0x9C
    float RadFixed;                                 // +0xA0
    bool ActAsFixedCam;                             // +0xA4
    bool GoingIntoBuilding;                         // +0xA5
    std::list<C3DVector> DrawCoordList;             // +0xA8
    std::list<C3DVector> LocalThingPosList;         // +0xAC
    long TriggerFrame;                              // +0xB0
    unsigned __int64 BuildingUID;                   // +0xB8
    float PhiChanger;                               // +0xC0
    bool IsThingCollision();
};

bool CGameCameraBase::IsThingCollision()
{
    bool collision = false;
    std::list<CThingGameObject*> things;
    C3DVector cameraPos = InternalCam.Position;
    long x = (long)PointingAt.X;
    long y = (long)PointingAt.Y;
    C2DBoxI area(x - 5, y - 5, x + 5, y + 5);
    CThingFilter_OverlappingSphere sphereFilter(PointingAt, 5.0f);
    CThingFilter_MinAllowedRadius radiusFilter(0.5f);
    World->PThingSearchTools->GetAllThingsInRoughArea<CThingGameObject>(area, CFilter_Bind2<CThingFilter_MinAllowedRadius, CThingFilter_OverlappingSphere>(radiusFilter, sphereFilter), things);
    DrawCoordList.clear();
    LocalThingPosList.clear();
    for (std::list<CThingGameObject*>::iterator it = things.begin(); it != things.end(); ++it) {
        CThing* thing = (CThing*)*it;
        CTCPhysicsBase* physics = thing->PPhysics;
        LocalThingPosList.push_back(thing->GetPosition());
        if (physics->GetPhysicsMeshInfo()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            GoingIntoBuilding = BuildingUID > 0;
            CMeshIntersectionData intersection;
            if (mesh.GetPointIntersectionFromTo(PointingAt, cameraPos, intersection)) {
                float lift = 0.0f;
                if (BuildingUID > 0 && !GoingIntoBuilding)
                    lift = 5.0f;
                if (BuildingUID == thing->UID)
                    DrawCoordList.push_front(C3DVector(intersection.IntersectionPosition.X, intersection.IntersectionPosition.Y, intersection.IntersectionPosition.Z + lift));
                else
                    DrawCoordList.push_back(C3DVector(intersection.IntersectionPosition.X, intersection.IntersectionPosition.Y, intersection.IntersectionPosition.Z + lift));
                collision = true;
            }
        }
    }
    return collision;
}
