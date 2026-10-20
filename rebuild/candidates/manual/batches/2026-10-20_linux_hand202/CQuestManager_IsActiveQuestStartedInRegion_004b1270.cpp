// CQuestManager::IsActiveQuestStartedInRegion(const CCharString&) const -- whether a non-vignette active quest card's definition starts (teleports the hero) in the named region; false for an empty name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1270, length 204 bytes, `ret 4`
// (PDB ?IsActiveQuestStartedInRegion@CQuestManager@@QBE_NABVCCharString@@@Z).
// Callees: CCharString::operator!=(const char*) 0x0099E960 ("" 0x0122D70E), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (TC type 0x6c), CTCQuestCard::
// IsVignette 0x007022E0, CDefString::operator==(const CCharString&) 0x004B7980, the definition's release (slot 1).
// Layout: retail ActiveQuestCards +0x5c; CTCQuestCard PDef +0x0c; CQuestCardDef refcount +4, TeleporterRegionName
// +0x38 (PDB +0x3c, defs sit 4 below the PDB; IsVignette +0x59 confirms the shift).
// Shape: the unchecked GetQuestCard helper again (register seeded from the argument word); the definition is a counted
// copy in an inner block (copy null test, the compare called on the unchecked pointer, then the null-tested release),
// so the bool result rides in bl across the release and `this` is reloaded from its frame word afterwards.
struct CCharString {
    struct CCharStringData* PStringData;
    bool operator!=(const char* text) const;             // 0x0099E960
};
struct CDefString {                                      // 4-byte def-name handle
    long m_index;
    bool operator==(const CCharString& name) const;      // 0x004B7980
};

struct CTCBase;
struct CTCEntry {                                        // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     Type;
    CTCBase* TC;
};
struct CTCMap {                                          // CThing::Components: a sorted vector of pairs (12 bytes)
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase* Find(long type) const         // the inlined map find: the end entry when absent
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};
struct CThing {                                          // retail CThing (local model: offsets PDB -0x10 past the containers)
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                    // +0x20 which TC types the thing holds
    unsigned char _pad40[4];
    CTCMap        Components;                            // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const { return Components.Find(type); }
};

struct CQuestCardDef {                                   // intrusively counted definition (vptr, count +4)
    virtual void v00();
    virtual void Release();                              // slot 1 (+4): destroys the definition
    long          RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void DecRef() { if (--RefCount == 0) Release(); }
    unsigned char _pad08[0x34 - 0x08];
    CDefString    RegionName;                            // +0x34 (PDB +0x38)
    CDefString    TeleporterRegionName;                  // +0x38 (PDB +0x3c)
    unsigned char _pad3c[0x59 - 0x3c];
    bool          IsVignette;                            // +0x59 (PDB +0x5d)
};
struct CQuestCardDefPointer {                            // CDefPointer<const CQuestCardDef>
    CQuestCardDef* Object;
    CQuestCardDefPointer(const CQuestCardDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CQuestCardDefPointer() { if (Object) Object->DecRef(); }
};
struct CTCQuestCard {                                    // TC type 0x6c (retail layout = PDB, 0x34 bytes)
    unsigned char        _pad00[0x0c];                   // CTCBase: vptr, ParentThing, flags
    CQuestCardDefPointer PDef;                           // +0x0c
    bool                 Finished;                       // +0x10
    bool                 Replayable;                     // +0x11
    long                 FeatIndex;                      // +0x14
    CCharString          QuestName;                      // +0x18
    CCharString          ObjectiveName;                  // +0x1c
    CCharString          RegionName;                     // +0x20
    CCharString          TeleporterRegionName;           // +0x24
    CCharString          ScriptQuestName;                // +0x28
    long                 GoldReward;                     // +0x2c
    long                 RenownReward;                   // +0x30
    bool IsVignette() const;                             // 0x007022E0 (PDef->IsVignette)
};
__forceinline CTCQuestCard* GetQuestCard(CThing* thing)  // the unchecked lookup: unset when the thing has no quest card TC
{
    CTCQuestCard* tc;
    if (thing->HasTC(0x6c))
        tc = (CTCQuestCard*)thing->GetTC(0x6c);
    return tc;
}

struct CBaseIntelligentPointer {                         // 8 bytes: vptr, item block
    void* PItem;
    virtual ~CBaseIntelligentPointer();
    CThing* GetPItem() const;                            // 0x00A01B50
};
struct CCardNode {                                       // STLport list<CIntelligentPointer<CThing>> node
    CCardNode*              _M_next;
    CCardNode*              _M_prev;
    CBaseIntelligentPointer _M_data;                     // +0x08
};

struct CQuestManager {
    unsigned char _pad00[0x5c];
    CCardNode*    ActiveQuestCards;                      // +0x5c list<CIntelligentPointer<CThing>> (head node)
    CCardNode*    CompletedQuestCards;                   // +0x60
    bool IsActiveQuestStartedInRegion(const CCharString& regionName) const;
};

bool CQuestManager::IsActiveQuestStartedInRegion(const CCharString& regionName) const
{
    if (regionName != "") {
        for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
            CTCQuestCard* tc = GetQuestCard(node->_M_data.GetPItem());
            if (tc && !tc->IsVignette()) {
                bool started;
                {
                    CQuestCardDefPointer def(tc->PDef);
                    started = def.Object->TeleporterRegionName == regionName;
                }
                if (started)
                    return true;
            }
        }
    }
    return false;
}
