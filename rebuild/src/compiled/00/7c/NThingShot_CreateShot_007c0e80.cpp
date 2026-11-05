// NThingShot::CreateShot(CThing& shot, CThing* shooter, CThing* weapon, const CThing* target, float, float, float, const CDefPointer<const CMeleeCombatAbilityDef>& ability, long shot_def_index) -- turn an existing thing into a shot: the shot def (the given index, else the weapon's created-shot def; the shooter stands in for a missing weapon) names the shot-definition TC to add, which is Init'ed with the def, shooter, weapon, target, the three floats and the ability; returns the shot (0 when shots are off, there is no shooter/weapon or no shot def)
// Leaf: ?CreateShot@NThingShot@@YIPAVCThing@@AAV2@PAV2@1PBV2@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@J@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000); the local same-TU LowerBound body removed (the header defines the same-TU noinline body); the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C0E80, 297 bytes, __fastcall (ecx shot, edx shooter), `ret 1Ch` (seven stack arguments).
// Identity (proven): Ego_r.pdb ?CreateShot@NThingShot@@YIPAVCThing@@AAV2@PAV2@1PBV2@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@J@Z
//   (donor 0x007BF690), the overload of the def-index CreateShot (donor 0x007BF280) that takes the thing: 2 register + 7 stack
//   arguments = `ret 1Ch`, and it forwards exactly the Init overload taking (CThing*, CThing*, const CThing*). Callers 0x00871618,
//   0x008C8B91, 0x008D5CD9.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0, CTCWeapon::GetCreatedShotDefGlobalIndex() const
//   0x005DA750, CMainGameComponent::PeekDefinitionManager() const 0x00415FBC (jmp thunk), CDefinitionManager::GetDef<CThingShotDef>
//   (long, CDefPointer<const CThingShotDef>*) const 0x0078B230, CDefStringTable::GetString(long) const 0x009D49B0 on
//   GDefStringTable 0x013CA828 (CDefString's inline operator CCharString), CThing::AddTC(const CCharString&, bool, const
//   CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0, NThingShot::CTCShotDefinitionBase::Init(CDefPointer<const CThingShotDef>,
//   CThing*, CThing*, const CThing*, float, float, float, const CDefPointer<const CMeleeCombatAbilityDef>&) 0x007C0690; inside
//   GetDef: CDefinitionManager::GetPDefFromGlobalIndex(long) const 0x009AD9E0.
//   Data: CMainGameComponent::Get() 0x013B86A0, the shots-enabled switch 0x0137573F (a bool read by both CreateShot overloads;
//   descriptive name).
// Layout: retail CThing UsingComponents +0x20 / Components +0x44 (interface 0x48 CTCWeapon); CThingShotDef ShotDefinitionClassName
//   (CDefString) +0x60 (Ego_r +0x64); the def lock count is CIVCountedPointeeBase::IVRefCount +0x04 (Ego_r.pdb; AddRef / Release / virtual Delete slot 1).
// Shape: GetDef<T> is a same-TU noinline body (header template code); the weapon-component pointer is an uninitialised local filled under a named HasTC bool (both homed in the dead weapon
//   argument slot); the TC name is the AddTC full-expression temporary; the by-value def copy locks in the argument area; the
//   valid-index work is the then-block with the final `return 0` after it (the shared null exit sits at the end).
class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };

#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

// Same-TU body of the real retail const LowerBound instance (binary search over the sorted pairs), not inlined: it lets the
// inlined lookup share its key frame word with the dead weapon argument slot, as retail does.
#pragma auto_inline(off)
#pragma auto_inline(on)

