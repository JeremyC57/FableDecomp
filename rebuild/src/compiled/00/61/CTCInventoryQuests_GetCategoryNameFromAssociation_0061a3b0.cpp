// CTCInventoryQuests::GetCategoryNameFromAssociation(CThing* thing) const -- the quest name of a live quest-card thing, else the empty wide string
// Leaf: GetCategoryNameFromAssociation
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A3B0, length 106 bytes (+ int3 padding), `ret 8` (hidden CWideString result + thing).
// Ego_r.pdb: ?GetCategoryNameFromAssociation@CTCInventoryQuests@@UBE?AVCWideString@@PAVCThing@@@Z (the catalog's "vfunc_76").
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the inline CThing::GetInterface for interface 0x6C, the quest card: bit 12 of
// the bitset dword at +0x2C, `test ch,10h`), CTCQuestCard::GetQuestName() const @ 0x00702310 (CWideString by hidden pointer),
// CWideString::CWideString(const CWideString&) @ 0x0099B720 from the empty-string global 0x013BCA24.
// Shape: the inline IsThingAlive test (null, dead bit 0 of +0x91) and the bitset test share the empty-string arm; the
// GetInterface key is homed in the dead `thing` slot.
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);      // 0x0099B720
};
extern const CWideString g_EmptyWideString;     // 0x013BCA24

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                     // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
};
inline bool IsThingAlive(const CThing* thing) { return thing && !thing->Dead; }

class CTCQuestCard { public:                           // interface type 0x6C
    CWideString GetQuestName() const;           // 0x00702310
};

class CTCInventoryQuests {
public:
    virtual CWideString GetCategoryNameFromAssociation(CThing* thing) const;
};

CWideString CTCInventoryQuests::GetCategoryNameFromAssociation(CThing* thing) const
{
    CTCQuestCard* questCard;
    if (IsThingAlive(thing) && thing->GetInterface(&questCard, 0x6C))
        return questCard->GetQuestName();
    return g_EmptyWideString;
}