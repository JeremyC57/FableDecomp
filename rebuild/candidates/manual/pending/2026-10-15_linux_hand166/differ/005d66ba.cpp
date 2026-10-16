// CTCInventoryStats::SetCreatureAsKilled(CThing* creature) -- counts a kill of the creature's kind in the stats screen's kill table (first kill inserts 1)
// Grade: DIFFER(151v142) -- (1) the sub-def object is loaded into eax then copied to ebx after the lea (retail loads ebx directly); (2) the def dtor re-reads [ebp-4] after `dec` (retail: `mov ecx,[ebp-4] ; test ; dec [ecx+4] ; jne` on one load) (tried: plain zeroed CDefPointer for the sub-def, hand release through a local, dtor through a pointer local (gives `add eax,4 ; dec [eax] ; cmp [eax],0`), static Release helper)
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
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CSubDefPointer {    // the GetSubDef out pointer, released by hand
    T* Object;
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CCreatureKillDef : CDefPointeeBase {
    unsigned char _pad08[0xac - 0x08];
    long KillCategory;                          // +0xac
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CSubDefPointer<CCreatureKillDef>* out);   // 0x00485815
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CThing {
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

struct CTCInventoryStats {
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
    if (creatureDef && --creatureDef->m_refCount == 0)
        creatureDef->Destroy();
}
