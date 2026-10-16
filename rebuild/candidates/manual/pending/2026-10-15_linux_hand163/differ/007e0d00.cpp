// CTCEffectOnDie::OnDie() -- unless paused, play the death sound criteria through the parent's sound component and create the death effect thing at the parent's position
// Grade: DIFFER(112v112) -- retail loads the sound component into ecx (mov ecx,[eax+4]) BEFORE the five pushes and forms the criteria address in edx; ours loads it after the pushes and uses ecx for the lea; tried GetInterface()->PlaySound, a named component local, a GetTC(thing,k) map Find helper, an inline PlaySound wrapper member
// Leaf: OnDie
//
// Retail 0x007E0D00, 112 bytes (plain `ret`; vtable 0x0126FEB4 slot 13, Ego_r.pdb
// `?OnDie@CTCEffectOnDie@@UAEXXZ`; the catalog "CTCEffectOnDie::vfunc_13" is the slot name).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for
// the sound component, interface 16), the sound component's play method @ 0x0066BD00 (here with the
// criteria string, 1, 0, 0, -1), CThing::GetPosition() const @ 0x004C73D0, the global Create(long,
// const C3DVector&, bool) @ 0x006E0880 (`__fastcall`, the bool pushed first; result unused).
// Layout = Ego_r.pdb CTCEffectOnDie (0x24): Effect +0x10, DeathSoundCriteria +0x1c, Paused +0x21.
// Shape: the component is a named local taken right after the lookup (`mov ecx,[eax+4]` before the
// pushes; used straight in the call it is loaded after them); the effect is created only when its index is non-zero.
struct CTCBase;
struct C3DVector;

struct CCharString {
    void* PStringData;
};

struct CTCSound                                             // the thing's interface-16 component
{
    unsigned long PlaySound(const CCharString& criteria, long a, long b, long c, long d);   // 0x0066BD00
};

struct CInterfaceMap                                        // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

CThing* __fastcall Create(long defIndex, const C3DVector& position, bool flag);   // 0x006E0880

struct CTCEffectOnDie
{
    virtual void v00();
    CThing*     ParentThing;                                // +0x04 (CTCBase)
    bool        ToBeRemoved;                                // +0x08
    bool        IsInGlobalUpdateTCs;                        // +0x09
    unsigned char _pad0a[2];
    bool        StartedNonRecoverableFadeout;               // +0x0c
    long        Effect;                                     // +0x10
    long        Timer;                                      // +0x14
    long        FadeOutTimer;                               // +0x18
    CCharString DeathSoundCriteria;                         // +0x1c
    bool        Pausable;                                   // +0x20
    bool        Paused;                                     // +0x21
    void OnDie();
};

void CTCEffectOnDie::OnDie()
{
    if (!Paused)
    {
        if (ParentThing->HasInterface(16))
        {
            CTCSound* sound = (CTCSound*)ParentThing->GetInterface(16);
            sound->PlaySound(DeathSoundCriteria, 1, 0, 0, -1);
        }
        if (Effect != 0)
            Create(Effect, ParentThing->GetPosition(), false);
    }
}
