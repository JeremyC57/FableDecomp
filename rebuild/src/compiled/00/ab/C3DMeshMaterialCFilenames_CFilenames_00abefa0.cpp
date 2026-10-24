// C3DMeshMaterial::CFilenames::~CFilenames() -- destroy the material's four map filenames (illumination, reflection, bump, diffuse), then the intrusive counted-pointee base
// Leaf: ??1CFilenames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABEFA0, length 43 bytes, plain `ret`; no call or address references (the
// class's ??_G inlines it; this out-of-line copy is an unreferenced COMDAT). It sits right after the CFilenames constructor
// 0x00ABEF60 (vtable 0x0129EDB4, IVRefCount = 1, four CCharString ctors at +8..+0x14). The manifest label
// `_Dest_val<allocator<CMacro>,CMacro>` is propagated over the byte-identical shape (CMacro has no vtable) and wrong.
// Layout: Ego_r.pdb CFilenames (0x18) = CIVCountedPointeeBase {vptr, IVRefCount +4}, CCharString DiffuseMapFilename +8,
// BumpMapFilename +0xc, ReflectionMapFilename +0x10, IlluminationMapFilename +0x14.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x4); vtable 0x012354D0 = CIVCountedPointeeBase (rtti-vtables.tsv).
// Shape: CFilenames declares no destructor, so VC7.1's generated destructor stores no vtable of its own (GOTCHAS wave 25);
// the strings go in reverse order and the base's inline user destructor contributes the closing base-vtable store.
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

class C3DMeshMaterial {
public:
    class CFilenames : public CIVCountedPointeeBase {        // vtable 0x0129EDB4, 0x18 bytes
    public:
        CCharString DiffuseMapFilename;                      // +0x08
        CCharString BumpMapFilename;                         // +0x0c
        CCharString ReflectionMapFilename;                   // +0x10
        CCharString IlluminationMapFilename;                 // +0x14
    };
};

void DestroyFilenames(C3DMeshMaterial::CFilenames* filenames)   // emits the implicit destructor; not part of the retail body
{
    filenames->C3DMeshMaterial::CFilenames::~CFilenames();
}