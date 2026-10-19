// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CTCInventory::IsSaveMenuReceivingInput() -- scans each inventory screen's COMPONENT_SAVE entries for input states 1 or 2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058376C, 169 bytes. The two-state query and list lifetime are kept inline to reproduce both exits.
// Callees: STLport list<entry*> default ctor 0x0042ABAF and dtor 0x0042ABCA (manifest "CActiveFile::OnReadFinished" is a
//   propagated twin), CCharString(const char*, long) 0x0099EBF0 for the by-value name argument, the screen's virtual
//   slot 5 (+0x14) entry search, and the entry's second-base virtual slot 2 (+0x08) state query.
// Local model: the CTCInventory screen vector sits at +0x0c (retail); no engine/ header declares these members.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);       // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
struct CStateQuery {
    virtual void v00(); virtual void v01();
    virtual bool IsInState(long state);              // slot 2
};
struct CComponentBase { virtual void v00(); };
struct CMenuEntry : CComponentBase, CStateQuery {};
template<class T> struct CListAllocator { CListAllocator() {} };
struct CEntryList {
    struct SNode { SNode* next; SNode* prev; CMenuEntry* data; };
    SNode* node;
    explicit CEntryList(const CListAllocator<CMenuEntry*>& a = CListAllocator<CMenuEntry*>()); // 0x0042ABAF
    ~CEntryList(); // 0x0042ABCA
};
struct CMenuScreen {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void FindEntries(CCharString name, CEntryList& out); // slot 5, 0x14
};
struct CScreenVector {
    CMenuScreen** first; CMenuScreen** last;
    unsigned long size() const { return (unsigned long)(last - first); }
    CMenuScreen*& operator[](unsigned long n) const { return first[n]; }
};
struct CTCInventory {
    unsigned char _pad[0x0c];
    CScreenVector Screens;
    bool IsSaveMenuReceivingInput();
};
bool CTCInventory::IsSaveMenuReceivingInput()
{
    unsigned long i = 0;
    for (; i < Screens.size(); ++i) {
        CEntryList entries;
        Screens[i]->FindEntries(CCharString("COMPONENT_SAVE", -1), entries);
        for (CEntryList::SNode* node = entries.node->next; node != entries.node; node = node->next) {
            CMenuEntry* entry = node->data;
            if (entry) {
                CStateQuery* query = entry;                      // second base, +0x04
                if (query->IsInState(1) || query->IsInState(2)) {
                    return true;
                }
            }
        }
    }
    return false;
}