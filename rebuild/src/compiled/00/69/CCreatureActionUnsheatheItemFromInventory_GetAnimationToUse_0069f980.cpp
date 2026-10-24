// CCreatureAction_UnsheatheItemFromInventory::GetAnimationToUse(const CThing& creature, const CThing& item) -- the item's weapon unsheathe animation, else "STANDARD_UNSHEATHE"
// Leaf: GetAnimationToUse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F980, length 96 bytes, `ret 4`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CTCWeapon::GetUnsheatheAnim() const @ 0x005DA540 (manifest label "GetSheatheAnim" is propagated; the
//   sheathe sibling 0x0069F3E0 calls 0x005DA5A0), CCharString(const CCharString&) @ 0x0099EC30,
//   CCharString(const char*, long) @ 0x0099EBF0.
// Identity: Ego_r.pdb `?GetAnimationToUse@CCreatureAction_UnsheatheItemFromInventory@@CI?AVCCharString@@ABVCThing@@0@Z`
//   (private static __fastcall: hidden result in ecx, the unused creature in edx, the item on the stack -> `ret 4`);
//   the literal 0x0125D028 is "STANDARD_UNSHEATHE".
// Layout: the local CThing model of the sheathe sibling (weapon component 0x48 = word +0x28 bit 8, map at +0x44).
// Shape: PeekTC<CTCWeapon> on the item (key in the dead item-argument slot: no frame), the hit returns the
//   weapon's unsheathe CCharString through the copy constructor, the miss builds the default literal.
class CTCBase;
enum ETCInterfaceType { TC_WEAPON = 0x48 };

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

class CTCWeapon {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_WEAPON; }
    const CCharString& GetUnsheatheAnim() const;    // 0x005DA540
};

class CCreatureAction_UnsheatheItemFromInventory {
    static CCharString __fastcall GetAnimationToUse(const CThing& creature, const CThing& item);
};

CCharString __fastcall CCreatureAction_UnsheatheItemFromInventory::GetAnimationToUse(const CThing& creature, const CThing& item)
{
    const CTCWeapon* weapon;
    if (item.PeekTC(&weapon))
        return weapon->GetUnsheatheAnim();
    return CCharString("STANDARD_UNSHEATHE");
}