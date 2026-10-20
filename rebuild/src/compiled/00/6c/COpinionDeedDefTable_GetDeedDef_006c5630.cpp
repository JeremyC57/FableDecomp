// COpinionDeedDefTable::GetDeedDef(long deedType) -- the def registered for a deed type, fetched from the definition manager through the owner thing and returned as a counted pointer
// Leaf: GetDeedDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5630, 78 bytes (+ int3 padding).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0 (`mov ecx,[ecx+4] ; jmp CThing::GetDefinitionManager`: the owner
//          thing at +4), CDefinitionManager::GetDef<...> @ 0x006C76B0, the def's Destroy (vtable slot 1).
// Identity: a component (owner thing at +4) holding a table of unsigned-short def indices at +0x2e, reached through
//   interface type 0x51 by CTCHeroStats::UpdateOutstandingCrimes (0x0057DA68) and called from CTCVillage,
//   CTCCreatureOpinionOfHero and CTCHeroOpinionDeedLog deed code (44 call sites); it sits just before
//   CTCHeroOpinionDeedLog's methods. The class is not resolved: the name is descriptive. The manifest label
//   "CThingBuilding::GetFloorMaterial" is propagated.
// Shape: as 0x0083C1E0 -- a CDefPointer local filled by the manager, returned by value through the hidden pointer
//   (`ret 8`: hidden result + the deed type).
struct CDefPointeeBase {                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
};

struct COpinionDeedTypeDef : CDefPointeeBase { };

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};

struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<COpinionDeedTypeDef>* out);   // 0x006C76B0
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<COpinionDeedTypeDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct COpinionDeedDefTable {
    unsigned char _pad00[0x2e];
    unsigned short DeedDefIndices[1];   // +0x2e, indexed by deed type
    CDefinitionManager* GetDefinitionManager();     // 0x006869D0 (CTCBase: through the owner thing)
    CDefPointer<COpinionDeedTypeDef> GetDeedDef(long deedType);
};

CDefPointer<COpinionDeedTypeDef> COpinionDeedDefTable::GetDeedDef(long deedType)
{
    CDefPointer<COpinionDeedTypeDef> def;
    GetDefinitionManager()->GetDef(DeedDefIndices[deedType], &def);
    return def;
}