// NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::ReplaceMeshByIndex(unsigned long index) -- switch the owner's main graphic to the def's replacement mesh `index`, rebuild its physics mesh and broadcast CMessageOnMeshReplaced
// Leaf: ReplaceMeshByIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A69F0, length 180 bytes (+ int3 padding). __fastcall, `ret 4`.
// Callees: NGraphicAppearance::CTCGraphicAppearance::SetAsChanged(unsigned long) @ 0x004BF9E0, CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound @ 0x0040F020 (interface 2 = the physics component, word 0 bit 2),
//          CTCPhysicsBase::CreatePPhysicsMeshInfo(const CEngineGraphic&) @ 0x006B08E0, CBaseClass ctor/dtor @
//          0x0099A2D0/0x0099A2E0 (NTCMessages::CMessageOnMeshReplaced, vtable 0x0126CB54),
//          CThing::SendMessageW(const NTCMessages::CMessageBase&) @ 0x004C7820.
// Shape: the chosen mesh is a pointer into the def's 14-byte CEngineGraphic array (`imul 0Eh`, kept in ebx), copied into
//        MainGraphic as three dwords and a word (pack(2)); the physics component comes from the inline accessor that
//        leaves its result unset when absent, and its key, that unset value and the 4-byte message all live in the dead
//        `index` argument slot (this body opens no frame).
// Layout: Ego_r.pdb CTCGraphicAppearanceReplaceableMesh (matches retail): Replaced +0x10, MeshIndex +0x14, PDef +0x1C;
//         CReplaceableMeshDef::Meshes +0x28 (first word: the CEngineGraphic array); retail CThing PTCGraphicAppearance
//         +0x64 and CTCGraphicAppearance::MainGraphic +0x44 (PDB +0x5C: two 12-byte maps and two 4-byte lists).
class CTCBase;
class CThing;

class CBaseClass {                                  // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                   // 0x0099A2D0
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
namespace NTCMessages {
class CMessageBase : public CBaseClass {
public:
    virtual void v000();
};
class CMessageOnMeshReplaced : public CMessageBase {   // vtable 0x0126CB54
public:
    virtual void v000();
};
}

#pragma pack(push, 2)
class CEngineGraphic {                              // 14 bytes
public:
    unsigned long BankIndex;                        // +0x00
    float AnimStep;                                 // +0x04
    float RenderSizeX;                              // +0x08
    bool AdditiveAlpha;                             // +0x0C
    unsigned char Type;                             // +0x0D
};
#pragma pack(pop)

class CReplaceableMeshDef {
public:
    unsigned char _pad00[0x28];
    CEngineGraphic* Meshes;                         // +0x28 CReplaceableMeshes (first word: the graphic array)
};

class CTCPhysicsBase {                              // interface 2
public:
    void CreatePPhysicsMeshInfo(const CEngineGraphic& graphic);   // 0x006B08E0
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char _pad00[0x44];
    CEngineGraphic MainGraphic;                     // +0x44
    void SetAsChanged(unsigned long reasons);       // 0x004BF9E0
};
}

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    void SendMessageW(const NTCMessages::CMessageBase& message);       // 0x004C7820
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
    __forceinline CTCPhysicsBase* GetPhysicsTC()
    {
        CTCPhysicsBase* physics;
        if (HasTC(2))
            physics = (CTCPhysicsBase*)GetTC(2);
        return physics;
    }
};

namespace NGraphicAppearance {
class CTCGraphicAppearanceReplaceableMesh {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    bool Replaced;                                  // +0x10
    unsigned long MeshIndex;                        // +0x14
    void* PrimHandle;                               // +0x18 CPrimitive*
    const CReplaceableMeshDef* PDef;                // +0x1C CDefPointer<const CReplaceableMeshDef>::Object
    void ReplaceMeshByIndex(unsigned long index);
};

void CTCGraphicAppearanceReplaceableMesh::ReplaceMeshByIndex(unsigned long index)
{
    MeshIndex = index;
    Replaced = true;
    const CEngineGraphic& mesh = PDef->Meshes[index];
    ParentThing->PTCGraphicAppearance->MainGraphic = mesh;
    ParentThing->PTCGraphicAppearance->SetAsChanged(4);
    ParentThing->GetPhysicsTC()->CreatePPhysicsMeshInfo(mesh);
    ParentThing->SendMessageW(NTCMessages::CMessageOnMeshReplaced());
}
}