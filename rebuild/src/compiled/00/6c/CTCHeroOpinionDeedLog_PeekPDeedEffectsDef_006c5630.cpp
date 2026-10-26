// CTCHeroOpinionDeedLog::PeekPDeedEffectsDef(EOpinionDeedType type) const -- the effects def of a deed type: its global index from the ushort table DeedEffectsDefIndices, fetched from the definition manager into a counted pointer returned by value
// Leaf: ?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5630, 78 bytes, thiscall `ret 8` (hidden result + the deed type).
// Identity (sweep3; was the descriptive COpinionDeedDefTable::GetDeedDef, earlier labels CDeedLog::GetEffectsDef /
//   CTCHeroOpinionDeedLog::GetDeedEffectsDef (FINDINGS chunk 478) -- no PDB has GetDeedEffectsDef): Ego_r.pdb / FableWin.pdb
//   ?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z; the
//   word table at +0x2E is DeedEffectsDefIndices; the callee 0x006C76B0 is GetDef<COpinionDeedEffectsDef> (sweep3 rename).
//   44 call sites (CTCHeroStats::UpdateOutstandingCrimes through TC interface 0x51, CTCVillage, CTCCreatureOpinionOfHero, the
//   deed log itself).
// Layout (retail, from Ego_r CTCHeroOpinionDeedLog with STLport containers): CTCBase 0xC, OpinionDeedMap +0x0C (12-byte map),
//   OpinionDeedsSearchingForWitnesses +0x18 (4-byte list), NextHandle +0x1C, VillageForCrimes +0x20, WeaponOutDeedHandle +0x28,
//   WeaponOutCrimeEnabled +0x2C, GuardsIgnoreCrimes +0x2D, DeedEffectsDefIndices (ushort[92]) +0x2E (Ego_r +0x3A).
// Order: Ego_r SetVillageForCrimes 0x006FCB00, PeekPDeedEffectsDef 0x006FCB50, OnInitialActivate 0x006FCB80 = retail
//   0x006C55E0, 0x006C5630, OnInitialActivate 0x006C5680.
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0, CDefinitionManager::GetDef<COpinionDeedEffectsDef> 0x006C76B0 (a same-TU stand-in body, see below),
//   the def's Destroy (vtable slot 1) when the local copy's release drops the last reference.
class CDefBase {                                       // counted def object: vptr, reference count at +4
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1
    mutable long RefCount;                             // +0x04
};
class COpinionDeedEffectsDef : public CDefBase { };

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
};

class CDefinitionManager {
public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x006C76B0 (T = COpinionDeedEffectsDef)
};
class CGameDefinitionManager : public CDefinitionManager { };
// Same-TU stand-in body for the out-of-line GetDef<COpinionDeedEffectsDef> (as the landed row had): it writes the out slot
// only inline, so VC7.1 knows the local def pointer's address does not escape the call (the landed 0x00848730 idiom).
// The stand-in is not graded; its call binds 0x006C76B0 by the explicit specialisation's symbol.
void* FindDef_006c5630();
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<COpinionDeedEffectsDef>(long index, CDefPointer<const COpinionDeedEffectsDef>* out) const
{
    void* def = FindDef_006c5630();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CTCBase {
public:
    virtual ~CTCBase();
protected:
    void* ParentThing;                                 // +0x04
    bool  ToBeRemoved;                                 // +0x08
    bool  IsInGlobalUpdateTCs;                         // +0x09
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

enum EOpinionDeedType { };

class CTCHeroOpinionDeedLog : public CTCBase {
public:
    unsigned char  _pad0c[0x2E - 0x0C];                // OpinionDeedMap .. GuardsIgnoreCrimes
    unsigned short DeedEffectsDefIndices[92];          // +0x2E
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;
};

CDefPointer<const COpinionDeedEffectsDef> CTCHeroOpinionDeedLog::PeekPDeedEffectsDef(EOpinionDeedType type) const
{
    CDefPointer<const COpinionDeedEffectsDef> def;
    GetDefinitionManager().GetDef(DeedEffectsDefIndices[type], &def);
    return def;
}
