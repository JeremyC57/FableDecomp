// CTCVillage::UpdateMirroredCrimeData() -- mirror the village's crime records into the hero-facing copy, updating each entry against the main player's hero
// Grade: DIFFER(421v421) -- size and calls equal; retail keeps this in ebx and the hero in another callee-saved reg, ours puts the hero in ebx (register allocation only)
// Leaf: UpdateMirroredCrimeData
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};
struct CBitset {
    unsigned long Bits[9];
    bool test(unsigned int pos) const;                      // 0x00410DE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       m_tcBits;                                 // +0x20
    CInterfaceMap m_tcs;                                    // +0x44
    __forceinline bool HasTC(long type) const { return m_tcBits.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_tcs;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CPlayer { CThing* GetHeroThing(); };                 // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };        // 0x0049D9D0
struct CMainGameComponent { unsigned char _pad00[0x24]; CPlayerManager* PlayerManager; };
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

struct CRegion { unsigned char _pad00[0x18]; CCharString Name; };
struct CWorldMap {
    long GetCurrentRegion();                                // 0x004FB150
    CRegion* GetRegion(long region);                        // 0x004FC180
};
struct SVillageCrimeData {                                  // 0x44 bytes
    long CrimeCounts[10];                                   // +0x00
    long FramePlayerLastSeenByGuard;                        // +0x28
    long Other[6];
};
struct CCrimeDataMap {
    SVillageCrimeData& operator[](const CCharString& region);   // 0x005812DF
};
struct CTCCrimeMirror {                                     // the hero's TC 4
    unsigned char _pad00[0x0c];
    CCrimeDataMap Crimes;                                   // +0x0c
};
struct CDeed { long Type; };
struct CDeedDef {
    virtual void v00();
    virtual void DeleteThis();                              // slot 1
    long RefCount;                                          // +0x04
    unsigned char _pad08[0x7c - 0x08];
    unsigned char Flags;                                    // +0x7c
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDeedDefPointer {
    CDeedDef* Object;
    __forceinline ~CDeedDefPointer() { if (Object) { Object->Release(); Object = 0; } }
    CDeedDef* operator->() const { return Object; }
};
struct CTCHeroOpinionDeedLog {                              // the hero's TC 0x51
    CDeed* GetDeedFromHandle(long handle);                  // 0x006C58F0
    CDeedDefPointer GetDeedDef(long type);                  // 0x006C5630
};
struct CLongSetNode {
    long  Color;
    CLongSetNode* Parent;
    CLongSetNode* Left;
    CLongSetNode* Right;
    long  Value;                                            // +0x10
};
CLongSetNode* IncrementNode(CLongSetNode* node);            // 0x004292C0
struct CLongSet {
    CLongSetNode* Header;
    long  NodeCount;
    long  Pad;
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    CWorldMap* GetWorldMap() const;                         // 0x00686D20
};
struct CTCVillage : CTCBase {
    unsigned char _pad08[0x64 - 0x08];
    CLongSet PlayersCrimes;                                 // +0x64
    unsigned char _pad70[0x80 - 0x70];
    long FramePlayerLastSeenByGuard;                        // +0x80
    void UpdateMirroredCrimeData();
};

void CTCVillage::UpdateMirroredCrimeData()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero == 0)
        return;
    if (!hero->HasTC(4))
        return;
    CTCCrimeMirror* mirror = (CTCCrimeMirror*)hero->GetTC(4);
    CCharString region = GetWorldMap()->GetRegion(GetWorldMap()->GetCurrentRegion())->Name;
    SVillageCrimeData data = mirror->Crimes[region];
    data.FramePlayerLastSeenByGuard = FramePlayerLastSeenByGuard;
    for (long i = 0; i < 10; ++i)
        data.CrimeCounts[i] = 0;
    CTCHeroOpinionDeedLog* deedLog;
    if (hero->HasTC(0x51))
        deedLog = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
    CLongSetNode* end = PlayersCrimes.Header;
    for (CLongSetNode* node = end->Left; node != end; node = IncrementNode(node)) {
        CDeed* deed = deedLog->GetDeedFromHandle(node->Value);
        if (deed && deed->Type < 10) {
            if (!(deedLog->GetDeedDef(deed->Type)->Flags & 0x20) || data.CrimeCounts[deed->Type] == 0)
                ++data.CrimeCounts[deed->Type];
        }
    }
    mirror->Crimes[region] = data;
}
