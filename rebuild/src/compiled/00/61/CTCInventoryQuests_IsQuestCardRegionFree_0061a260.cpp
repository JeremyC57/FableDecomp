// CTCInventoryQuests::IsQuestCardRegionFree(CTCQuestCard* card) -- true unless an active quest is already started in the card's region
// Retail 0x0061A260, 84 bytes (+12 int3), ret 4. Only caller: CTCInventoryQuests::TakeQuest
// (0x006222AA), which pushes the thing's type-108 component (a CTCQuestCard) -- `this` is never
// set up, so this is a static `__stdcall` helper; the manifest label
// "CTCInventoryItem::GetStartingRightHandedSet" is propagated and wrong (name here is descriptive).
// Shape: the card's PDef (CDefPointer<const CQuestCardDef>, +0x0C) is copied into a local (add-ref
// under a null test); the def's RegionName string id (+0x38, Ego_r.pdb) is turned into a CCharString
// by CDefStringTable::GetString 0x009D49B0 on the global table 0x013CA828 -- the temporary is homed in
// the dead `card` argument slot -- and handed to CQuestManager::IsActiveQuestStartedInRegion
// 0x004B1270 on the global manager pointer 0x013B89FC; the bool rides in bl across ~CCharString
// 0x0099EAE0 and the def release (vtable slot 1, no null test), then `test bl,bl ; sete al`.
// Source form that reproduces it: the def copy and the string in an inner scope (the negation after
// it -- a `return !started` inside the scope is evaluated before the release), the string as a named
// local (a bare temporary reuses GetString's returned eax instead of `lea eax,[esp+0Ch]`), and an
// `unsigned char` result (a bool return adds `xor eax,eax` before the sete).
// Leaf: IsQuestCardRegionFree
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).

class CCharString { public:
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                // 0x0099EAE0
};

class CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)           // 0x013CA828

#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:
    bool IsActiveQuestStartedInRegion(const CCharString& region);  // 0x004B1270
};

class CQuestCardDef { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long refCount;                                 // +0x04
    unsigned char _pad_08[0x30];
    long RegionName;                               // +0x38 CDefString
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};

struct CQuestCardDefPointer {                      // CDefPointer<const CQuestCardDef>
    CQuestCardDef* Object;
    CQuestCardDefPointer(const CQuestCardDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CQuestCardDefPointer() { Object->Release(); }
    CQuestCardDef* operator->() const { return Object; }
};

class CThing;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};

class CTCQuestCard : public CTCBase { public:
    CQuestCardDefPointer PDef;                     // +0x0C
};

class CTCInventoryQuests { public:
    static unsigned char __stdcall IsQuestCardRegionFree(CTCQuestCard* card);
};

unsigned char __stdcall CTCInventoryQuests::IsQuestCardRegionFree(CTCQuestCard* card)
{
    bool started;
    {
        CQuestCardDefPointer def(card->PDef);
        CCharString region = GDefStringTable.GetString(def->RegionName);
        started = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsActiveQuestStartedInRegion(region);
    }
    return !started;
}