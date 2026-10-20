// NEntityEvents::CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters& params, long ability) -- a hit event for a hero special ability: aggressive unless the ability's CHeroAbilityDef says otherwise
// Leaf: ??0CEventHitBySpecialAbility
// Grade: DIFFER(233v211) -- dead-argument-slot homing of both strings and the def matches; the lane loads the dereferenced def into eax and emits the long `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test` release where retail keeps ecx with `dec [ecx+4] ; jne` (DecRef/GetRefCount, nulling dtor and pointer-local forms tried), plus the unaligned loop head
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811830, 209 bytes (`ret 8`); vtable 0x012723F0
// NEntityEvents::CEventHitBySpecialAbility.
// Label: the catalog says CTCSinglePersonOccupiable::IsOfType (propagated); the vtable store is CEventHitBySpecialAbility's
// (rtti-vtables.tsv) and the base is the landed CEventHitBase constructor.
// Callees: NEntityEvents::CEventHitBase::CEventHitBase(const CHitParameters&) @ 0x00811760, CDefinitionManager::Get (static,
// __fastcall) @ 0x0044C6B0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("HERO_ABILITY" @ 0x01235E14),
// CDefinitionManager::GetNumberOfDefsOfType(const CCharString&) @ 0x009AD3B0, CDefinitionManager::GetDef_005f89f2(const
// CCharString&, long, CDefPointer&) @ 0x005F89F2 (the landed body names its first parameter `long kind`: it is the type-name
// string's address); the def's virtual slot 1.
// Layout: CEventHitBase (WorldFrame +0x04, PHitParams +0x08) then Ability +0x10, Aggressive +0x14; CHeroAbilityDef (retail def
// at PDB - 4) Ability +0x3c, Aggressive +0x94.
// Shape: both type-name strings and the def out pointer live in the constructor's dead argument slots (VC homes the
// block-scoped locals there); the def is released without a null test (dereferenced first); the search returns on the hit.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CHeroAbilityDef : CDefBase {
    unsigned char _pad08[0x3c - 0x08];
    long Ability;                                       // +0x3c EHeroAbility
    unsigned char _pad40[0x94 - 0x40];
    bool Aggressive;                                    // +0x94
};
struct CDefinitionManager {
    static CDefinitionManager& __fastcall Get();        // 0x0044C6B0
    long GetNumberOfDefsOfType(const CCharString& typeName);   // 0x009AD3B0
    bool GetDef_005f89f2(const CCharString& typeName, long index, CDefPointer<CHeroAbilityDef>& out);   // 0x005F89F2
};
struct CHitParameters;
struct CCPPointerInfo;
namespace NEntityEvents {
struct CEventHitBase {
    virtual void v00();
    long WorldFrame;                                    // +0x04
    CHitParameters* PHitParams_Data;                    // +0x08 CCountedPointer<CHitParameters>
    CCPPointerInfo* PHitParams_Info;                    // +0x0c
    CEventHitBase(const CHitParameters& params);        // 0x00811760
};
struct CEventHitBySpecialAbility : CEventHitBase {      // vtable 0x012723F0
    long Ability;                                       // +0x10 EHeroAbility
    bool Aggressive;                                    // +0x14
    virtual void v00();
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);
};

CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters& params, long ability)
    : CEventHitBase(params), Ability(ability), Aggressive(true)
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
