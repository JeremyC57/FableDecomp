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
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013BCA24 is CWideString::NullString (?NullString@CWideString@@2V1@A, public static;
//   Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CWideString::NullString'', initializer 0x01223DA0 landed by agentFAM09); was g_EmptyWideString.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING   // fable_string.h: public static CWideString::NullString
#include "engine/CThing.h"

class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

inline bool IsThingAlive(const CThing* thing) { return thing && !thing->IsDead(); }

class CTCQuestCard { public:                           // interface type 0x6C
    CWideString GetQuestName() const;           // 0x00702310
};

class CTCInventoryQuests {
public:
    virtual CWideString GetCategoryNameFromAssociation(CThing* thing) const;
};

CWideString CTCInventoryQuests::GetCategoryNameFromAssociation(CThing* thing) const
{
    const CTCQuestCard* questCard;
    if (IsThingAlive(thing) && thing->PeekTC(TCI_QUEST_CARD, &questCard))
        return questCard->GetQuestName();
    return CWideString::NullString;
}