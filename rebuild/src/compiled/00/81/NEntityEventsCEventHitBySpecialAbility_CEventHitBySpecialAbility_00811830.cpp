// NEntityEvents::CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters& params, EHeroAbility ability) -- a hit event for a hero special ability: aggressive unless the ability's CHeroAbilityDef says otherwise
// Leaf: ??0CEventHitBySpecialAbility@NEntityEvents@@QAE@ABVCHitParameters@@W4EHeroAbility@@@Z
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ??0CEventHitBySpecialAbility@NEntityEvents@@QAE@ABVCHitParameters@@W4EHeroAbility@@@Z
//   (the ability is the EHeroAbility enum, engine/EHeroAbility.h); it was spelled with a long.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811830, 209 bytes (`ret 8`); vtable 0x012723F0
// NEntityEvents::CEventHitBySpecialAbility.
// Label: the catalog says CTCSinglePersonOccupiable::IsOfType (propagated); the vtable store is CEventHitBySpecialAbility's
// (rtti-vtables.tsv) and the base is the landed CEventHitBase constructor.
// Callees: NEntityEvents::CEventHitBase::CEventHitBase(const CHitParameters&) @ 0x00811760, CDefinitionManager::Get (static,
// __fastcall) @ 0x0044C6B0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("HERO_ABILITY" @ 0x01235E14),
// CDefinitionManager::GetNoInstantiatedDefsOfType(const CCharString&) @ 0x009AD3B0, CDefinitionManager::GetDef<CHeroAbilityDef>(const
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
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
};
#include "engine/EHeroAbility.h"
class CHeroAbilityDef : public CDefBase { public:
    unsigned char _pad08[0x3c - 0x08];
    EHeroAbility Ability;                               // +0x3c
    unsigned char _pad40[0x94 - 0x40];
    bool Aggressive;                                    // +0x94
};
class CDefinitionManager { public:
    long GetNoInstantiatedDefsOfType(const CCharString& typeName) const;   // 0x009AD3B0 (Ego_r.pdb / FableWin.pdb)
    template <class T> bool GetDef(const CCharString& typeName, long index, CDefPointer<const T>* out) const;   // 0x005F89F2 GetDef<CHeroAbilityDef> (FableWin.pdb / Ego_r.pdb)
};
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CHeroAbilityDef>(const CCharString& typeName, long index, CDefPointer<const CHeroAbilityDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CHitParameters;
class CCPPointerInfo;
namespace NEntityEvents {
class CEventHitBase { public:
    virtual void v00();
    long WorldFrame;                                    // +0x04
    CHitParameters* PHitParams_Data;                    // +0x08 CCountedPointer<CHitParameters>
    CCPPointerInfo* PHitParams_Info;                    // +0x0c
    CEventHitBase(const CHitParameters& params);        // 0x00811760
};
class CEventHitBySpecialAbility : public CEventHitBase { public:      // vtable 0x012723F0
    EHeroAbility Ability;                               // +0x10
    bool Aggressive;                                    // +0x14
    virtual void v00();
    CEventHitBySpecialAbility(const CHitParameters& params, EHeroAbility ability);
};

CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters& params, EHeroAbility ability)
    : CEventHitBase(params), Ability(ability), Aggressive(true)
{
    CDefinitionManager& manager = CGameDefinitionManager::Get();
    long count;
    {
        CCharString typeName("HERO_ABILITY", -1);
        count = manager.GetNoInstantiatedDefsOfType(typeName);
    }
    for (long i = 1; i < count; ++i) {
        CDefPointer<const CHeroAbilityDef> def;
        {
            CCharString typeName("HERO_ABILITY", -1);
            manager.GetDef(typeName, i, &def);
        }
        if (def->Ability == Ability) {
            Aggressive = def->Aggressive;
            return;
        }
    }
}
}