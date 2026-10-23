// Grade: DIFFER(525v533) -- conditional-temporary bookkeeping: retail keeps the cleanup flags in eax (test al / and eax / store), its CAnimFilterNoGroup temporary's destructor stores the base vtable 0x0122FFCC (ours drops the store), it places the two named CCharString copies at the top of the frame (ours at the bottom) and evaluates the type compare before loading the mesh-getter arguments; the shape (two global-string copies, type-5/4 mesh info, appearance sub-def, the result-discarded DEFAULT animation check, the dummy-object and helper-point checks with conditional CCountedPointer temporaries and inline CRC of the names) matches
// CThingCreatureDef::Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const -- the release
// build keeps only the side effects of the def checks (results unused): the creature's mesh info (by type), the
// appearance def's DEFAULT animation, and the mesh's dummy object / helper point named by the global strings
// Leaf: ?Validate@CThingCreatureDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00670D50, `ret 8`.
// Callees: CCharString copy 0x0099EC30 / (const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, the mesh bank's
//   0x00A24280 (type 5) / 0x00A242B0 (type 4) mesh-info getters, CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2,
//   CAnimationSet::HasAnimation 0x005DC2E0 (CAnimFilterNoGroup temporary, vtables 0x0122FFDC / base 0x0122FFCC),
//   C3DMeshInfo::GetMeshStats 0x00A25530, Calc (CRC) 0x004014A0, C3DMeshStats::HasDummyObject 0x00A2EA60 /
//   HasHelperPoint 0x00A2EA20, operator delete 0x00BFE9BC.
// Layout: retail CThingCreatureDef has the dummy check flag +0xd8, mesh index +0x100, creature type byte +0x10d;
//   init +8 = the resource holder whose +0x3c is the mesh bank. Globals: CCharStrings 0x013BAC30, 0x013BAC34,
//   0x013BAC48.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
struct CCharStringData {
    char*         m_pData;
    unsigned long m_length;
};
struct CCharString {
    CCharStringData* m_pStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetPString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pData; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0
inline unsigned long HashOf(const CCharString& s) { return Calc_004014a0(0, s.GetPString(), s.GetLength()); }
extern CCharString g_CCharString_013bac30;                  // 0x013BAC30
extern CCharString g_CCharString_013bac34;                  // 0x013BAC34
extern CCharString g_CCharString_013bac48;                  // 0x013BAC48

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    T* operator->() const { return m_pObject; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct C3DMeshStats {
    bool HasHelperPoint(unsigned long key) const;           // 0x00A2EA20
    bool HasDummyObject(unsigned long key) const;           // 0x00A2EA60
};
struct C3DMeshInfo {
    CCountedPointer<const C3DMeshStats> GetMeshStats() const;   // 0x00A25530
};
struct CMeshDataBank {
    const C3DMeshInfo* GetCreatureMeshInfo(long index);     // 0x00A24280
    const C3DMeshInfo* GetHeroMeshInfo(long index);         // 0x00A242B0
};
struct CValidateResources { unsigned char _pad[0x3c]; CMeshDataBank* MeshBank; };
struct CValidateDefinitionInitBase {
    unsigned char       _pad[8];
    CValidateResources* Resources;                          // +0x08
};

struct CAnimationEntry;
struct CAnimFilterBase {                                    // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {               // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
struct CAnimationSet {
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x005DC2E0
};
struct CAppearanceDef {
    virtual void v000();
    virtual void DestroyDef();
    long          m_refCount;                               // +4
    unsigned char _pad08[0x2c];
    CAnimationSet Animation;                                // +0x34
};
template<class T> struct CDefPointer {
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject && --m_pObject->m_refCount == 0)
            m_pObject->DestroyDef();
    }
    T* operator->() const { return m_pObject; }
};
struct CDefClassBase {
    virtual void v000();
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>* out) const { out->m_pObject = 0; return false; }
#pragma auto_inline(on)

struct CThingCreatureDef : CDefClassBase {
    unsigned char _pad04[0xd8 - 0x04];
    long          CheckDummyObject;                         // +0xd8
    unsigned char _padDC[0x100 - 0xdc];
    long          MeshIndex;                                // +0x100
    unsigned char _pad104[0x10d - 0x104];
    unsigned char CreatureType;                             // +0x10d
    virtual void Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const;
};

void CThingCreatureDef::Validate(const CValidateDefinitionInitBase& init, const CCharString&) const
{
    CValidateResources* resources = init.Resources;
    CCharString dummyName(g_CCharString_013bac30);
    CCharString helperName(g_CCharString_013bac34);
    const C3DMeshInfo* mesh = 0;
    if (CreatureType == 5)
        mesh = resources->MeshBank->GetCreatureMeshInfo(MeshIndex);
    else if (CreatureType == 4)
        mesh = resources->MeshBank->GetHeroMeshInfo(MeshIndex);
    CDefPointer<CAppearanceDef> appearance;
    GetSubDef(&appearance);
    CreatureType != 4 || appearance->Animation.HasAnimation(CCharString("DEFAULT"), CAnimFilterNoGroup());
    if (CheckDummyObject)
        !mesh || mesh->GetMeshStats()->HasDummyObject(HashOf(dummyName));
    !mesh || mesh->GetMeshStats()->HasHelperPoint(HashOf(g_CCharString_013bac48));
}
