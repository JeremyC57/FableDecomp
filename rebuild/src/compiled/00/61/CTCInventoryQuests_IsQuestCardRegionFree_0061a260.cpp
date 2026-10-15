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

struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                // 0x0099EAE0
};

struct CDefStringTable {
    CCharString GetString(long id);                // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;           // 0x013CA828

struct CQuestManager {
    bool IsActiveQuestStartedInRegion(const CCharString& region);  // 0x004B1270
};
extern CQuestManager* g_pQuestManager;             // 0x013B89FC

struct CQuestCardDef {
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

struct CThing;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};

struct CTCQuestCard : CTCBase {
    CQuestCardDefPointer PDef;                     // +0x0C
};

struct CTCInventoryQuests {
    static unsigned char __stdcall IsQuestCardRegionFree(CTCQuestCard* card);
};

unsigned char __stdcall CTCInventoryQuests::IsQuestCardRegionFree(CTCQuestCard* card)
{
    bool started;
    {
        CQuestCardDefPointer def(card->PDef);
        CCharString region = g_DefStringTable.GetString(def->RegionName);
        started = g_pQuestManager->IsActiveQuestStartedInRegion(region);
    }
    return !started;
}