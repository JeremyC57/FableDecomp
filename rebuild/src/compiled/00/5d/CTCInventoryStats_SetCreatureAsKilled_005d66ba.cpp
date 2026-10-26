// CTCInventoryStats::SetCreatureAsKilled(CThing* creature) -- counts a kill of the creature's kind in the stats screen's kill table (first kill inserts 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D66BA, length 140 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Ego_r.pdb: ?SetCreatureAsKilled@CTCInventoryStats@@QAEXPAVCThing@@@Z.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (`__fastcall`, reads 0x013B879C), CDefinitionManager::GetDef<CDefClassBase>
// (long, CDefPointer*) @ 0x0042AF3C (manifest names a propagated instantiation), CDefClassBase::GetSubDef<T>(CDefPointer*)
// @ 0x00485815, the kill table's find @ 0x005D83B3 (manifest "CMemoryAllocatorVariableSize_FindNode", propagated) and
// operator[] @ 0x005D8933, the defs' slot 1 on release.
// Shape: the thing's def is a zeroed CDefPointer local ([ebp-4]); the sub-def pointer is a second zeroed local that VC homes in
// the dead `creature` slot ([ebp+8]); both release through a null-tested dtor. The find result is compared with the header
// word directly; the hit increments the node's value (+0x14).
// Layout: CThing def index (unsigned short) +0x8C; retail CTCInventoryStats kill table (STLport map<long,long>, header word
// first) at +0x134; the creature sub-def's kill-category key at +0xAC (retail offsets).
#pragma optimize("y", off)
#pragma optimize("s", on)
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CSubDefPointer {    // the GetSubDef out pointer, released by hand
    T* Object;
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CCreatureKillDef : CDefPointeeBase {
    unsigned char _pad08[0xac - 0x08];
    long KillCategory;                          // +0xac
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CSubDefPointer<CCreatureKillDef>* out);   // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CSubDefPointer<CCreatureKillDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                    // +0x8c
};
struct CKillMapNode {
    long  Colour;
    void* Parent;
    void* Left;
    void* Right;
    long  Key;                                  // +0x10
    long  Count;                                // +0x14
};
struct CKillMap {                               // STLport map<long,long>
    CKillMapNode* Header;                       // +0x00
    long          NodeCount;
    CKillMapNode* find(const long& key);        // 0x005D83B3
    long& operator[](const long& key);          // 0x005D8933
};

class CTCInventoryStats { public:
    unsigned char _pad00[0x134];
    CKillMap KilledCreatures;                   // +0x134
    void SetCreatureAsKilled(CThing* creature);
};

void CTCInventoryStats::SetCreatureAsKilled(CThing* creature)
{
    CDefPointer<CDefClassBase> def;
    CGameDefinitionManager::Get().GetDef(creature->DefIndex, &def);
    CSubDefPointer<CCreatureKillDef> killDef;
    killDef.Object = 0;
    def->GetSubDef(&killDef);
    CCreatureKillDef* creatureDef = killDef.Object;
    const long& category = creatureDef->KillCategory;
    CKillMapNode* node = KilledCreatures.find(category);
    if (node == KilledCreatures.Header)
        KilledCreatures[category] = 1;
    else
        ++node->Count;
    if (creatureDef && DecRefToZero(creatureDef->m_refCount))
        creatureDef->Destroy();
}