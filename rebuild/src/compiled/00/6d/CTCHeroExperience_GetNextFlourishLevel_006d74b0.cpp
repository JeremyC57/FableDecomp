// CTCHeroExperience::GetNextFlourishLevel() const -- with flourishes enabled in the hero-combat def, the next multiple of the flourish step above the current experience-multiplier level (level = (long)(param / def divisor + 1.0), at least 1); otherwise the next fallback multiplier
// Leaf: GetNextFlourishLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D74B0, length 146 bytes (+14 int3 padding).
// Ego_r.pdb: ?GetNextFlourishLevel@CTCHeroExperience@@ABEJXZ
// Callees: __ftol2 @ 0x00BFEA70 (the `(long)` of the double), CTCHeroExperience::GetNextExperienceFallbackMultiplier()
// const @ 0x006D7430 (manifest "Combat_GetNextFlourishLevelAsFloat"; tail-jumped), the def's slot 1 on release.
// Shape: the hero-combat def is read through an inline by-value getter, so each use copies the CDefPointer (null-tested AddRef)
// and releases the copy with NO null test (`dec [r+4] ; jne`): one temporary for the enabled test, one named copy for the
// multiplier, one temporary for the divisor. The level adds the double 1.0 (0x01237F00) before the truncation.
// Layout: retail ExperienceMultiplierParam +0x30, PDef +0x54 (CHeroExperienceDef: the level divisor float at +0x64),
// PHeroCombatDef +0x58 (CHeroCombatDef: flourish-enabled byte +0xd0, flourish level step +0xd4).
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CRefCounted {
    long m_count;
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefObject : CDefVirtual, CRefCounted {};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer() { if (DecRefToZero(Object->m_count)) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CHeroExperienceDef : CDefObject {
    unsigned char _pad08[0x64 - 0x08];
    float ExperienceMultiplierParamPerLevel;    // +0x64
};
struct CHeroCombatDef : CDefObject {
    unsigned char _pad08[0xd0 - 0x08];
    bool  FlourishesEnabled;                    // +0xd0
    unsigned char _padd1[3];
    long  FlourishLevelStep;                    // +0xd4
};
class CThing;
class CTCHeroExperience {
public:
    virtual ~CTCHeroExperience();
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x30 - 0x08];
    float         ExperienceMultiplierParam;    // +0x30
    unsigned char _pad34[0x54 - 0x34];
    CDefPointer<CHeroExperienceDef> PDef;       // +0x54
    CDefPointer<CHeroCombatDef>     PHeroCombatDef;   // +0x58
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const { return PHeroCombatDef; }
    long GetNextExperienceFallbackMultiplier() const;     // 0x006D7430
    long GetNextFlourishLevel() const;
};

long CTCHeroExperience::GetNextFlourishLevel() const
{
    if (GetHeroCombatDef()->FlourishesEnabled) {
        CDefPointer<CHeroCombatDef> combatDef = GetHeroCombatDef();
        long level = (long)(ExperienceMultiplierParam / PDef->ExperienceMultiplierParamPerLevel + 1.0);
        if (level < 1)
            level = 1;
        return (level / GetHeroCombatDef()->FlourishLevelStep + 1) * combatDef->FlourishLevelStep;
    }
    return GetNextExperienceFallbackMultiplier();
}