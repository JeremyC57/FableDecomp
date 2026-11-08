// CTCWallMount::UnattachObject(long* boughtForAmount) -- take the trophy off the mount: hand back (and zero) the price paid; for a live trophy run its wall-mount effects' taken-off hook, make it targetable again and drop the live hero's mounted-trophy count; un-persist and un-flag the mount's thing; clear the trophy pointer and return the trophy
// Leaf: ?UnattachObject@CTCWallMount@@QAEPAVCThing@@PAJ@Z
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DF130, length 370 bytes (`ret 4`, no frame pointer; Ego_r.pdb
//   `?UnattachObject@CTCWallMount@@QAEPAVCThing@@PAJ@Z`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CBaseIntelligentPointer() 0x00A01B10 (then the
//   CIntelligentPointer<CThing> vtable 0x01238C6C), SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10;
//   CVectorMap LowerBound 0x0040F020 (inlined GetTC: interfaces 0xDB, 9, 4); CTCWallMountEffects::OnTakenOffMount
//   0x007DF440 (landed); CTCBase::GetWorld() 0x00686A70 (manifest "CViewBase::GetDisplayEngine" propagated);
//   CWorld::PeekMyPlayer 0x0049D9D0 (manifest "_Cons_val<...>" propagated); CThing::SetAsGameStatePersisted(bool)
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
#include "fable_compiler.h"
class CThing;
class CTCBase;
class CBaseObject;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];           // +0x20 interface-present bitset
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad_0x54[0x3d];
    unsigned char   m_flags91;                  // +0x91 bit 0: dead
    unsigned char   m_flags92;                  // +0x92
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        return (it == map->end() || interfaceType < it->first ? map->end() : it)->second;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
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
    void SetPItem(CBaseObject*a0);                // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for T = CThing
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
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
class CWorld {
public:
    CPlayer const* PeekMyPlayer() const;   // 0x0049D9D0 (Ego_r.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
private:                 // 0x0049D9D0
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
        CThing* hero = ((CPlayer*)((CWorld*)&(GetWorld()))->PeekMyPlayer())->GetPlayerCharacter();
        if (IS_THING_ALIVE(hero) && hero->HasTC(4))
            --((CTCHeroTrophies*)hero->GetTC(4))->MountedTrophies;
    }
    ParentThing->SetAsGameStatePersisted(false);
    ParentThing->m_flags92 &= ~0x10;
    CThing* trophy = ((CThing*)Trophy.GetPItem());
    Trophy = CIntelligentPointer<CThing>(0);
    return trophy;
}