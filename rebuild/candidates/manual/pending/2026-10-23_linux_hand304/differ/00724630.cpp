// Grade: DIFFER(749v741) -- 20 difflines: (1) `and byte ptr [esi+0x85],0xef` (retail RMW) vs ours load/and/store hoisted above the zero stores; (2) bounding-box centre: retail loads Max.X at [eax+0x90] before `add eax,0x84`, ours Min.X first; (3) the two radius products load RenderSizeX / the mesh field in the opposite order (operand order in source has no effect). Tried bool/unsigned bitfields, union byte mask, free/member operator+, operand swaps.
// CTCPhysicsStandard::SetPhysicsMeshBankIndex(long index) -- index 0 drops the physics mesh info (radius recomputed, zero offset, no navigation layers); otherwise (re)fill the mesh info from the bank's physics mesh: the render scale, the bounding-box centre rotated into the body's orientation and scaled, the bounding sphere and circle radii, then the offset, radius and navigation-layer flag; finally re-place the thing in its mapwho if it is in one
// Leaf: SetPhysicsMeshBankIndex
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724630, 739 bytes, `ret 4`.
// Callees: operator delete 0x00BFE9BC, operator new 0x00BFEA1A, CTCBase::GetWorld 0x00686A80, the appearance scale getter
//   0x004BBFF0, the mesh bank's GetPhysicsMeshInfo 0x00A24250, CPhysicsMeshInfo::BeginLoading 0x00A25BD0 and GetMesh
//   0x00A262B0 (CIVCountedPointer<CPhysicsMesh const> by value, x4), the orientation matrix 0x00A358F0 (__fastcall,
//   hidden result), CTCPhysicsBase::CalculateRadius 0x006AFE20, GetMapwhoPosition 0x006AFCB0, CVectorMap LowerBound
//   0x0040F020, CTCMapwho::OnDie 0x0071A730 / PlaceInMapwho 0x0071A710. 0.5f 0x0122F59C, 0.01f 0x01230A5C.
// Layout: engine/CTCPhysicsStandard.h offsets (local model as the landed CTCPhysicsStandard rows): PPhysicsMeshInfo +0x40,
//   OffsetVector3D +0x44, RHSet +0x50, NavigationLayers +0x85 bit 4; retail CPhysicsMesh = PDB - 0x1c (bounding sphere
//   radius +0x80, bounding box +0x84, bounding circle radius +0xa4, navigation containment volumes first/last +0x54/+0x58).
extern void operator delete(void* p);                // 0x00BFE9BC
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    void Clear() { X = Y = Z = 0.0f; }
};
struct C3DBoundingBox {
    C3DVector Min;
    C3DVector Max;
    C3DVector GetCentre() const { return (Min + Max) * 0.5f; }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct C3DMatrix {
    float M[3][3];
    C3DVector Translation;
    void Transform(C3DVector& v) const
    {
        float x = v.X;
        float y = v.Y;
        v.X = x * M[0][0] + y * M[1][0] + v.Z * M[2][0];
        v.Y = x * M[0][1] + y * M[1][1] + v.Z * M[2][1];
        v.Z = x * M[0][2] + y * M[1][2] + v.Z * M[2][2];
    }
};
const C3DMatrix& __fastcall GetMatrixFromRHSet(C3DMatrix& out, const CRightHandedSet& set);   // 0x00A358F0

struct CPtrRange {
    void* first;
    void* last;
    bool empty() const { return first == last; }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CPhysicsMesh : CRefVirtual, CRefCounted {
    unsigned char _pad08[0x54 - 0x08];
    CPtrRange NavigationVolumes;                // +0x54 (first, last)
    unsigned char _pad5c[0x80 - 0x5c];
    float     BoundingSphereRadius;             // +0x80
    C3DBoundingBox BoundingBox;                 // +0x84
    unsigned char _pad9c[0xa4 - 0x9c];
    float     BoundingCircleRadius;             // +0xa4
    __forceinline C3DVector GetBoundingBoxCentre() const { return BoundingBox.GetCentre(); }
    bool HasNavigationLayers() const { return !NavigationVolumes.empty(); }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CIVCountedPointer {
    T* Data;
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
struct CPhysicsMeshInfo {
    void BeginLoading();                        // 0x00A25BD0
    CIVCountedPointer<CPhysicsMesh> GetMesh() const;   // 0x00A262B0
};
struct CMeshBank {
    CPhysicsMeshInfo* GetPhysicsMeshInfo(long index);  // 0x00A24250
};
struct CWorld {
    unsigned char _pad00[0x3c];
    CMeshBank* PMeshBank;                       // +0x3c
};
struct CThingPhysicsMeshInfo {                  // 0x1c bytes
    unsigned short BankIndex;                   // +0x00
    float          RenderSizeX;                 // +0x04
    C3DVector      ToCentre;                    // +0x08
    float          BoundingRadius;              // +0x14
    float          BoundingCircleRadius;        // +0x18
};
struct CTCGraphicAppearance {
    float GetScale();                           // 0x004BBFF0
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCMapwho {
    unsigned char _pad00[0x50];
    unsigned char InMapwho : 1;                 // +0x50 bit 0
    void PlaceInMapwho(const C3DVector& position);   // 0x0071A710
    void OnDie();                               // 0x0071A730
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};

struct CTCPhysicsStandard {
    virtual void v000();
    CThing*         ParentThing;                    // +0x04
    unsigned char   _pad08[0x34 - 0x08];
    float           Radius;                         // +0x34
    unsigned char   _pad38[0x40 - 0x38];
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;        // +0x40
    C3DVector       OffsetVector3D;                 // +0x44
    CRightHandedSet RHSet;                          // +0x50
    CRightHandedSet OldRHSet;                       // +0x68
    long            RHSetLastFrameSet;              // +0x80
    unsigned char   Flags84;                        // +0x84
    unsigned char   CollidesWithThingsOneWay : 1;   // +0x85
    unsigned char   CollidesWithLandscape : 1;
    unsigned char   PhysicsEnabled : 1;
    unsigned char   Immoveable : 1;
    bool            NavigationLayers : 1;
    const CWorld& GetWorld() const;                 // 0x00686A80 (CTCBase)
    float CalculateRadius();                        // 0x006AFE20
    C3DVector GetMapwhoPosition() const;            // 0x006AFCB0
    virtual void SetPhysicsMeshBankIndex(long index);
};

void CTCPhysicsStandard::SetPhysicsMeshBankIndex(long index)
{
    if (index == 0) {
        delete PPhysicsMeshInfo;
        PPhysicsMeshInfo = 0;
        Radius = CalculateRadius();
        OffsetVector3D.X = 0.0f;
        OffsetVector3D.Y = 0.0f;
        OffsetVector3D.Z = 0.0f;
        NavigationLayers = false;
    } else {
        if (!PPhysicsMeshInfo)
            PPhysicsMeshInfo = new CThingPhysicsMeshInfo;
        PPhysicsMeshInfo->BankIndex = (unsigned short)index;
        PPhysicsMeshInfo->RenderSizeX = ParentThing->PGraphicAppearance->GetScale();
        CMeshBank* bank = GetWorld().PMeshBank;
        CPhysicsMeshInfo* meshInfo = bank->GetPhysicsMeshInfo(index);
        meshInfo->BeginLoading();
        C3DVector centre = meshInfo->GetMesh()->GetBoundingBoxCentre();
        C3DMatrix matrix;
        GetMatrixFromRHSet(matrix, RHSet).Transform(centre);
        PPhysicsMeshInfo->ToCentre = centre *= PPhysicsMeshInfo->RenderSizeX * 0.01f;
        PPhysicsMeshInfo->BoundingRadius = meshInfo->GetMesh()->BoundingSphereRadius * PPhysicsMeshInfo->RenderSizeX * 0.01f;
        PPhysicsMeshInfo->BoundingCircleRadius = PPhysicsMeshInfo->RenderSizeX * meshInfo->GetMesh()->BoundingCircleRadius * 0.01f;
        OffsetVector3D = PPhysicsMeshInfo->ToCentre;
        Radius = CalculateRadius();
        NavigationLayers = meshInfo->GetMesh()->HasNavigationLayers();
    }
    CTCMapwho* mapwho;
    ParentThing->GetInterface(&mapwho, 0x3a);
    if (mapwho->InMapwho) {
        mapwho->OnDie();
        mapwho->PlaceInMapwho(GetMapwhoPosition());
    }
}
