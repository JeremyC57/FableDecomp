// CTCWallMount::UnattachObject(long* boughtForAmount) -- take the trophy off the mount: hand back (and zero) the price paid; for a live trophy run its wall-mount effects' taken-off hook, make it targetable again and drop the live hero's mounted-trophy count; un-persist and un-flag the mount's thing; clear the trophy pointer and return the trophy
// Leaf: ?UnattachObject@CTCWallMount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DF130, length 370 bytes (`ret 4`, no frame pointer; Ego_r.pdb
//   `?UnattachObject@CTCWallMount@@QAEPAVCThing@@PAJ@Z`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CBaseIntelligentPointer() 0x00A01B10 (then the
//   CIntelligentPointer<CThing> vtable 0x01238C6C), SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10;
//   CVectorMap LowerBound 0x0040F020 (inlined GetTC: interfaces 0xDB, 9, 4); CTCWallMountEffects::OnTakenOffMount
//   0x007DF440 (landed); CTCBase::GetWorld() 0x00686A70 (manifest "CViewBase::GetDisplayEngine" propagated);
//   CWorld::GetCurrentPlayer 0x0049D9D0 (manifest "_Cons_val<...>" propagated); CThing::SetAsGameStatePersisted(bool)
//   0x004C7880.
// Layout (Ego_r.pdb, offsets hold, as the landed OnInitialActivate 0x007DE5D0): CTCBase ParentThing +0x04;
//   CTCWallMount BoughtForAmount +0x0c, Trophy +0x10; CThing bitset +0x20 (0xDB = bit 27 of word +0x38, 9 = bit 9 of
//   word +0x20, 4 = bit 4 of word +0x20), interface map +0x44, dead bit +0x91 bit 0, the +0x92 flag byte (bit 4 cleared
//   here; descriptive); CTCTargeted::Targetable +0x10; CPlayer PlayerCharacter +0x34 (PDB); the interface-4
//   component's +0xd4 mounted-trophy count (descriptive).
// Shape: speed TU (inline IS_THING_ALIVE fetches the trophy twice); the effects and hero presence tests fetch the thing
//   once for the bitset and the lookup (HasTC + GetTC); the Targetable store goes through the out-parameter GetInterface
//   (`T* p; if (thing->GetInterface(&p, 9)) p->X = v;` -- the landed 0x007504E0 shape: the component lands in eax, the
//   HasTC/GetTC spelling colours it ecx and rotates the hero lookup's key lea); the hero is tested null / dead through
//   one fetch; `Trophy = 0` goes through the inline operator= with a converting temporary, after the result is fetched.
class CThing;
class CTCBase;
class CBaseObject;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];           // +0x20 interface-present bitset
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad_0x4c[0x45];
    unsigned char   m_flags91;                  // +0x91 bit 0: dead
    unsigned char   m_flags92;                  // +0x92
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for T = CThing
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem(item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCWallMountEffects { public:                    // interface 0xDB
    void OnTakenOffMount();                     // 0x007DF440
};
class CTCTargeted { public:                            // interface 9
    unsigned char _pad_0x00[0x10];
    bool          Targetable;                   // +0x10
};
struct CTCHeroTrophies {                        // interface 4 (descriptive)
    unsigned char _pad_0x00[0xd4];
    long          MountedTrophies;              // +0xd4
};

class CPlayer { public:
    unsigned char                       _pad_0x00[0x34];
    CIntelligentPointer<CThing>         PlayerCharacter;    // +0x34 CIntelligentPointer<CThingPlayerCreature>
    CThing* GetPlayerCharacter() const { return ((CThing*)PlayerCharacter.GetPItem()); }
};
class CWorld { public:
    CPlayer* GetCurrentPlayer();                // 0x0049D9D0
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCWallMount : public CTCBase { public:
    long                        BoughtForAmount;   // +0x0c
    CIntelligentPointer<CThing> Trophy;            // +0x10
    unsigned __int64            TrophyID;          // +0x18
    CThing* UnattachObject(long* boughtForAmount);
};

CThing* CTCWallMount::UnattachObject(long* boughtForAmount)
{
    if (boughtForAmount)
        *boughtForAmount = BoughtForAmount;
    BoughtForAmount = 0;
    if (IS_THING_ALIVE(Trophy)) {
        CThing* trophy = ((CThing*)Trophy.GetPItem());
        if (trophy->HasTC(0xdb))
            ((CTCWallMountEffects*)trophy->GetTC(0xdb))->OnTakenOffMount();
        CTCTargeted* targeted;
        if (((CThing*)Trophy.GetPItem())->GetInterface(&targeted, 9))
            targeted->Targetable = true;
        CThing* hero = GetWorld().GetCurrentPlayer()->GetPlayerCharacter();
        if (IS_THING_ALIVE(hero) && hero->HasTC(4))
            --((CTCHeroTrophies*)hero->GetTC(4))->MountedTrophies;
    }
    ParentThing->SetAsGameStatePersisted(false);
    ParentThing->m_flags92 &= ~0x10;
    CThing* trophy = ((CThing*)Trophy.GetPItem());
    Trophy = CIntelligentPointer<CThing>(0);
    return trophy;
}