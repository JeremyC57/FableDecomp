// CTCPhysicsRigidBody::SetPhysicsMeshBankIndex(long index) -- (re)fill the physics mesh info from the bank's physics mesh: the render scale, the bounding-box centre rotated into the body's orientation and scaled, the bounding sphere and circle radii, then the XY offset from the mesh info's centre and the radius; finally re-place the thing in its mapwho if it is in one
// Grade: RELOCATION_MATCH under /EHsc (retry wave 2 sec2: the lander's flag sweep; at plain /O2 the BoundingRadius product loaded the mesh radius first, 611v611). Respelled: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (bound 0x0040F020), the PDB class keys (class, not struct), CTCBase::PeekWorld / CTCPhysicsBase::CalculateRadius / GetMapwhoPosition on their own classes, the protected virtual leaf (Ego_r MAE), CMeshDataBank::PeekPhysicsMeshInfo and NGraphicAppearance::CTCGraphicAppearance::GetScale (both PDBs; the bound GetWorld / CMeshBank::GetPhysicsMeshInfo / CTCGraphicAppearance::GetScale spellings were wrong, NAME_FIXES.tsv), CIVCountedPointer<const CPhysicsMesh> (Ego_r GetMesh). GetMatrixFromRHSet 0x00A358F0 keeps its descriptive name (no PDB entry).
// Leaf: SetPhysicsMeshBankIndex
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075A530, 610 bytes, `ret 4`.
// Callees: operator new 0x00BFEA1A, CTCBase::PeekWorld 0x00686A80, NGraphicAppearance::CTCGraphicAppearance::GetScale 0x004BBFF0,
//   CMeshDataBank::PeekPhysicsMeshInfo 0x00A24250, CPhysicsMeshInfo::GetMesh 0x00A262B0 (CIVCountedPointer<CPhysicsMesh const> by value,
//   x3), the orientation matrix builder 0x00A358F0 (__fastcall), virtual slot 93 GetPhysicsMeshInfo (+0x174),
//   CTCPhysicsBase::CalculateRadius 0x006AFE20, GetMapwhoPosition 0x006AFCB0, CVectorMap LowerBound 0x0040F020,
//   CTCMapwho::OnDie 0x0071A730 / PlaceInMapwho 0x0071A710. 0.5f 0x0122F59C, 0.01f 0x01230A5C.
// Layout: as the landed ctor 0x0075AA80 / OnCreate 0x0075B2D0: PPhysicsMeshInfo +0x64, RHSet +0x68, OffsetVector +0x98
//   (C2DVector); retail CPhysicsMesh = PDB - 0x1c (bounding sphere radius +0x80, bounding box +0x84, circle radius +0xa4).
//   Same shape as the sibling CTCPhysicsStandard::SetPhysicsMeshBankIndex 0x00724630.
extern void operator delete(void* p);
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    void Clear() { X = Y = Z = 0.0f; }
};
class C3DBoundingBox { public:
    C3DVector Min;
    C3DVector Max;
    C3DVector GetCentre() const { return (Min + Max) * 0.5f; }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class C3DMatrix { public:
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
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy() const;               // slot 1
};
class CPhysicsMesh : public CRefVirtual, public CRefCounted { public:
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
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CIVCountedPointer { public:
    T* Data;
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
class CPhysicsMeshInfo { public:
    void BeginLoading();                        // 0x00A25BD0
    CIVCountedPointer<const CPhysicsMesh> GetMesh() const;   // 0x00A262B0
};
class CMeshDataBank { public:
    const CPhysicsMeshInfo* PeekPhysicsMeshInfo(unsigned long index) const;   // 0x00A24250 (Ego_r + FableWin ?PeekPhysicsMeshInfo@CMeshDataBank@@QBEPBVCPhysicsMeshInfo@@K@Z)
};
class CWorld { public:
    unsigned char _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;               // +0x3c CCountedPointer<CMeshDataBank>::Data (Ego_r)
};
class CThingPhysicsMeshInfo { public:                  // 0x1c bytes
    unsigned short BankIndex;                   // +0x00
    float          RenderSizeX;                 // +0x04
    C3DVector      ToCentre;                    // +0x08
    float          BoundingRadius;              // +0x14
    float          BoundingCircleRadius;        // +0x18
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    float GetScale() const;                     // 0x004BBFF0 (Ego_r + FableWin ?GetScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ)
};
}
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {                                  // retail STLport vector<pair<K, V> > + comparator (only the used members)
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;                           // +0x00
    value_type* _M_finish;                          // +0x04
    value_type* LowerBound(const K& key);     // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;
class CTCMapwho { public:
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
    NGraphicAppearance::CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->_M_finish || interfaceType < it->first) ? map->_M_finish : it)->second;
        }
    }
};

class C2DVector { public: float X, Y; };
#define V4(n) virtual void v##n##a(); virtual void v##n##b(); virtual void v##n##c(); virtual void v##n##d();
class CTCBase { public:
    V4(0) V4(1) V4(2) V4(3) V4(4) V4(5) V4(6) V4(7) V4(8) V4(9) V4(10) V4(11) V4(12) V4(13) V4(14) V4(15)
    V4(16) V4(17) V4(18) V4(19) V4(20) V4(21) V4(22)              // 0-91
    virtual void v92();
    CThing*         ParentThing;                    // +0x04
protected:
    const CWorld& PeekWorld() const;                // 0x00686A80 (Ego_r + FableWin ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ; GetWorld is 0x00686A70)
};
class CTCPhysicsBase : public CTCBase { public:
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() const = 0;   // slot 93 (+0x174)
    C3DVector GetMapwhoPosition() const;            // 0x006AFCB0 (Ego_r ?GetMapwhoPosition@CTCPhysicsBase@@QBE?AVC3DVector@@XZ)
    unsigned char   _pad08[0x34 - 0x08];
    float           Radius;                         // +0x34
protected:
    float CalculateRadius() const;                  // 0x006AFE20 (Ego_r ?CalculateRadius@CTCPhysicsBase@@IBEMXZ)
};
class CTCPhysicsRigidBody : public CTCPhysicsBase { public:
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() const;   // slot 93 (+0x174), Ego_r UBE
protected:
    virtual void SetPhysicsMeshBankIndex(long index);   // slot 94, Ego_r ?SetPhysicsMeshBankIndex@CTCPhysicsRigidBody@@MAEXJ@Z
public:
    unsigned char   _pad38[0x64 - 0x38];
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;        // +0x64
    CRightHandedSet RHSet;                          // +0x68
    CRightHandedSet OldRHSet;                       // +0x80
    C2DVector       OffsetVector;                   // +0x98
};

void CTCPhysicsRigidBody::SetPhysicsMeshBankIndex(long index)
{
    if (!PPhysicsMeshInfo)
        PPhysicsMeshInfo = new CThingPhysicsMeshInfo;
    PPhysicsMeshInfo->BankIndex = (unsigned short)index;
    PPhysicsMeshInfo->RenderSizeX = ParentThing->PGraphicAppearance->GetScale();
    CMeshDataBank* bank = PeekWorld().PMainMeshBank;
    const CPhysicsMeshInfo* meshInfo = bank->PeekPhysicsMeshInfo(index);
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