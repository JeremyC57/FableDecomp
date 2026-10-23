// Grade: DIFFER(611v611) -- 6 difflines: the BoundingRadius product loads the mesh radius first (`mov eax,[eax] ; fld [eax+0x80] ; mov ecx,[esi+0x64] ; fmul [ecx+4]`); retail loads PPhysicsMeshInfo first and fld RenderSizeX. Source operand order has no effect; an inline getter fixed the same pattern for the circle radius but not here (tried field, getter, sphere-struct ref/getter, named block-local mesh).
// CTCPhysicsRigidBody::SetPhysicsMeshBankIndex(long index) -- (re)fill the physics mesh info from the bank's physics mesh: the render scale, the bounding-box centre rotated into the body's orientation and scaled, the bounding sphere and circle radii, then the XY offset from the mesh info's centre and the radius; finally re-place the thing in its mapwho if it is in one
// Leaf: SetPhysicsMeshBankIndex
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075A530, 610 bytes, `ret 4`.
// Callees: operator new 0x00BFEA1A, CTCBase::GetWorld 0x00686A80, the appearance scale getter 0x004BBFF0, the mesh bank's
//   GetPhysicsMeshInfo 0x00A24250, CPhysicsMeshInfo::GetMesh 0x00A262B0 (CIVCountedPointer<CPhysicsMesh const> by value,
//   x3), the orientation matrix builder 0x00A358F0 (__fastcall), virtual slot 93 GetPhysicsMeshInfo (+0x174),
//   CTCPhysicsBase::CalculateRadius 0x006AFE20, GetMapwhoPosition 0x006AFCB0, CVectorMap LowerBound 0x0040F020,
//   CTCMapwho::OnDie 0x0071A730 / PlaceInMapwho 0x0071A710. 0.5f 0x0122F59C, 0.01f 0x01230A5C.
// Layout: as the landed ctor 0x0075AA80 / OnCreate 0x0075B2D0: PPhysicsMeshInfo +0x64, RHSet +0x68, OffsetVector +0x98
//   (C2DVector); retail CPhysicsMesh = PDB - 0x1c (bounding sphere radius +0x80, bounding box +0x84, circle radius +0xa4).
//   Same shape as the sibling CTCPhysicsStandard::SetPhysicsMeshBankIndex 0x00724630.
extern void operator delete(void* p);
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
    float GetBoundingSphereRadius() const { return BoundingSphereRadius; }
    float GetBoundingCircleRadius() const { return BoundingCircleRadius; }
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

struct C2DVector { float X, Y; };
#define V4(n) virtual void v##n##a(); virtual void v##n##b(); virtual void v##n##c(); virtual void v##n##d();
struct CTCPhysicsRigidBody {
    V4(0) V4(1) V4(2) V4(3) V4(4) V4(5) V4(6) V4(7) V4(8) V4(9) V4(10) V4(11) V4(12) V4(13) V4(14) V4(15)
    V4(16) V4(17) V4(18) V4(19) V4(20) V4(21) V4(22)              // 0-91
    virtual void v92();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
    virtual void SetPhysicsMeshBankIndex(long index);
    CThing*         ParentThing;                    // +0x04
    unsigned char   _pad08[0x34 - 0x08];
    float           Radius;                         // +0x34
    unsigned char   _pad38[0x64 - 0x38];
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;        // +0x64
    CRightHandedSet RHSet;                          // +0x68
    CRightHandedSet OldRHSet;                       // +0x80
    C2DVector       OffsetVector;                   // +0x98
    const CWorld& GetWorld() const;                 // 0x00686A80 (CTCBase)
    float CalculateRadius();                        // 0x006AFE20
    C3DVector GetMapwhoPosition() const;            // 0x006AFCB0
};

void CTCPhysicsRigidBody::SetPhysicsMeshBankIndex(long index)
{
    if (!PPhysicsMeshInfo)
        PPhysicsMeshInfo = new CThingPhysicsMeshInfo;
    PPhysicsMeshInfo->BankIndex = (unsigned short)index;
    PPhysicsMeshInfo->RenderSizeX = ParentThing->PGraphicAppearance->GetScale();
    CMeshBank* bank = GetWorld().PMeshBank;
    CPhysicsMeshInfo* meshInfo = bank->GetPhysicsMeshInfo(index);
    C3DVector centre = meshInfo->GetMesh()->GetBoundingBoxCentre();
    C3DMatrix matrix;
    GetMatrixFromRHSet(matrix, RHSet).Transform(centre);
    PPhysicsMeshInfo->ToCentre = centre *= PPhysicsMeshInfo->RenderSizeX * 0.01f;
    PPhysicsMeshInfo->BoundingRadius = PPhysicsMeshInfo->RenderSizeX * meshInfo->GetMesh()->GetBoundingSphereRadius() * 0.01f;
    PPhysicsMeshInfo->BoundingCircleRadius = PPhysicsMeshInfo->RenderSizeX * meshInfo->GetMesh()->GetBoundingCircleRadius() * 0.01f;
    const C3DVector& toCentre = GetPhysicsMeshInfo()->ToCentre;
    OffsetVector.X = toCentre.X;
    OffsetVector.Y = toCentre.Y;
    Radius = CalculateRadius();
    CTCMapwho* mapwho;
    ParentThing->GetInterface(&mapwho, 0x3a);
    if (mapwho->InMapwho) {
        mapwho->OnDie();
        mapwho->PlaceInMapwho(GetMapwhoPosition());
    }
}
