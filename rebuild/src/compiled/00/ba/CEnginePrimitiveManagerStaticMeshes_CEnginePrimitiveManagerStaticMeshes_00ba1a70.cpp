// CEnginePrimitiveManagerStaticMeshes::CEnginePrimitiveManagerStaticMeshes(long type) -- the static-meshes base, this class's vtable, SetName("EnableStaticMeshes") through a CCharString temporary, the count at +0x314 set to 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA1A70, length 70 bytes.
// Callees: CEnginePrimitiveManagerStaticMeshesBase::CEnginePrimitiveManagerStaticMeshesBase @ 0x00B9ECE0 (the type), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 (the literal @ 0x012A5148, length -1), the name setter @ 0x00BACD80 (an empty `ret 4` in retail; the manifest label "$E2" is propagated),
// CCharString::~CCharString @ 0x0099EAE0 (the manifest label is a propagated _Cons_val symbol). Vtable 0x012A5114 = CEnginePrimitiveManagerStaticMeshes
// (rtti-vtables.tsv). Shape: the temporary lives in the caller's frame (`lea ecx,[esp+10h]` after the two pushes, `lea ecx,[esp+8]` after), the vtable store is
// scheduled between the pushes and the string constructor, the +0x314 store is an immediate 1.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

struct CEnginePrimitiveManagerStaticMeshesBase {
    virtual void v00();
    unsigned char _pad[0x310];                     // +0x04 .. +0x314
    CEnginePrimitiveManagerStaticMeshesBase(long type);   // 0x00B9ECE0
};

class CEnginePrimitiveManagerStaticMeshes : public CEnginePrimitiveManagerStaticMeshesBase { public:   // vtable 0x012A5114
    long m_count;                                  // +0x314
    virtual void v00();
    void SetName(const CCharString& name);         // 0x00BACD80
    CEnginePrimitiveManagerStaticMeshes(long type);
};

CEnginePrimitiveManagerStaticMeshes::CEnginePrimitiveManagerStaticMeshes(long type)
    : CEnginePrimitiveManagerStaticMeshesBase(type)
{
    {
        CCharString name("EnableStaticMeshes", -1); // a named local: its address goes to the setter (a bare temporary would push the constructor's eax)
        SetName(name);
    }                                              // destroyed before the count store (the store follows the destructor call)
    m_count = 1;
}