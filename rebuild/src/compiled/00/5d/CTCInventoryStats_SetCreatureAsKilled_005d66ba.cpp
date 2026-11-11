// CTCInventoryStats::SetCreatureAsKilled(CThing* creature) -- counts a kill of the creature's kind in the stats screen's kill table (first kill inserts 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D66BA, length 140 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Ego_r.pdb: ?SetCreatureAsKilled@CTCInventoryStats@@QAEXPAVCThing@@@Z.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (`__fastcall`, reads 0x013B879C), CDefinitionManager::GetDef<CThingBaseDef>
// (long, CDefPointer<const CThingBaseDef>*) const @ 0x0042AF3C, CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const ..>*) const
// @ 0x00485815 (its StatGetTypeName literal 0x01231D3C is "CCreatureDef"), the kill table's find @ 0x005D83B3 (manifest "CMemoryAllocatorVariableSize_FindNode", propagated) and
// operator[] @ 0x005D8933, the defs' slot 1 on release.
// Shape: the thing's def is a zeroed CDefPointer local ([ebp-4]); the sub-def pointer is a second zeroed local that VC homes in
// the dead `creature` slot ([ebp+8]); both are released by hand with a null test (the sub-def first, then the thing's def). The find result is compared with the header
// word directly; the hit increments the node's value (+0x14).
// Layout: CThing def index (unsigned short) +0x8C; retail CTCInventoryStats kill table (STLport map<long,long>, header word
// first) at +0x134; the creature sub-def's CreatureGroup key at +0xAC (retail offsets).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
static FABLE_FORCEINLINE bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:   // the def lookups' out slot (released by hand at its scope end)
    CDefPointeeBase* Object;                    // the pointee as its non-const base (engine/CDefPointer.h)
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00485815 GetSubDef<CCreatureDef> (Ego_r / FableWin template)
};
class CThingBaseDef : public CDefClassBase { public: };
class CCreatureDef : public CDefClassBase { public:
    unsigned char _pad08[0xac - 0x08];
    unsigned long CreatureGroup;                // +0xac (PDB +0xBC: four 12-byte STLport containers before it)
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    template <class T> bool GetDef(long globalIndex, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef> (FableWin.pdb template)
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<const T>* out) const
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
    CKillMapNode* find(const unsigned long& key);   // 0x005D83B3
    long& operator[](const unsigned long& key);     // 0x005D8933
};

class CTCInventoryStats { public:
    unsigned char _pad00[0x134];
    CKillMap KilledCreatures;                   // +0x134
    void SetCreatureAsKilled(CThing* creature);
};

void CTCInventoryStats::SetCreatureAsKilled(CThing* creature)
{
    CDefPointer<const CThingBaseDef> def;
    def.Object = 0;
    CGameDefinitionManager::Get().GetDef(creature->DefIndex, &def);
    CDefPointer<const CCreatureDef> creatureDefPointer;
    creatureDefPointer.Object = 0;
    def->GetSubDef(&creatureDefPointer);
    CCreatureDef* creatureDef = static_cast<CCreatureDef*>(creatureDefPointer.Object);
    const unsigned long& category = creatureDef->CreatureGroup;
    CKillMapNode* node = KilledCreatures.find(category);
    if (node == KilledCreatures.Header)
        KilledCreatures[category] = 1;
    else
        ++node->Count;
    if (creatureDef && DecRefToZero(creatureDef->m_refCount))
        creatureDef->Destroy();
    CDefPointeeBase* thingDef = def.Object;
    if (thingDef && DecRefToZero(thingDef->m_refCount))
        thingDef->Destroy();
}