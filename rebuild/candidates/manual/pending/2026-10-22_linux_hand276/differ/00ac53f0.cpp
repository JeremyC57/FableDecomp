// C3DMeshFileEngineMaterialChunk::C3DMeshFileEngineMaterialChunk(const C3DMeshFileMaterialChunk&) -- build the engine material from a file material: index, name, two-sided, self-illumination from the extended properties, map filenames by 3ds Max map channel (-1/1 diffuse, 5 illumination, 6 opacity = transparent, 8 bump, 9 reflection), then the map flags
// Grade: DIFFER(703v755) -- retail builds an `and esp,-8` frame with ebp as frame pointer (material in edi, no zero register, immediate zero stores); ours uses ebp for `material` and a zero register in ebx. Without the switch the lane does produce the aligned frame (probe ac53f0_r1), so the switch blocks (4 CCharString temporaries) change VC's frame decision
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC53F0, length 755 bytes (`ret 4`; jump table + byte index table at 0x00AC56C0 / 0x00AC56D8).
// Callees: C3DMeshFileChunk ctor 0x00AA8360 ("EMTL"; manifest "CShopItemDef::CShopItemDef" propagated), CCharString 0x0099E4B0 /
//   copy 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0, HasExtendedProperties 0x00ABFCD0, GetExtendedProperties 0x00AC0B70
//   (manifest "C3DMeshFileSubMeshChunk::FindFirstBone" propagated), GetSelfIllumination 0x00ABFDC0 (manifest
//   "CWorld::DrawGetEnvironment" propagated), FindFirstTexture 0x00AC0E50 / FindNextTexture 0x00AC0EE0 (manifest
//   FindFirstBone/FindNextBone propagated), CalculateMapFlags 0x00AC5260, operator new 0x00BFEA1A, operator delete 0x00BFE9BC.
// Layout: retail chunk base 0x2c (PDB 0x34): MaterialIndex +0x2c, Name +0x30, MapFilenames +0x34, bank indices +0x38..+0x44,
//   MapFlags +0x48, SelfIllumination +0x4c, TwoSided/Transparent/BooleanAlpha/DegenerateTriangles +0x50..+0x53.
// Leaf: C3DMeshFileEngineMaterialChunk
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template <class T> struct CCountedPointer {
    T* Object;
    SCountBlock* Count;
    CCountedPointer() : Object(0), Count(0) {}
    struct CastTag {};
    template<class U> CCountedPointer(const CCountedPointer<U>& other) : Object((T*)other.Object), Count(other.Count) { if (Count) ++Count->references; }
    template<class U> CCountedPointer(const CCountedPointer<U>& other, CastTag) : Object(0), Count(0)
    {
        if (other.Count) { Object = (T*)other.Object; Count = other.Count; ++Count->references; }
    }
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->references; }
    ~CCountedPointer()
    {
        if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
        Object = 0;
        Count = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* c = other.Count; T* p = other.Object;
        if (Count != c) {
            if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
            Object = p; Count = c; if (c) ++c->references;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
struct CChunkedFileChunk;
struct C3DMeshFileChunk {
    virtual void v00();
    unsigned char _pad04[0x28];
    C3DMeshFileChunk(const char* id);                  // 0x00AA8360
};
struct C3DMeshFileTextureChunk : C3DMeshFileChunk {
    long MapType;                                      // +0x2c (3ds Max map channel, -1 = none)
    unsigned char _pad30[4];
    CCharString Filename;                              // +0x34
    CCharString GetFilename() const { return Filename; }
};
struct C3DMeshFileExtendedMaterialChunk {
    unsigned long GetSelfIllumination() const;         // 0x00ABFDC0
};
struct CChunkedFileSuperChunk {
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;                       // 0x00AA7F40
    CCountedPointer<CChunkedFileChunk> FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>& prev) const; // 0x00AA7FF0
};
struct C3DMeshFileMaterialChunk : C3DMeshFileChunk, CChunkedFileSuperChunk {
    long MaterialIndex;                                // +0x2c
    bool TwoSided;                                     // +0x30
    unsigned char _pad31[0x1b];
    CCharString Name;                                  // +0x4c
    long GetMaterialIndex() const { return MaterialIndex; }
    CCharString GetName() const { return Name; }
    bool GetTwoSided() const { return TwoSided; }
    bool HasExtendedProperties() const;                // 0x00ABFCD0
    CCountedPointer<C3DMeshFileExtendedMaterialChunk> GetExtendedProperties() const;   // 0x00AC0B70
    CCountedPointer<C3DMeshFileTextureChunk> FindFirstTexture() const;                  // 0x00AC0E50
    CCountedPointer<C3DMeshFileTextureChunk> FindNextTexture(const CCountedPointer<C3DMeshFileTextureChunk>& prev) const;   // 0x00AC0EE0
};
#pragma auto_inline(off)
CCountedPointer<C3DMeshFileTextureChunk> C3DMeshFileMaterialChunk::FindFirstTexture() const
{
    CCountedPointer<C3DMeshFileTextureChunk> texture(FindFirstSubChunkOfType("TXTR"), CCountedPointer<C3DMeshFileTextureChunk>::CastTag());
    return texture;
}
CCountedPointer<C3DMeshFileTextureChunk> C3DMeshFileMaterialChunk::FindNextTexture(const CCountedPointer<C3DMeshFileTextureChunk>& current) const
{
    CCountedPointer<CChunkedFileChunk> prev(current);
    CCountedPointer<C3DMeshFileTextureChunk> texture(FindNextSubChunk(prev), CCountedPointer<C3DMeshFileTextureChunk>::CastTag());
    return texture;
}
#pragma auto_inline(on)
struct CIVCountedObject {
    virtual void v00();
    virtual void DeleteThis();                         // slot 1
    long RefCount;                                     // +4
    CIVCountedObject() : RefCount(1) {}
};
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer& operator=(T* object)
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->DeleteThis();
            Object = 0;
        }
        Object = object;
        return *this;
    }
    T* operator->() const { return Object; }
};

