// std::vector<CMultiStaticMeshEntryDef>::resize(unsigned int n) -- one-argument resize: build a default CMultiStaticMeshEntryDef temporary, call resize(n, value), destroy the temporary
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EDF0A, 58 bytes (a /Os module: unaligned entry, ebp
// frame, `leave`), `ret 4`.
// Label correction: the catalog name ~WaypointData is propagated.  The temporary's vtable 0x012438A4
// is CMultiStaticMeshEntryDef (rtti-vtables.tsv), and the only caller 0x004EDEDE is the CMultiStaticMeshEntryDef
// TransferVectorOfSubComponents loop (resize, then each 0x38-byte element's virtual Transfer, slot
// 0x48).  The callee 0x004EDF44 (catalog "CArray<pair<ulong,CSoundPair>>::push_back", propagated) is
// the two-argument resize(n, const T&).
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (out of line in this module: CResource
// ctor, DefIndex / flag bits, its vtable 0x01231D54), resize(n, value) @ 0x004EDF44,
// CResource::~CResource @ 0x009FC550.
// CMultiStaticMeshEntryDef (Ego_r.pdb, 56 bytes) : CSubDefClassBase (vtable 0x01231D54) : CDefPointeeBase
// (vtable 0x01230BA0) : CResource; own members Mesh +0x28, OverrideLightingChannel +0x2c, OverrideRenderFadeDistance +0x2d,
// RenderFadeDistance +0x30, ELightingChannel LightingChannel +0x34 (not initialised by the
// default ctor).
// Shape (the landed 0x0067D610 model): the inline default ctor is the out-of-line base ctor plus the
// CMultiStaticMeshEntryDef vtable store; the derived destructors are IMPLICIT (no vtable store), the inline
// CDefPointeeBase destructor stores 0x01230BA0 before the out-of-line CResource destructor.
#pragma optimize("y", off)
#pragma optimize("s", on)

enum ELightingChannel { LIGHTING_CHANNEL_NONE = 0 };

struct CResource {
    virtual ~CResource();                          // 0x009FC550
    unsigned long m_refCount;                      // +0x04
    void*         m_pResourceList;                 // +0x08
    CResource*    m_pPrevResource;                 // +0x0c
    CResource*    m_pNextResource;                 // +0x10
    unsigned long m_resourceSize;                  // +0x14
    unsigned long m_lastUsedFrame;                 // +0x18
};

struct CDefPointeeBase : CResource {               // vtable 0x01230BA0
    virtual ~CDefPointeeBase() {}
};

struct CSubDefClassBase : CDefPointeeBase {        // vtable 0x01231D54
    long          m_defIndex;                      // +0x1c
    long          m_reserved;                      // +0x20
    unsigned char m_flags;                         // +0x24
    CSubDefClassBase();                            // 0x0044C0C0
};

struct CMultiStaticMeshEntryDef : CSubDefClassBase {         // vtable 0x012438A4, 0x38 bytes
    long  Mesh;                                    // +0x28
    bool  OverrideLightingChannel;                 // +0x2c
    bool  OverrideRenderFadeDistance;              // +0x2d
    float RenderFadeDistance;                      // +0x30
    ELightingChannel LightingChannel;              // +0x34
    CMultiStaticMeshEntryDef() {}
};

struct CMultiStaticMeshEntryDefVector {                      // std::vector<CMultiStaticMeshEntryDef>
    unsigned char _Alval;
    CMultiStaticMeshEntryDef* _Myfirst;
    CMultiStaticMeshEntryDef* _Mylast;
    CMultiStaticMeshEntryDef* _Myend;
    void resize(unsigned int n, const CMultiStaticMeshEntryDef& value);   // 0x004EDF44
    void resize(unsigned int n);
};

void CMultiStaticMeshEntryDefVector::resize(unsigned int n)
{
    resize(n, CMultiStaticMeshEntryDef());
}