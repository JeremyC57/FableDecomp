// NPlayerGui::CDrawMeleeActionButtonEffects::Update() -- pick the hero's current melee ability def (the flourish ability when a flourish is valid, else the base ability when the combat component says so), restart the flash timers when it changes, clear the effects when there is none, and count the timers down
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064BE45, length 373 bytes (frame pointer, `ret`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::PeekPControlledCreature @ 0x00487DD0, IS_THING_ALIVE @ 0x0043DBF0
// (`__fastcall`, by reference), std::bitset::test @ 0x00410DE0, the interface map LowerBound const @ 0x004365B0, CTCCombat::
// IsMeleeFlourishValid @ 0x00733EE0, ::GetPMeleeFlourishAbility @ 0x00733480 (CDefPointer by hidden pointer), CTCCombat @ 0x00733290
// (a bool query; the manifest label "CThingFilter_IsUnconscious::operator()" is propagated), the base-ability getter @ 0x007334B0
// (thiscall on the CTCCombat component, CDefPointer by hidden pointer), ClearEffects @ 0x006445BA (thiscall on the Effects vector), the def's virtual slot 1 on release.
// Layout: Ego_r.pdb CDrawMeleeActionButtonEffects minus 4 (the 16-byte PDB vector is 12 in retail): PlayerManager +0x08, Effects +0x28,
// FlashTimer +0x34, ComboLoadingIconTimer +0x38, FlourishCounter +0x3c, EffectCount +0x40, PCurrentAbility +0x44 (names of the last two
// descriptive). CTCCombat = interface 0x30.
// Shape: frame-pointer size TU; IS_THING_ALIVE (out of line in retail) has a same-TU read-only stand-in body so the hero's frame word
// does not escape and is reused for the lookup key and the second temporary; the component comes from the out-param GetInterface
// (the type constant 0x30 is held in ebx); both getters are called on the CTCCombat component; the chosen def is a CDefPointer local
// filled from each getter's full-expression temporary through an AddRef copy; CDefPointer release and assignment clear the pointer
// only inside the non-null arm (`if (p) { p->Release(); p = 0; }`).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
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
class CAbilityDef : public CDefVirtual, public CRefCounted { public:
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    void Adopt(const CDefPointer& other)
    {
        T* object = other.Object;
        if (object) {
            object->AddRef();
            Object = object;
        }
    }
    __forceinline void Assign(T* object)
    {
        if (Object != object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = object;
            if (object)
                object->AddRef();
        }
    }
};
class CThing { public:
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long type) const
    {
        if (m_present.test(type)) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingAliveImpl(const void* thing);          // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;               // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCCombat { public:
    bool IsMeleeFlourishValid();                   // 0x00733EE0
    CDefPointer<CAbilityDef> GetPMeleeFlourishAbility();   // 0x00733480
    bool IsShowingBaseAbility();                   // 0x00733290 (label unverified)
    CDefPointer<CAbilityDef> GetPBaseAbility();    // 0x007334B0
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
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    CDefPointer<CAbilityDef> ability;
    if (IS_THING_ALIVE(hero)) {
        CTCCombat* combat;
        if (hero->GetInterface(&combat, 0x30)) {
            if (combat->IsMeleeFlourishValid()) {
                ability.Adopt(combat->GetPMeleeFlourishAbility());
            }
            else if (combat->IsShowingBaseAbility()) {
                ability.Adopt(combat->GetPBaseAbility());
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