// CThingObjectDef::Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const -- release-build validation: copies the three validation label strings, fetches the def's graphic from the world's main mesh bank (static graphic for type 5, animated for type 4), checks an animated object's appearance def for its idle animation, and for 0x100/0x200 class objects touches the graphic's mesh with the CRC of the shared name string (the checks' results are discarded)
// Grade: DIFFER(432v439) -- conditional temporaries (flags 3 and 4), the def chain, the counted-pointer release and teardown match; two residues: retail places the empty-string arm of the inline c_str/length (mov edi,"" ; jmp back) as an out-of-line block after the epilogue where ours keeps it inline, and the static-graphic index goes through edx (ours ecx). Tried an if/return c_str and an inline CRC helper (lost the frame packing)
// Leaf: ?Validate@CThingObjectDef
// Callees: CCharString copy ctor @ 0x0099EC30 / (const char*, long) ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0, CMeshBank @ 0x00A24280 /
//          0x00A242B0, CThingBaseDef::GetSubDef<CAppearanceDef> @ 0x0042B0A2, CAnimationSet::Find @ 0x005DC2E0, the graphic's
//          counted-pointer getter @ 0x00A25530, CRC @ 0x004014A0 (__fastcall), the mesh's 0x00A2EA60, operator delete @ 0x00BFE9BC.
// Layout: CValidateDefinitionInitBase world +0x08; CWorld main mesh bank +0x3c; CThingObjectDef graphic index +0x70, graphic
//          type +0x7d, class flags +0x90 (descriptive names); CAnimFilterNoGroup vtable 0x0122FFDC (base 0x0122FFCC).
struct CCharStringData {
    char* m_str;                                        // +0x00
    long  m_length;                                     // +0x04
};
struct CCharString {
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const char* c_str() const { return m_pData ? m_pData->m_str : ""; }
    long Length() const { return m_pData ? m_pData->m_length : 0; }
};
extern const CCharString g_ValidateLabelA;              // 0x013BAC38
extern const CCharString g_ValidateLabelB;              // 0x013BAC3C
extern const CCharString g_ValidateLabelC;              // 0x013BAC30
extern const CCharString g_ValidateMeshName;            // 0x013BACA0
unsigned long __fastcall CRC(unsigned long seed, const char* text, long length);   // 0x004014A0

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template<class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    T* operator->() const { return m_pObject; }
    __forceinline ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};
struct CMesh {
    void SetNameCRC(unsigned long crc);                 // 0x00A2EA60
};
struct CGraphic {
    CCountedPointer<CMesh> GetMesh();                   // 0x00A25530
};
struct CMeshBank {
    CGraphic* GetStaticGraphic(long index);             // 0x00A24280
    CGraphic* GetAnimatedGraphic(long index);           // 0x00A242B0
};
struct CWorld {
    unsigned char _pad[0x3c];
    CMeshBank* PMainMeshBank;                           // +0x3c
};
struct CValidateDefinitionInitBase {
    unsigned char _pad[0x08];
    CWorld* PWorld;                                     // +0x08
};

struct CAnimFilterBase {                                // vtable 0x0122FFCC
    virtual void v000();
    virtual void v001();
    virtual void v002();
    ~CAnimFilterBase() { }
};
struct CAnimFilterNoGroup : CAnimFilterBase {           // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
    ~CAnimFilterNoGroup() { }
    virtual void v000();
    virtual void v001();
    virtual void v002();
};
struct CAnimationSet {
    bool Find(const CCharString& name, const CAnimFilterBase* filter) const;   // 0x005DC2E0
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CAppearanceDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                            // +0x34
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
void* FindDef_0050();
struct CThingBaseDef {
    virtual void v00();
    bool GetSubDef(CDefPointer<const CAppearanceDef>* out) const;     // 0x0042B0A2
};
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<const CAppearanceDef>* out) const
{
    void* def = FindDef_0050();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThingObjectDef : CThingBaseDef {
    unsigned char _pad04[0x70 - 0x04];
    long          GraphicIndex;                         // +0x70
    unsigned char _pad74[0x7d - 0x74];
    unsigned char GraphicType;                          // +0x7d
    unsigned char _pad7e[0x90 - 0x7e];
    long          ObjectClass;                          // +0x90
    virtual void Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const;
};

void CThingObjectDef::Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const
{
    CWorld* world = init.PWorld;
    CCharString labelA(g_ValidateLabelA);
    CCharString labelB(g_ValidateLabelB);
    CCharString labelC(g_ValidateLabelC);
    CGraphic* graphic = 0;
    if (GraphicType == 5 && GraphicIndex)
        graphic = world->PMainMeshBank->GetStaticGraphic(GraphicIndex);
    else if (GraphicType == 4 && GraphicIndex)
        graphic = world->PMainMeshBank->GetAnimatedGraphic(GraphicIndex);
    CDefPointer<const CAppearanceDef> def;
    GraphicType == 4 && GetSubDef(&def) && def->Animation.Find("DEFAULT", &CAnimFilterNoGroup());
    if (ObjectClass == 0x100 || ObjectClass == 0x200)
        graphic && (graphic->GetMesh()->SetNameCRC(CRC(0, g_ValidateMeshName.c_str(), g_ValidateMeshName.Length())), true);
    const CDefClassBase* object = def.Object;
    if (object && --const_cast<CDefClassBase*>(object)->RefCount == 0)
        const_cast<CDefClassBase*>(object)->Destroy();
}
