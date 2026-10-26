// CTCActionUseBase::IsThisTheft() const -- using this object is theft when it is owned (interface 0x33) by a living owner who belongs to a village, the owner is a house (0xBD) or a shop (0x53), and the owner is not a buyable house (0xD5) the player owns
// Leaf: IsThisTheft
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EACB0, 275 bytes, `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (the parent's lookup) and non-const
//   0x0040F020 (the owner's lookups), CBaseIntelligentPointer::GetPItem 0x00A01B50 (five times),
//   CTCVillageMember::GetPVillage() const 0x007EE540 (manifest "IS_THING_ALIVE<...>" is propagated).
// Layout: CTCBase {vptr, ParentThing +4}; the 0x33 component (CTCOwnedEntity) holds its owner CIntelligentPointer<CThing>
//   at the odd offset +0x0D (pack(1), as the landed CTCStockItem::GetShopTC 0x007F4FC0); CThing bitset +0x20, dead flag
//   bit 0 of +0x91, interface map +0x44; CTCBuyableHouse::OwnedByPlayer +0x11.
// Shape: the parent lookup is the out-parameter const GetInterface with the owner pointer bound to a reference local
//   (`lea edi,[eax+0Dh]`); the village member is the ternary GetTC used straight as `this`; HasTC is `__forceinline`
//   (plain inline is called out of line). The success path is NESTED so every failure shares the trailing `xor al,al`
//   with the callee-saved pushes at entry (flat early returns shrink-wrap edi); the final `return !(HasTC(0xD5) &&
//   OwnedByPlayer)` materialises `xor eax,eax` / `mov eax,1`.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                 // +0x91 (bit 0: dead)
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:                         // interface 0x33
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer POwner;             // +0x0d
};
#pragma pack(pop)
class CTCVillage;
class CTCVillageMember { public:                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
class CTCBuyableHouse { public:                        // interface 0xD5 (engine/CTCBuyableHouse.h)
    unsigned char _pad00[0x11];
    bool OwnedByPlayer;                         // +0x11
};

class CTCActionUseBase { public:
    void*         __vftable;                    // +0x00 (CTCBase)
    const CThing* ParentThing;                  // +0x04
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    void*         PActionUseDef;                // +0x0c CDefPointer<CActionUseDef const>
    bool          Usable;                       // +0x10
    bool IsThisTheft() const;
};

bool CTCActionUseBase::IsThisTheft() const
{
    CTCOwnedEntity* owned;
    if (ParentThing->GetInterface(&owned, 0x33)) {
        CBaseIntelligentPointer& owner = owned->POwner;
        CThing* thing = owner.GetPItem();
        if (thing && !(thing->m_stateFlags & 1)) {
            thing = owner.GetPItem();
            if (thing->HasTC(0x23) && thing->GetTC<CTCVillageMember>(0x23)->GetPVillage()) {
                if (owner.GetPItem()->HasTC(0xbd) || owner.GetPItem()->HasTC(0x53)) {
                    thing = owner.GetPItem();
                    return !(thing->HasTC(0xd5) && thing->GetTC<CTCBuyableHouse>(0xd5)->OwnedByPlayer);
                }
            }
        }
    }
    return false;
}