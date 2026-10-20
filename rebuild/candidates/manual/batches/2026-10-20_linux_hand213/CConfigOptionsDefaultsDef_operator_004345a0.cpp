// CConfigOptionsDefaultsDef::operator=(const CConfigOptionsDefaultsDef&) -- def copy with the CDefClassBase/CParentDefClassBase chain inlined (root operator=, manager/index words, three flag bits, instantiation name, sub-def map), then the fourteen video-config defaults; returns *this
// Leaf: ??4CConfigOptionsDefaultsDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004345A0, length 192 bytes.
// Identity: the manifest label "CThingBuildingDef::operator=" is propagated. The body sits behind the
//   5-byte `jmp` thunk at 0x00434590 = slot 19 of vtable 0x0123125C = CConfigOptionsDefaultsDef
//   (rtti-vtables.tsv); PDB CConfigOptionsDefaultsDef = 14 words after a 0x40 CParentDefClassBase,
//   here at +0x3c..+0x70 (the retail parent base is 0x3c bytes).
// Callees: CDefClassBase's polymorphic root operator= @ 0x00431F60 (copies +0x04..+0x18; the manifest
//   name "CTCPhysicsBase::ConstructFromParams" is propagated) and SubDefInfoMap's 12-byte vector
//   operator= @ 0x00431F90.
// Layout: same def base chain as the landed CFrontEndDef::operator= 0x00432E60 / CEngineVideoOptionsDef
//   0x00434450 -- the retail CParentDefClassBase is byte-packed (dword at +0x25, vector at +0x29, sorted
//   flag at +0x36 are unaligned in the retail copy), so the `#pragma pack(1)` reproduces a proven native
//   layout. The three 1-bit fields give the xor/and/xor read-modify-write byte sequence. This TU (the
//   CDefClassBase module at 0x4345xx) is speed-compiled: copies rotate through eax/ecx/edx.
// No engine/ header exists for the class: modelled locally from struct_layouts_egor.tsv.
#pragma pack(push, 1)
struct CDefRootBase {                                   // 28-byte polymorphic root of CDefClassBase
    virtual void v00();
    unsigned long m_words[6];                           // +0x04..+0x18
    CDefRootBase& operator=(const CDefRootBase& other); // 0x00431F60
};
struct CDefinitionManager;
struct CDefClassBase : CDefRootBase {
    CDefinitionManager* PDefManager;                    // +0x1c
    unsigned long GlobalIndex;                          // +0x20
    unsigned char Setup : 1;                            // +0x24 bit 0
    unsigned char Template : 1;                         // +0x24 bit 1
    unsigned char DefaultValsApplied : 1;               // +0x24 bit 2
    __forceinline CDefClassBase& operator=(const CDefClassBase& other)
    {
        CDefRootBase::operator=(other);
        PDefManager = other.PDefManager;
        GlobalIndex = other.GlobalIndex;
        Setup = other.Setup;
        Template = other.Template;
        DefaultValsApplied = other.DefaultValsApplied;
        return *this;
    }
};
#pragma pack(pop)

template<class T> struct CRetailVector {                // begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector& operator=(const CRetailVector& other); // CSubDefInfo instance: 0x00431F90
};
struct CKeyPairCompareLess {};
template<class K, class V> struct CVectorMap {          // retail: 14 bytes
    CRetailVector<V>    m_vector;                       // +0x00
    CKeyPairCompareLess m_compare;                      // +0x0c (empty)
    bool                m_sorted;                       // +0x0d
    __forceinline CVectorMap& operator=(const CVectorMap& other)
    {
        m_vector = other.m_vector;
        m_compare = other.m_compare;
        m_sorted = other.m_sorted;
        return *this;
    }
};
struct CDefString { long TablePos; };
struct CSubDefInfo;

#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {
    CDefString                             InstantiationName;   // +0x25
    CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap;       // +0x29
    __forceinline CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        CDefClassBase::operator=(other);
        InstantiationName = other.InstantiationName;
        SubDefInfoMap = other.SubDefInfoMap;
        return *this;
    }
};
#pragma pack(pop)

struct CConfigOptionsDefaultsDef : CParentDefClassBase {        // vtable 0x0123125C
    unsigned long Antialiasing;                         // +0x3c
    long  ResolutionWidth;                              // +0x40
    long  ResolutionHeight;                             // +0x44
    long  BitDepth;                                     // +0x48
    long  MinResolutionWidth;                           // +0x4c
    long  MinResolutionHeight;                          // +0x50
    float TextureDetail;                                // +0x54
    float MaxTextureDetail;                             // +0x58
    float ShadowDetail;                                 // +0x5c
    float MaxShadowDetail;                              // +0x60
    float MeshDetail;                                   // +0x64
    float MaxMeshDetail;                                // +0x68
    float EffectsDetail;                                // +0x6c
    float MaxEffectsDetail;                             // +0x70
    CConfigOptionsDefaultsDef& operator=(const CConfigOptionsDefaultsDef& other);
};

CConfigOptionsDefaultsDef& CConfigOptionsDefaultsDef::operator=(const CConfigOptionsDefaultsDef& other)
{
    CParentDefClassBase::operator=(other);
    Antialiasing = other.Antialiasing;
    ResolutionWidth = other.ResolutionWidth;
    ResolutionHeight = other.ResolutionHeight;
    BitDepth = other.BitDepth;
    MinResolutionWidth = other.MinResolutionWidth;
    MinResolutionHeight = other.MinResolutionHeight;
    TextureDetail = other.TextureDetail;
    MaxTextureDetail = other.MaxTextureDetail;
    ShadowDetail = other.ShadowDetail;
    MaxShadowDetail = other.MaxShadowDetail;
    MeshDetail = other.MeshDetail;
    MaxMeshDetail = other.MaxMeshDetail;
    EffectsDetail = other.EffectsDetail;
    MaxEffectsDetail = other.MaxEffectsDetail;
    return *this;
}
