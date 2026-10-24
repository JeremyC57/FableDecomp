// CCreatureAction_SheatheItemToInventory::GetAnimationToUse(const CThing& creature) -- the sheathe animation of the live weapon in the creature's primary carry slot, else "STANDARD_SHEATHE"
// Leaf: GetAnimationToUse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F3E0, length 156 bytes (+ int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CTCCarrying::GetThingInPrimarySlot() const @ 0x006A1440 (landed), CTCWeapon::GetSheatheAnim() const @ 0x005DA5A0,
//   CCharString::CCharString(const CCharString&) @ 0x0099EC30, CCharString::CCharString(const char*, long) @ 0x0099EBF0.
// Identity: Ego_r.pdb `?GetAnimationToUse@CCreatureAction_SheatheItemToInventory@@CI?AVCCharString@@ABVCThing@@@Z`
//   (private static __fastcall: the hidden CCharString result in ecx, the creature in edx); the literal
//   0x0125D014 is "STANDARD_SHEATHE".
// Layout (retail, local model as the landed CTCCarrying::IsThingCarrying* rows 0x006A1540..0x006A1620): component
//   bitset at +0x20 (carrying 0x46, weapon 0x48), {kind, component} vector map at +0x44, Dead bit +0x91 bit 0.
// Shape: the IsThingCarryingMeleeWeapon body (PeekTC<T> with the long->ETCInterfaceType conversion temporary as
//   the one shared key word) returning `weapon->GetSheatheAnim()` through the copy constructor into the hidden
//   result; every miss falls to the one `CCharString("STANDARD_SHEATHE")` construction (`push -1 ; push offset`).
class CTCBase;
enum ETCInterfaceType { TC_CARRYING = 0x46, TC_WEAPON = 0x48 };

class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
    __forceinline SEntry* Find(long type) const
    {
        SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->type) ? last : it;
    }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap Components;                       // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                         // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool PeekTC(const T** pp) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            *pp = (const T*)Components.Find(T::StatGetInterfaceType())->component;
            return true;
        }
        return false;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    if (thing && !thing->Dead)
        return true;
    return false;
}

class CTCWeapon {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_WEAPON; }
    const CCharString& GetSheatheAnim() const;      // 0x005DA5A0
};

class CTCCarrying {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_CARRYING; }
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440
};

class CCreatureAction_SheatheItemToInventory {
    static CCharString __fastcall GetAnimationToUse(const CThing& creature);
};

CCharString __fastcall CCreatureAction_SheatheItemToInventory::GetAnimationToUse(const CThing& creature)
{
    const CTCCarrying* carrying;
    if (creature.PeekTC(&carrying)) {
        CThing* item = carrying->GetThingInPrimarySlot();
        const CTCWeapon* weapon;
        if (IsThingAlive(item) && item->PeekTC(&weapon))
            return weapon->GetSheatheAnim();
    }
    return CCharString("STANDARD_SHEATHE");
}