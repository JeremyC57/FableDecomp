// CTCHeroOpinionDeedLog::SetVillageForCrimes(CThing& village) -- remember the village the hero's crimes are reported to (an intelligent pointer at +0x20)
// Leaf: ?SetVillageForCrimes@CTCHeroOpinionDeedLog@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C55E0, 71 bytes, thiscall `ret 4`.
// Identity (sweep3; was the generated _unattributed::AssignTarget_006c55e0 / CClass_006c55e0): Ego_r.pdb / FableWin.pdb
//   ?SetVillageForCrimes@CTCHeroOpinionDeedLog@@QAEXAAVCThing@@@Z; the body assigns VillageForCrimes (Ego_r +0x2C, retail +0x20).
// Layout (retail, from Ego_r CTCHeroOpinionDeedLog with STLport containers): CTCBase 0xC, OpinionDeedMap +0x0C (12-byte map),
//   OpinionDeedsSearchingForWitnesses +0x18 (4-byte list), NextHandle +0x1C, VillageForCrimes +0x20 (CIntelligentPointer<CThing>,
//   8 bytes), WeaponOutDeedHandle +0x28, WeaponOutCrimeEnabled +0x2C, GuardsIgnoreCrimes +0x2D, DeedEffectsDefIndices
//   (ushort[92]) +0x2E (Ego_r +0x3A).
// Order: Ego_r.pdb CTCHeroOpinionDeedLog SetVillageForCrimes 0x006FCB00, PeekPDeedEffectsDef 0x006FCB50, OnInitialActivate
//   0x006FCB80 = retail 0x006C55E0 (stores into +0x20), 0x006C5630 (reads the +0x2E ushort table), OnInitialActivate 0x006C5680.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, SetPItem(CBaseObject*) 0x00A01B90 (on the
//   temporary, then on the member), GetPItem() const 0x00A01B50, ~CBaseIntelligentPointer() 0x00A01C10; the temporary's
//   vtable is CIntelligentPointer<CThing> 0x01238C6C.
// Shape: `VillageForCrimes = &village;` -- a CIntelligentPointer<CThing> temporary built from the thing, then operator=
//   (SetPItem(other.GetPItem())), then the temporary destroyed.
class CBaseObject {
public:
    virtual ~CBaseObject();
};
class CThing : public CBaseObject { };
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
    CBaseObjectPointer* PData;                         // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CTCHeroOpinionDeedLog {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];                 // CTCBase, OpinionDeedMap, OpinionDeedsSearchingForWitnesses, NextHandle
    CIntelligentPointer<CThing> VillageForCrimes;      // +0x20
    void SetVillageForCrimes(CThing& village);
};

void CTCHeroOpinionDeedLog::SetVillageForCrimes(CThing& village)
{
    VillageForCrimes = &village;
}
