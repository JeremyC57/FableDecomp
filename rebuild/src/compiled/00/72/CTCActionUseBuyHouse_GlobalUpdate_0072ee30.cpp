// CTCActionUseBuyHouse::GlobalUpdate(long) -- while waiting for the buy confirmation, take the most recent answer message (type 0x10); if this sign still belongs to a living buyable house, confirm the purchase on a yes, stop waiting, and after the buy question go on to the rent question once the player owns the house; the message is consumed
// Leaf: GlobalUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072EE30, 255 bytes (+17 int3 padding), `ret 4` (the argument is never read);
//   CTCActionUseBuyHouse vtable 0x01266A7C slot 26.
// Callees: CTCBase::GetWorld() 0x00686A70, CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType>
//   0x004AB8B0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCActionUseBuyHouse::ReceivedConfirmation(CTCBuyableHouse*) 0x0072EB40 / OnActionUseRent(CTCBuyableHouse*)
//   0x0072E810, CMessageEventManager::RemoveMessage(const CMessageEvent*) 0x00493B5B.
// Layout: as the OnActionUse sibling 0x0072ED10 (WaitingForConfirmation +0x18, HouseQuestion +0x1c); CWorld
//   PMessageEventManager +0x60 (landed CTCHeroPodium::FrameUpdate); the message's answer word +0x3c; the buyable house's
//   OwnedByPlayer +0x11 (engine/CTCBuyableHouse.h).
// Shape: the message manager is a named local spilled to the frame for the final RemoveMessage; the filter is a
//   temporary with a constructor; the owner lookup is the out-parameter GetInterface with a reference to the owner
//   pointer; WaitingForConfirmation is cleared between the HouseQuestion compare and its jump (statement order).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
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
class CTCBuyableHouse { public:                        // interface 0xD5
    unsigned char _pad00[0x10];
    bool IsScripted;                            // +0x10
    bool OwnedByPlayer;                         // +0x11
};
class CMessageEvent { public:
    unsigned char _pad00[0x3c];
    long m_answer;                              // +0x3c (1 = yes)
};
class CMessageEventFilter_IsOfType { public:
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) {}
};
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
class CWorld { public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;  // +0x60
};
class CBuyHouseDef;
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

class CTCActionUseBase {
public:
    void*   __vftable;                          // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    void*   PActionUseDef;                      // +0x0c
    bool    Usable;                             // +0x10
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
};
enum EHouseQuestion { HOUSE_QUESTION_NONE = 0, HOUSE_QUESTION_BUY = 1, HOUSE_QUESTION_RENT = 2 };
class CTCActionUseBuyHouse : public CTCActionUseBase {
public:
    CDefPointer<const CBuyHouseDef> PDef;       // +0x14
    bool           WaitingForConfirmation;      // +0x18
    EHouseQuestion HouseQuestion;               // +0x1c
    void ReceivedConfirmation(CTCBuyableHouse* house);   // 0x0072EB40
    void OnActionUseRent(CTCBuyableHouse* house);        // 0x0072E810
    void GlobalUpdate(long frame);
};

void CTCActionUseBuyHouse::GlobalUpdate(long frame)
{
    if (WaitingForConfirmation) {
        CMessageEventManager* messages = GetWorld()->PMessageEventManager;
        CMessageEvent* answer = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x10));
        if (answer) {
            CTCOwnedEntity* owned;
            if (ParentThing->GetInterface(&owned, 0x33)) {
                CBaseIntelligentPointer& owner = owned->POwner;
                CThing* thing = owner.GetPItem();
                if (thing && !(thing->m_stateFlags & 1)) {
                    thing = owner.GetPItem();
                    if (thing->HasTC(0xd5)) {
                        CTCBuyableHouse* house = thing->GetTC<CTCBuyableHouse>(0xd5);
                        if (answer->m_answer == 1)
                            ReceivedConfirmation(house);
                        WaitingForConfirmation = false;
                        if (HouseQuestion == HOUSE_QUESTION_BUY && house->OwnedByPlayer)
                            OnActionUseRent(house);
                    }
                }
            }
            messages->RemoveMessage(answer);
        }
    }
}