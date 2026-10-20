// Grade: DIFFER(279v260) -- VC7.1 CSEs the twice-used "HERO_ABILITY" literal into ebx and keeps this in esi / i in ebx; retail pushes the immediate twice, keeps this in edi, i in a frame word, and pushes ebx only inside the loop branch (sunk callee-saved push)
// CTCInventoryAbilities::InitialiseAbilities() -- binds every HERO_ABILITY def to its ability slot and clears the experience spent on it, then sizes the received-ability order list from the UI misc-things def
// Retail 0x005EEDE2, 260 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCBase::GetDefinitionManager 0x006869D0
// (landed as GetDefinitions / GetDefinitionManager), CDefinitionManager::GetNumberOfDefsOfType(const CCharString&)
// 0x009AD3B0, GetDef(const CCharString&, long, CDefPointer<CHeroAbilityDef>*) 0x005F89F2, GetUIMiscThingsDef()
// 0x005EE3B3 (CDefPointer by value; manifest "CCreatureAction_CastStrengthMultiStrikeProper" is propagated),
// vector<EHeroAbility>::resize(n, const EHeroAbility&) 0x005F9DF8 (manifest "CArray<pair<long,long>>::push_back"),
// the defs' slot 1 on release.
// Shape: both type names convert implicitly (built first, re-addressed with `lea`); the count lives in a block that
// closes before the misc-things def, so its frame word is reused by the resize value temporary, and the type-name
// temporary's word is reused by the loop index. Each loop pass binds a fresh def pointer (inline zeroing ctor) into
// Abilities[def->Ability] through the inline operator= (old def released and zeroed, the new one add-ref'd), and the
// def pointers release without a null test.
// Layout (retail; Ego_r.pdb names): ExperienceSpentOnAbilities vector<long> +0x148, ReceivedAbilityOrder +0x154,
// Abilities vector<pair<long, CDefPointer>> +0x160; CHeroAbilityDef::Ability +0x3C; CUIMiscThingsDef::
// TotalSpellsInPalettes +0x148 (PDB +0x14C).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };
struct CHeroAbilityDef : CDefPointeeBase {
    unsigned char _pad_0x08[0x3c - 0x08];
    EHeroAbility Ability;                              // +0x3c
};
struct CUIMiscThingsDef : CDefPointeeBase {
    unsigned char _pad_0x08[0x148 - 0x08];
    unsigned long TotalSpellsInPalettes;               // +0x148
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { Object->Release(); }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            Object->AddRef();
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    long GetNumberOfDefsOfType(const CCharString& typeName);                                       // 0x009AD3B0
    bool GetDef(const CCharString& typeName, long index, CDefPointer<CHeroAbilityDef>* out);       // 0x005F89F2
};
struct SAbilityEntry {                                 // pair<long, CDefPointer<const CHeroAbilityDef>>
    long Level;                                        // +0x00
    CDefPointer<CHeroAbilityDef> PDef;                 // +0x04
};
template <class T> struct CVector {                    // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](long n) { return *(begin() + n); }
};
struct CAbilityOrderVector : CVector<EHeroAbility> {
    void resize(unsigned long n, const EHeroAbility& value);   // 0x005F9DF8
};

struct CTCInventoryAbilities {
    unsigned char _pad_0x000[0x148];
    CVector<long> ExperienceSpentOnAbilities;          // +0x148
    CAbilityOrderVector ReceivedAbilityOrder;          // +0x154
    CVector<SAbilityEntry> Abilities;                  // +0x160
    CDefinitionManager* GetDefinitionManager() const;  // 0x006869D0 (CTCBase)
    CDefPointer<CUIMiscThingsDef> GetUIMiscThingsDef();   // 0x005EE3B3
    void InitialiseAbilities();
};

void CTCInventoryAbilities::InitialiseAbilities()
{
    long count = GetDefinitionManager()->GetNumberOfDefsOfType("HERO_ABILITY");
    for (long i = 1; i < count; ++i) {
        CDefPointer<CHeroAbilityDef> def;
        GetDefinitionManager()->GetDef("HERO_ABILITY", i, &def);
        Abilities[def->Ability].PDef = def;
        ExperienceSpentOnAbilities[i] = 0;
    }
    CDefPointer<CUIMiscThingsDef> misc = GetUIMiscThingsDef();
    ReceivedAbilityOrder.resize(misc->TotalSpellsInPalettes, ABILITY_NULL);
}