class C3DMeshFileEngineMaterialChunk : public C3DMeshFileChunk {
public:
    struct CMapFilenames : CIVCountedObject {          // vtable 0x0129EFBC, 0x18 bytes
        CCharString DiffuseMap;                        // +0x08
        CCharString BumpMap;                           // +0x0c
        CCharString ReflectionMap;                     // +0x10
        CCharString IlluminationMap;                   // +0x14
        virtual void v00();
    };
    long MaterialIndex;                                // +0x2c
    CCharString Name;                                  // +0x30
    CIVCountedPointer<CMapFilenames> MapFilenames;     // +0x34
    unsigned long DiffuseMapBankIndex;                 // +0x38
    unsigned long BumpMapBankIndex;                    // +0x3c
    unsigned long ReflectionMapBankIndex;              // +0x40
    unsigned long IlluminationMapBankIndex;            // +0x44
    unsigned long MapFlags;                            // +0x48
    unsigned long SelfIllumination;                    // +0x4c
    bool TwoSided;                                     // +0x50
    bool Transparent;                                  // +0x51
    bool BooleanAlpha;                                 // +0x52
    bool DegenerateTriangles;                          // +0x53
    virtual void v00();
    C3DMeshFileEngineMaterialChunk(const C3DMeshFileMaterialChunk& material);
    void CalculateMapFlags();                          // 0x00AC5260
};

C3DMeshFileEngineMaterialChunk::C3DMeshFileEngineMaterialChunk(const C3DMeshFileMaterialChunk& material)
    : C3DMeshFileChunk("EMTL"), DegenerateTriangles(false)
{
    MaterialIndex = material.GetMaterialIndex();
    Name = material.GetName();
    TwoSided = material.GetTwoSided();
    SelfIllumination = 0;
    if (material.HasExtendedProperties()) {
        CCountedPointer<C3DMeshFileExtendedMaterialChunk> extended = material.GetExtendedProperties();
        SelfIllumination = extended->GetSelfIllumination();
    }
    Transparent = false;
    BooleanAlpha = false;
    MapFilenames = new CMapFilenames;
    CCountedPointer<C3DMeshFileTextureChunk> texture = material.FindFirstTexture();
    while (texture.Object) {
        switch (texture->MapType) {
        case -1:
        case 1:
            {
                CCharString name = texture->GetFilename();
                MapFilenames->DiffuseMap = name;
            }
            break;
        case 5:
            {
                CCharString name = texture->GetFilename();
                MapFilenames->IlluminationMap = name;
            }
            break;
        case 6:
            Transparent = true;
            break;
        case 8:
            {
                CCharString name = texture->GetFilename();
                MapFilenames->BumpMap = name;
            }
            break;
        case 9:
            {
                CCharString name = texture->GetFilename();
                MapFilenames->ReflectionMap = name;
            }
            break;
        }
        texture = material.FindNextTexture(texture);
    }
    DiffuseMapBankIndex = 0;
    BumpMapBankIndex = 0;
    ReflectionMapBankIndex = 0;
    IlluminationMapBankIndex = 0;
    CalculateMapFlags();
}
