// C3DMeshMaterial::InitialiseFromFileData(const C3DMeshFileEngineMaterialChunk&, CBankFile*) -- copy the material fields from the file chunk, resolve the four map filenames to bank indices and keep a copy of the filenames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABF840, length 528 bytes (`ret 8`).
// Callees: CCharString copy ctor @ 0x0099EC30, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0,
//          CBankFile::FindIndexByFilename(const CCharString&, long) @ 0x009CCDF0, operator new @ 0x00BFEA1A,
//          C3DMeshMaterial::CFilenames::CFilenames @ 0x00ABEF60, CFilenames slot-1 destroy (virtual).
// Shape: every chunk string goes through an inline by-value getter, so each use builds a CCharString copy in the dead
// chunk-argument slot (copy ctor, use, dtor). MaterialIndex, Name, DecalID = 0, SelfIllumination, the four flags and
// MapFlags are copied; with DegenerateTriangles set the four bank indices are cleared and nothing else happens;
// otherwise each map name is looked up in the bank (the bump map with entry type 2), and unless the bank has its
// +0x8c flag set a fresh CFilenames is attached (the intrusive pointer's assignment releases the old one through
// slot 1 when its count drops to zero, clears the pointer, then stores the new one) and the four names copied in.
// Layout: C3DMeshMaterial from the landed ctor 0x00ABF7B0 / SaveBinary 0x00ABF6B0; retail C3DMeshFileChunk base is
// 0x2c bytes, so the engine chunk's members sit 8 bytes below the PDB header (MaterialIndex +0x2c, Name +0x30,
// MapFilenames +0x34, MapFlags +0x48, SelfIllumination +0x4c, flags +0x50..+0x53); CFilenames names at +8..+0x14.
// Leaf: InitialiseFromFileData
class CCharString { public:
    void* m_pStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CBankFile { public:
    unsigned char _pad[0x8c];
    bool m_noFilenameCopy;                              // +0x8c (retail byte; set: materials keep no filename copy)
    unsigned long FindIndexByFilename(const CCharString& filename, long entryType);   // 0x009CCDF0
};

struct CCountedFilenames {
    virtual void v00();
    virtual void DeleteThis();                          // slot 1
    long RefCount;                                      // +0x04
    CCharString Diffuse;                                // +0x08
    CCharString Bump;                                   // +0x0c
    CCharString Reflection;                             // +0x10
    CCharString Illumination;                           // +0x14
    CCharString GetDiffuse() const { return Diffuse; }
    CCharString GetBump() const { return Bump; }
    CCharString GetReflection() const { return Reflection; }
    CCharString GetIllumination() const { return Illumination; }
};
struct CFilenames : CCountedFilenames {
    CFilenames();                                       // 0x00ABEF60
};

class C3DMeshFileEngineMaterialChunk { public:
    unsigned char _base[0x2c];                          // retail C3DMeshFileChunk (PDB 0x34)
    long MaterialIndex;                                 // +0x2c
    CCharString Name;                                   // +0x30
    CCountedFilenames* MapFilenames;                    // +0x34
    unsigned long DiffuseMapBankIndex;                  // +0x38
    unsigned long BumpMapBankIndex;                     // +0x3c
    unsigned long ReflectionMapBankIndex;               // +0x40
    unsigned long IlluminationMapBankIndex;             // +0x44
    unsigned long MapFlags;                             // +0x48
    unsigned long SelfIllumination;                     // +0x4c
    bool TwoSided;                                      // +0x50
    bool Transparent;                                   // +0x51
    bool BooleanAlpha;                                  // +0x52
    bool DegenerateTriangles;                           // +0x53
    CCharString GetName() const { return Name; }
};

template<class T> class CIVCountedPointer { public:
    T* Object;
    void operator=(T* object)
    {
        if (T* old = Object) {
            if (--old->RefCount == 0)
                old->DeleteThis();
            Object = 0;
        }
        Object = object;
    }
    T* operator->() const { return Object; }
};

class CBaseClass { public: virtual void v00(); };
class C3DMeshMaterial : public CBaseClass { public:
    long MaterialIndex;                                 // +0x04
    CCharString Name;                                   // +0x08
    long DecalID;                                       // +0x0c
    unsigned long DiffuseMapBankIndex;                  // +0x10
    unsigned long BumpMapBankIndex;                     // +0x14
    unsigned long ReflectionMapBankIndex;               // +0x18
    unsigned long IlluminationMapBankIndex;             // +0x1c
    unsigned long MapFlags;                             // +0x20
    unsigned long SelfIllumination;                     // +0x24
    bool TwoSided;                                      // +0x28
    bool Transparent;                                   // +0x29
    bool BooleanAlpha;                                  // +0x2a
    bool DegenerateTriangles;                           // +0x2b
    CIVCountedPointer<CCountedFilenames> Filenames;     // +0x2c
    void InitialiseFromFileData(const C3DMeshFileEngineMaterialChunk& chunk, CBankFile* bank);
};

void C3DMeshMaterial::InitialiseFromFileData(const C3DMeshFileEngineMaterialChunk& chunk, CBankFile* bank)
{
    MaterialIndex = chunk.MaterialIndex;
    Name = chunk.GetName();
    DecalID = 0;
    SelfIllumination = chunk.SelfIllumination;
    TwoSided = chunk.TwoSided;
    Transparent = chunk.Transparent;
    BooleanAlpha = chunk.BooleanAlpha;
    DegenerateTriangles = chunk.DegenerateTriangles;
    MapFlags = chunk.MapFlags;
    if (chunk.DegenerateTriangles) {
        DiffuseMapBankIndex = 0;
        BumpMapBankIndex = 0;
        ReflectionMapBankIndex = 0;
        IlluminationMapBankIndex = 0;
        return;
    }
    DiffuseMapBankIndex = bank->FindIndexByFilename(chunk.MapFilenames->GetDiffuse(), 0);
    BumpMapBankIndex = bank->FindIndexByFilename(chunk.MapFilenames->GetBump(), 2);
    ReflectionMapBankIndex = bank->FindIndexByFilename(chunk.MapFilenames->GetReflection(), 0);
    IlluminationMapBankIndex = bank->FindIndexByFilename(chunk.MapFilenames->GetIllumination(), 0);
    if (bank == 0 || !bank->m_noFilenameCopy) {
        Filenames = new CFilenames;
        Filenames->Diffuse = chunk.MapFilenames->GetDiffuse();
        Filenames->Bump = chunk.MapFilenames->GetBump();
        Filenames->Reflection = chunk.MapFilenames->GetReflection();
        Filenames->Illumination = chunk.MapFilenames->GetIllumination();
    }
}