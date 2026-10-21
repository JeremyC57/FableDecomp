// NPlayerGui::CDrawMeleeActionButtonEffects::Update() -- pick the hero's current melee ability def (the flourish ability when a flourish is valid, else the base ability when the combat component says so), restart the flash timers when it changes, clear the effects when there is none, and count the timers down
// Grade: DIFFER(338v373) -- the two CDefPointer temporaries share one frame slot and VC merges the two Adopt arms into one tail; retail keeps three frame words (ability, flourish temp, hero-param/key/base temp) and a register zero (ebx) for the local init
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064BE45, length 373 bytes (frame pointer, `ret`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetControlledCreature @ 0x00487DD0, IS_THING_ALIVE @ 0x0043DBF0
// (`__fastcall`, by reference), std::bitset::test @ 0x00410DE0, the interface map LowerBound const @ 0x004365B0, CTCCombat::
// IsMeleeFlourishValid @ 0x00733EE0, ::GetPMeleeFlourishAbility @ 0x00733480 (CDefPointer by hidden pointer), CTCCombat @ 0x00733290
// (a bool query; the manifest label "CThingFilter_IsUnconscious::operator()" is propagated), CThing::GetPBaseDef @ 0x007334B0
// (CDefPointer by hidden pointer; landed), ClearEffects @ 0x006445BA (thiscall on the Effects vector), the def's virtual slot 1 on release.
// Layout: Ego_r.pdb CDrawMeleeActionButtonEffects minus 4 (the 16-byte PDB vector is 12 in retail): PlayerManager +0x08, Effects +0x28,
// FlashTimer +0x34, ComboLoadingIconTimer +0x38, FlourishCounter +0x3c, EffectCount +0x40, PCurrentAbility +0x44 (names of the last two
// descriptive). CTCCombat = interface 0x30.
// Shape: frame-pointer size TU; the hero stays in edi: it reaches IS_THING_ALIVE through an inline helper's by-value parameter (the frame word the lookup key and the second temporary later reuse);
// the chosen def is a CDefPointer local (null at entry, released at exit) filled from each getter's full-expression temporary through an AddRef copy (the temporaries are released at the end of their statements);
// the member CDefPointer assignment is the inline operator= (release the old, adopt, AddRef).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CRefCounted {
    long m_count;
    void AddRef() { ++m_count; }
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CAbilityDef : CDefVirtual, CRefCounted {
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
    void Adopt(const CDefPointer& other)
    {
        if (other.Object) {
            other.Object->AddRef();
            Object = other.Object;
        }
    }
    void Assign(T* object)
    {
        if (Object != object) {
            if (Object)
                Object->Release();
            Object = 0;
            Object = object;
            if (object)
                object->AddRef();
        }
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) const { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUncheckedConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    CDefPointer<CAbilityDef> GetPBaseDef() const;  // 0x007334B0
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
void* FindRef_free_IS_THING_ALIVE_2();
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    void* p = FindRef_free_IS_THING_ALIVE_2();
    if (!p)
        return false;
    *(void**)&thing = p;
    return false;
}
#pragma auto_inline(on)
struct CPlayer {
    CThing* GetControlledCreature();               // 0x00487DD0
};
static __forceinline bool IsAlive(CThing* thing)   // the by-value parameter is the frame word IS_THING_ALIVE sees
{
    return IS_THING_ALIVE(thing);
}
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct CTCCombat {
    bool IsMeleeFlourishValid();                   // 0x00733EE0
    CDefPointer<CAbilityDef> GetPMeleeFlourishAbility();   // 0x00733480
    bool IsShowingBaseAbility();                   // 0x00733290 (label unverified)
};
struct CEffectVector {
    void ClearEffects();                           // 0x006445BA
};
struct CDrawMeleeActionButtonEffects {
    unsigned char            _pad0[0x08];
    CPlayerManager*          PlayerManager;        // +0x08
    unsigned char            _pad1[0x28 - 0x0c];
    CEffectVector            Effects;              // +0x28
    unsigned char            _pad2[0x34 - 0x28 - sizeof(CEffectVector)];
    long                     FlashTimer;           // +0x34
    long                     ComboLoadingIconTimer;// +0x38
    long                     FlourishCounter;      // +0x3c
    long                     EffectCount;          // +0x40
    CDefPointer<CAbilityDef> PCurrentAbility;      // +0x44
    void Update();
};

void CDrawMeleeActionButtonEffects::Update()
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    CDefPointer<CAbilityDef> ability;
    if (IsAlive(hero)) {
        if (hero->HasInterface(0x30)) {
            CTCCombat* combat = hero->GetInterfaceUncheckedConst<CTCCombat>(0x30);
            if (combat->IsMeleeFlourishValid()) {
                ability.Adopt(combat->GetPMeleeFlourishAbility());
            }
            else if (combat->IsShowingBaseAbility()) {
                ability.Adopt(hero->GetPBaseDef());
            }
            if (ability.Object != PCurrentAbility.Object) {
                ComboLoadingIconTimer = -1;
                FlashTimer = 15;
            }
        }
    }
    PCurrentAbility.Assign(ability.Object);
    if (FlashTimer == 0)
        ComboLoadingIconTimer = 30;
    if (!ability.Object) {
        Effects.ClearEffects();
        FlourishCounter = -1;
        EffectCount = 0;
        ComboLoadingIconTimer = -1;
        FlashTimer = -1;
    }
    if (ComboLoadingIconTimer > 0)
        --ComboLoadingIconTimer;
    if (FlashTimer > 0)
        --FlashTimer;
}