class CCharString;
class CTCParamsBase;
class CThing {                                          // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];
    std::bitset<274>   UsingComponents;                 // +0x20
    TCMap              Components;                      // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      Dead : 1;                        // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> __forceinline const T* PeekTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const TCMap* map = &Components;
        const TCMap::value_type* it = map->LowerBound(key);
        return (const T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
};
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CDefStringTable {
public:
    CCharString GetString(long table_pos) const;        // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)                // 0x013CA828
class CDefString {
public:
    long TablePos;
    operator CCharString() const { return GDefStringTable.GetString(TablePos); }
};

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                        // slot 1 (vtable 0x01230BCC slot 1 = 0x0042D930)
    unsigned long IVRefCount;                           // +0x04 (Ego_r.pdb)
    void AddRef() const { ++const_cast<CIVCountedPointeeBase*>(this)->IVRefCount; }
    long Release() const
    {
        CIVCountedPointeeBase* self = const_cast<CIVCountedPointeeBase*>(this);
        if (--self->IVRefCount == 0) {
            Delete();
            return 0;
        }
        return (long)self->IVRefCount;
    }
};
class CResource : public CIVCountedPointeeBase {
public:
    unsigned char _body[0x1c - 0x08];                   // ResourceList, PrevResource, NextResource, ResourceSize, LastUsedFrame
};
class CDefPointeeBase : public CResource { };
enum ENullDef { NULL_DEF = 0 };
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                            // Ego_r.pdb
    CDefPointer(ENullDef) : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    const T* Get() const { return static_cast<const T*>(Object); }
    const T* operator->() const { return Get(); }
    bool IsNull() const { return Object == 0; }
    void Reset(const T* object)
    {
        if (Object != object) {
            if (Object)
                Object->Release();
            Object = 0;
            Object = (CDefPointeeBase*)object;
            if (Object)
                Object->AddRef();
        }
    }
    template <class TOther> void SetDynamicCast(const CDefPointer<TOther>& other) { Reset(static_cast<const T*>(other.Get())); }
};
class CDefClassBase : public CDefPointeeBase { };
class CThingShotDef : public CDefClassBase {
public:
    unsigned char _pad1c[0x60 - 0x1c];
    CDefString    ShotDefinitionClassName;              // +0x60
};
class CMeleeCombatAbilityDef;

class CDefinitionManager {
public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long global_index) const;   // 0x009AD9E0
    template <class T> bool GetDef(long global_index, CDefPointer<const T>* def) const;   // 0x0078B230 (T = CThingShotDef)
};
// CDefinitionManager::GetDef<T> (0x0078B230): the header template body, same TU and not inlined (retail calls it): with the
// body in view the shot def's frame word does not escape, so it is read once into esi and the word is reused.
#pragma auto_inline(off)
template <class T>
__declspec(noinline) bool CDefinitionManager::GetDef(long global_index, CDefPointer<const T>* def) const
{
    if (global_index > 0) {
        CDefPointer<const CDefClassBase> base = GetPDefFromGlobalIndex(global_index);
        if (!base.IsNull()) {
            def->SetDynamicCast(base);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { };
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private:
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC
};
extern bool g_bShotsEnabled;                            // 0x0137573F (descriptive)

class CTCWeapon {
public:
    long GetCreatedShotDefGlobalIndex() const;          // 0x005DA750
};
namespace NThingShot {
class CTCShotDefinitionBase {
public:
    void Init(CDefPointer<const CThingShotDef> def, CThing* shooter, CThing* weapon, const CThing* target, float a, float b,
              float c, const CDefPointer<const CMeleeCombatAbilityDef>& ability);   // 0x007C0690
};

CThing* __fastcall CreateShot(CThing& shot, CThing* shooter, CThing* weapon, const CThing* target, float a, float b, float c,
                              const CDefPointer<const CMeleeCombatAbilityDef>& ability, long shot_def_index)
{
    if (!g_bShotsEnabled)
        return 0;
    if (weapon == 0) {
        weapon = shooter;
        if (weapon == 0)
            return 0;
    }
    const CTCWeapon* weapon_tc;
    bool has_weapon_tc = weapon->HasTC((ETCInterfaceType)0x48);
    if (has_weapon_tc)
        weapon_tc = weapon->PeekTC<CTCWeapon>((ETCInterfaceType)0x48);
    long def_index = shot_def_index > 0 ? shot_def_index : weapon_tc->GetCreatedShotDefGlobalIndex();
    if (def_index >= 0) {
        CDefPointer<const CThingShotDef> shot_def(NULL_DEF);
        CMainGameComponent::Get()->PeekDefinitionManager().GetDef<CThingShotDef>(def_index, &shot_def);
        CTCShotDefinitionBase* shot_tc =
            (CTCShotDefinitionBase*)shot.AddTC(shot_def->ShotDefinitionClassName, true, 0);
        shot_tc->Init(shot_def, shooter, weapon, target, a, b, c, ability);
        return &shot;
    }
    return 0;
}
}   // namespace NThingShot