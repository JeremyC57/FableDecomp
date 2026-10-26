// NEntityEvents::CEventPerformedSpecialAbility::CEventPerformedSpecialAbility(EHeroAbility ability) -- the "hero performed a special ability" event: aggressive unless the ability's CHeroAbilityDef says otherwise
// Leaf: ??0CEventPerformedSpecialAbility@NEntityEvents@@QAE@W4EHeroAbility@@@Z
// Retail: Fable.exe @ 0x008111F0, 213 bytes (+ int3 padding), `ret 4`: __fastcall; vtable 0x012723C0
//   (RTTI .?AVCEventPerformedSpecialAbility@NEntityEvents@@).
// Name: Ego_r.pdb / FableWin.pdb `??0CEventPerformedSpecialAbility@NEntityEvents@@QAE@W4EHeroAbility@@@Z`; Ego_r.pdb layout
//   (16 bytes): CEventBase (vptr, long WorldFrame +4), EHeroAbility Ability +8, bool Aggressive +0xC.
// Label: catalogued `CParticleMorphs_InitFromHeroAbility` (wrong: the vtable store is this event's).
// Callees: CDefinitionManager::Get (static __fastcall) 0x0044C6B0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0 ("HERO_ABILITY" @ 0x01235E14), CDefinitionManager::GetNumberOfDefsOfType 0x009AD3B0, the def lookup
//   0x005F89F2 (stand-in body below), the def's virtual slot 1 (release).
// Shape: the landed sibling CEventHitBySpecialAbility ctor 0x00811830 with the inline CEventBase part (WorldFrame = 0 before
//   the vtable store): block-scoped type-name strings homed in the dead argument slot, the def released without a null
//   test, the search returning on the hit; CHeroAbilityDef Ability +0x3c, Aggressive +0x94 (retail offsets).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefBase {
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CHeroAbilityDef : public CDefBase {
public:
    unsigned char _pad08[0x3c - 0x08];
    long Ability;                                       // +0x3c EHeroAbility
    unsigned char _pad40[0x94 - 0x40];
    bool Aggressive;                                    // +0x94
};
class CDefinitionManager {
public:
    static CDefinitionManager& __fastcall Get();        // 0x0044C6B0
    long GetNumberOfDefsOfType(const CCharString& typeName);   // 0x009AD3B0
    bool GetDef_005f89f2(const CCharString& typeName, long index, CDefPointer<CHeroAbilityDef>& out);   // 0x005F89F2
};
void* FindDef_CDefinitionManager_GetDef_005f89f2_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef_005f89f2(const CCharString& typeName, long index, CDefPointer<CHeroAbilityDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_005f89f2_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
enum EHeroAbility { HERO_ABILITY_NULL = 0 };
namespace NEntityEvents {
class CEventBase {                                      // vtable 0x0124F2C4
public:
    virtual void v00();
    long WorldFrame;                                    // +0x04
    CEventBase() : WorldFrame(0) { }
};
class CEventPerformedSpecialAbility : public CEventBase {   // vtable 0x012723C0
public:
    EHeroAbility Ability;                               // +0x08
    bool Aggressive;                                    // +0x0C
    virtual void v00();
    CEventPerformedSpecialAbility(EHeroAbility ability);
};

CEventPerformedSpecialAbility::CEventPerformedSpecialAbility(EHeroAbility ability)
    : Ability(ability), Aggressive(true)
{
    CDefinitionManager& manager = CDefinitionManager::Get();
    long count;
    {
        CCharString typeName("HERO_ABILITY", -1);
        count = manager.GetNumberOfDefsOfType(typeName);
    }
    for (long i = 1; i < count; ++i) {
        CDefPointer<CHeroAbilityDef> def;
        {
            CCharString typeName("HERO_ABILITY", -1);
            manager.GetDef_005f89f2(typeName, i, def);
        }
        if (def->Ability == Ability) {
            Aggressive = def->Aggressive;
            return;
        }
    }
}
}