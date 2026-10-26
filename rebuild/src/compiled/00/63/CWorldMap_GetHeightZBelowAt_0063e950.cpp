// CWorldMap::GetHeightZBelowAt(const C3DVector& pos, float radius, bool useTheme, const CThingFilterBase& filter) const -- the highest surface below pos: starts from the ground (or theme) height, gathers the things whose rough area overlaps a circle of `radius` round pos and pass `filter`, and raises the height to any physics-mesh hit (+0.007) on the 200-unit drop segment below pos that lies under pos
// Leaf: GetHeightZBelowAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E950, 463 bytes, `ret 10h`.
//   PDB ?GetHeightZBelowAt@CWorldMap@@QBEMABVC3DVector@@M_NABVCThingFilterBase@1@@Z.
// Callees: __ftol2 0x00BFEA70 (x4), C2DBoxI::ValidateOrientationAndFlip 0x00A3A150, malloc 0x00BFEA0E / free 0x00BFEA14
//   (the STLport list<CThing*> sentinel and nodes), CThingSearchTools::ForEachInRoughArea<...> 0x0063FD40 (this TU's
//   template instance; same-TU stand-in below), GetThemeSizeZAt 0x0063A390 / GetGroundSizeZAt 0x0063A300,
//   CTCPhysicsBase virtual slot 93 (+0x174), GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::
//   GetPointIntersectionFromTo 0x00A12450, ~CPhysicsMeshObject 0x00A127D0; floats 1.0f @ 0x0122DED8, 200.0f @
//   0x0125A168, 0.007f @ 0x0125A164.
// Layout: CWorldMap World +0x08 (as the PDB header); CWorld ThingSearchTools +0x20; CThing PhysicsTC +0x60; the
//   CPhysicsMeshObject / CMeshIntersectionData models of the landed CThingFindHeightZBelowAt::operator() 0x00668AB0.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long x0, long y0, long x1, long y1) : TLX(x0), TLY(y0), BRX(x1), BRY(y1) { }
    void ValidateOrientationAndFlip();                  // 0x00A3A150
};
class CMeshIntersectionData { public:
    C3DVector IntersectionPosition;             // +0x00
    C3DVector Normal;                           // +0x0c
    float     DistanceSquared;                  // +0x18
};
class CPhysicsMeshObject { public:
    void*         m_pMesh;                      // +0x04 counted mesh
    unsigned char _pad08[0x38];
    virtual ~CPhysicsMeshObject();                      // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData* out);   // 0x00A12450
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092();
    virtual void* GetPhysicsMeshParameters();   // slot 93 (+0x174)
public:
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};
class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
struct CThingFilterBase {
    virtual bool operator()(const CThing* thing) const;
};

extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E (MSVCR71)
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14 (MSVCR71)

struct SThingNode {                                      // STLport _List_node<CThing*>
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CThing*     _M_data;
};
struct CThingList {                                      // STLport list<CThing*>: one sentinel pointer
    SThingNode* _M_node;
    CThingList()
    {
        SThingNode* head = (SThingNode*)malloc(sizeof(SThingNode));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node = head;
    }
    ~CThingList()
    {
        SThingNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SThingNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
    void push_back(CThing* thing)                        // STLport insert(end(), thing)
    {
        SThingNode* pos = _M_node;
        SThingNode* node = (SThingNode*)malloc(sizeof(SThingNode));
        node->_M_data = thing;
        node->_M_next = pos;
        node->_M_prev = pos->_M_prev;
        pos->_M_prev->_M_next = node;
        pos->_M_prev = node;
    }
};

class CThingFilter_OverlappingCircle { public:
    C3DVector Center;
    float     Radius;
    CThingFilter_OverlappingCircle(const C3DVector& center, float radius) : Center(center), Radius(radius) { }
};
struct CBoundThingFilter {                               // the circle test and the caller's filter, by reference
    const CThingFilter_OverlappingCircle& Circle;
    const CThingFilterBase&               Filter;
    CBoundThingFilter(const CThingFilter_OverlappingCircle& circle, const CThingFilterBase& filter) : Circle(circle), Filter(filter) { }
};

CThing* NextThingInRoughArea(const C2DBoxI& box, CThing* previous);

class CThingSearchTools { public:
    void ForEachInRoughArea(const C2DBoxI& box, const CBoundThingFilter& filter, CThingList& things);   // 0x0063FD40
};
// Same-TU stand-in for the 0x0063FD40 template instance (retail emits it in this TU after the caller): with its body in
// view VC7.1 knows the walk only appends nodes and keeps the list sentinel in a register across the call.
#pragma auto_inline(off)
void CThingSearchTools::ForEachInRoughArea(const C2DBoxI& box, const CBoundThingFilter& filter, CThingList& things)
{
    for (CThing* thing = NextThingInRoughArea(box, 0); thing; thing = NextThingInRoughArea(box, thing))
        if (filter.Filter(thing))
            things.push_back(thing);
}
#pragma auto_inline(on)

class CWorld { public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;         // +0x20
};

class CWorldMap {
public:
    void*   __vftable;
    long    f4;                                  // +0x04
    CWorld* World;                               // +0x08
    float GetGroundSizeZAt(const C3DVector& pos) const;      // 0x0063A300
    float GetThemeSizeZAt(const C3DVector& pos) const;       // 0x0063A390
    float GetHeightZBelowAt(const C3DVector& pos, float radius, bool useTheme, const CThingFilterBase& filter) const;
};

float CWorldMap::GetHeightZBelowAt(const C3DVector& pos, float radius, bool useTheme, const CThingFilterBase& filter) const
{
    float size = radius + radius;
    C2DBoxI box((long)(pos.X - size - 1.0f), (long)(pos.Y - size - 1.0f), (long)(size + pos.X + 1.0f), (long)(size + pos.Y + 1.0f));
    box.ValidateOrientationAndFlip();
    CThingList things;
    CThingFilter_OverlappingCircle circle(pos, radius);
    World->ThingSearchTools->ForEachInRoughArea(box, CBoundThingFilter(circle, filter), things);
    float height = useTheme ? GetThemeSizeZAt(pos) : GetGroundSizeZAt(pos);
    C3DVector from = pos;
    C3DVector to(from.X, from.Y, from.Z - 200.0f);
    for (SThingNode* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next) {
        CTCPhysicsBase* physics = node->_M_data->PhysicsTC;
        if (physics && physics->GetPhysicsMeshParameters()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            CMeshIntersectionData data;
            if (mesh.GetPointIntersectionFromTo(from, to, &data)) {
                float z = data.IntersectionPosition.Z + 0.007f;
                if (z > height && z < from.Z)
                    height = z;
            }
        }
    }
    return height;
}