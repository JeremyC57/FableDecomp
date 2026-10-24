// C3DMeshFileEngineMaterialChunk::CMapFilenames::~CMapFilenames() -- destroy the chunk record's four map names (illumination, reflection, bump, diffuse), then the intrusive counted-pointee base
// Leaf: ??1CMapFilenames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC6210, length 43 bytes (+ int3 padding), plain `ret`; called only by
// CMapFilenames' scalar deleting destructor 0x00AC61F0 (landed). The constructor is the landed 0x00AC5190 (vtable
// 0x0129EFBC, IVRefCount = 1, four CCharStrings at +8..+0x14). The manifest label `_Dest_val<allocator<CMacro>,CMacro>`
// is propagated over the byte-identical shape and wrong.
// Layout: Ego_r.pdb CMapFilenames (0x18) = CIVCountedPointeeBase {vptr, IVRefCount +4}, CCharString DiffuseMap +8,
// BumpMap +0xc, ReflectionMap +0x10, IlluminationMap +0x14.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x4); vtable 0x012354D0 = CIVCountedPointeeBase (rtti-vtables.tsv).
// Shape: CMapFilenames declares no destructor, so VC7.1's generated destructor stores no vtable of its own (GOTCHAS wave
// 25); the strings go in reverse order and the base's inline user destructor contributes the closing base-vtable store.
// The engine header CIVCountedPointeeBase.h flattens the vptr into a data member, so the polymorphic base is modelled here.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

class CIVCountedPointeeBase {                                // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void DeleteThis();                               // slot 1 (+4)
    unsigned long IVRefCount;                                // +0x04
};

class C3DMeshFileEngineMaterialChunk {
public:
    class CMapFilenames : public CIVCountedPointeeBase {     // vtable 0x0129EFBC, 0x18 bytes
    public:
        CCharString DiffuseMap;                              // +0x08
        CCharString BumpMap;                                 // +0x0c
        CCharString ReflectionMap;                           // +0x10
        CCharString IlluminationMap;                         // +0x14
    };
};

void DestroyMapFilenames(C3DMeshFileEngineMaterialChunk::CMapFilenames* names)   // emits the implicit destructor; not part of the retail body
{
    names->C3DMeshFileEngineMaterialChunk::CMapFilenames::~CMapFilenames();
}