// CTCCombat::OnCreate() -- cache the parent creature; for a creature thing, load the hero combat def's default melee abilities into the ability list (when it has a hero combat def) and look up the block-counter and 360-flourish abilities by name; then fetch the creature's combat type def and, when found, set it and take its starts-ready-for-melee flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007363A0, length 482 bytes.
// Callees: CThingCreatureBase::GetPHeroCombatDef @ 0x00663340 (CDefPointer by value), CTCBase::GetDefinitionManager @ 0x006869D0,
//          CDefinitionManager::GetDef(long, CDefPointer<CMeleeCombatAbilityDef>*) @ 0x00737F80 / GetDef(const CCharString&,
//          CDefPointer&) @ 0x00737FF0 / GetDef(long, CDefPointer<CCombatTypeDef>&) @ 0x00669640 (same-TU out-only stand-ins),
//          vector<CDefPointer>::_M_insert_overflow @ 0x00739BC0 (stand-in), CThingCreatureBase::GetCombatTypeIndex @ 0x00662170,
//          CTCCombat::SetCombatType @ 0x007355F0 (stand-in), CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0, the defs' virtual slot 1.
// Shape: retry A's v15 (out-only stand-ins for every address-taking callee, by-value GetPHeroCombatDef with its temporary released
//          by hand through the inline dtor) plus a named `long index = *it;` for the loop lookup: the value load then takes eax and
//          the out slot's lea ecx (`mov eax,[ebp] ; lea ecx,[esp+14h] ; push ecx ; push eax`), where `GetDef(*it, &ability)` colours
//          them the other way round. A by-reference out parameter does not change it; a forceinline (out, key) wrapper also matches.
// Leaf: OnCreate
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
    T* operator->() const { return m_p; }
    bool IsNull() const { return m_p == 0; }
};

inline void* operator new(unsigned int, void* where) throw() { return where; }
struct __false_type { };
template <class T> struct CDefPointerVector {           // std::vector<CDefPointer<T> >
    CDefPointer<T>* _M_start;
    CDefPointer<T>* _M_finish;
    CDefPointer<T>* _M_end_of_storage;
    void _M_insert_overflow(CDefPointer<T>* pos, const CDefPointer<T>& x, const __false_type& pod, unsigned int fillLen,
                            bool atEnd);                // 0x00739BC0
    void push_back(const CDefPointer<T>& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CDefPointer<T>(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    const long* begin() const { return _M_start; }
    const long* end() const { return _M_finish; }
};
class CHeroCombatDef : public CDefBase { public:
    unsigned char _pad08[0x11c - 0x08];
    CLongVector   DefaultCombatAbilities;               // +0x11c
};
class CCombatTypeDef : public CDefBase { public:
    unsigned char _pad08[0xa8 - 0x08];
    bool          StartsAsReadyForMelee;                // +0xa8
};
class CMeleeCombatAbilityDef : public CDefBase { public: };

class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CMeleeCombatAbilityDef>* out) const;     // 0x00737F80
    bool GetDef(const CCharString& name, CDefPointer<CMeleeCombatAbilityDef>& out);   // 0x00737FF0
    bool GetDef(long index, CDefPointer<CCombatTypeDef>& out);                   // 0x00669640
};

void* FindDef_CDefinitionManager_GetDef();   // stand-in callee of the stand-in bodies below
// Same-TU stand-in bodies (the real GetDef instantiations are out of line in retail, in this TU): each writes the out
// pointer only, so VC7.1 knows the def pointer's address does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CMeleeCombatAbilityDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<CMeleeCombatAbilityDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CCombatTypeDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                           // +0x10 (2: creature)
};
class CThingCreatureBase : public CThing { public:
    long GetCombatTypeIndex() const;                    // 0x00662170
    CDefPointer<CHeroCombatDef> GetPHeroCombatDef() const;  // 0x00663340
};

class CTCCombat { public:
    void*               __vftable;                      // +0x00
    CThing*             ParentThing;                    // +0x04 (CTCBase)
    unsigned char       _pad08[0x24 - 0x08];
    CThingCreatureBase* ParentCreature;                 // +0x24
    CDefPointer<CCombatTypeDef> PCombatTypeDef;         // +0x28
    CDefPointerVector<CMeleeCombatAbilityDef> AbilityList;   // +0x2c
    unsigned char       _pad38[0x48 - 0x38];
    CDefPointer<CMeleeCombatAbilityDef> PBlockCounterAbility;   // +0x48
    CDefPointer<CMeleeCombatAbilityDef> PFlourish360Ability;    // +0x4c
    unsigned char       _pad50[0x80 - 0x50];
    bool                ReadyForMelee;                  // +0x80
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    const CThingCreatureBase& GetParentCreature() const { return *ParentCreature; }
    CDefPointer<CHeroCombatDef> GetPHeroCombatDef() const { return GetParentCreature().GetPHeroCombatDef(); }
    void SetCombatType(const CDefPointer<CCombatTypeDef>& def);   // 0x007355F0
    void OnCreate();
};

void* FindRef_CTCCombat();   // stand-in callee of the stand-in bodies below
// Same-TU stand-ins for the vector growth and SetCombatType (both out of line in this TU in retail): they read through
// their reference parameters only.
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CDefPointerVector<T>::_M_insert_overflow(CDefPointer<T>* pos,
    const CDefPointer<T>& x, const __false_type& pod, unsigned int fillLen, bool atEnd)
{
    void* p = FindRef_CTCCombat();
    if (!p)
        return;
    *(void**)pos = p;
    if (*(void**)&x != p)
        return;
}
__declspec(noinline) void CTCCombat::SetCombatType(const CDefPointer<CCombatTypeDef>& def)
{
    if (def.m_p == FindRef_CTCCombat())
        ParentCreature = 0;
}
#pragma auto_inline(on)

void CTCCombat::OnCreate()
{
    ParentCreature = (CThingCreatureBase*)ParentThing;
    if (ParentThing->ThingClass == 2) {
        if (!GetPHeroCombatDef().IsNull()) {
            for (const long* it = GetPHeroCombatDef()->DefaultCombatAbilities.begin();
                 it != GetPHeroCombatDef()->DefaultCombatAbilities.end(); ++it) {
                CDefPointer<CMeleeCombatAbilityDef> ability;
                long index = *it;
                GetDefinitionManager()->GetDef(index, &ability);
                AbilityList.push_back(ability);
            }
        }
        GetDefinitionManager()->GetDef("HERO_MELEE_COMBAT_ABILITY_BLOCK_COUNTER", PBlockCounterAbility);
        GetDefinitionManager()->GetDef("HERO_MELEE_COMBAT_ABILITY_FLOURISH_360_STRIKE", PFlourish360Ability);
    }
    CDefPointer<CCombatTypeDef> combatType;
    long combatTypeIndex = GetParentCreature().GetCombatTypeIndex();
    if (GetDefinitionManager()->GetDef(combatTypeIndex, combatType)) {
        SetCombatType(combatType);
        ReadyForMelee = combatType->StartsAsReadyForMelee;
    }
}