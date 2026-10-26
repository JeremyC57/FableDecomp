// CTCQuestCard::GetMarkerRegionName() const -- the region the quest marker points at: the teleporter region name, blanked when the world map knows the region and has a route to it from the current region (only an unreachable or unknown region is kept)
// Leaf: GetMarkerRegionName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007021F0, length 137 bytes (padded to 144), `ret 4` (hidden result).
// Label correction: the catalog's "CViewBase::" class is wrong: the neighbours 0x00702280..0x00702400 are CTCQuestCard members and
// +0x24 is CTCQuestCard::TeleporterRegionName (engine/CTCQuestCard.h, PDB offsets match retail here).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x2), CCharString::operator!=(const char*) @ 0x0099E960,
// the world-map accessor @ 0x00686D20 (x3, catalog "GetDisplayEngine"), CWorldMap::GetRegionNumberFromName(const CCharString&)
// @ 0x004FC210, CWorldMap::GetCurrentRegion @ 0x004FB150 (`mov eax,[ecx+9Ch]`; catalog "CWorld::DrawGetEnvironment"),
// CWorldMap::GetNextRegionOnRouteTo(long from, long to) @ 0x004FB4D0, CCharString::operator=(const char*) @ 0x0099EFE0,
// CCharString::~CCharString @ 0x0099EAE0. Data: "" @ 0x0122D70E.
// Shape: a named local copied into the hidden result at the end (no NRVO); the destination region is pushed before the
// current-region call (arguments right to left).
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    bool operator!=(const char* text) const;       // 0x0099E960
    CCharString& operator=(const char* text);      // 0x0099EFE0
};
class CWorldMap { public:
    long GetRegionNumberFromName(const CCharString& name);   // 0x004FC210
    long GetCurrentRegion() const;                 // 0x004FB150
    long GetNextRegionOnRouteTo(long from, long to);         // 0x004FB4D0
};
class CThing;
class CQuestCardDef;
class CTCQuestCard { public:
    void*          __vftable;                      // +0x00
    CThing*        ParentThing;                    // +0x04
    bool           ToBeRemoved;                    // +0x08
    bool           IsInGlobalUpdateTCs;            // +0x09
    CQuestCardDef* PDef;                           // +0x0c CDefPointer<CQuestCardDef const>
    bool           Finished;                       // +0x10
    bool           Replayable;                     // +0x11
    long           FeatIndex;                      // +0x14
    CCharString    QuestName;                      // +0x18
    CCharString    ObjectiveName;                  // +0x1c
    CCharString    RegionName;                     // +0x20
    CCharString    TeleporterRegionName;           // +0x24
    CCharString    ScriptQuestName;                // +0x28
    long           GoldReward;                     // +0x2c
    long           RenownReward;                   // +0x30
    CWorldMap* GetWorldMap() const;                // 0x00686D20
    CCharString GetMarkerRegionName() const;
};
CCharString CTCQuestCard::GetMarkerRegionName() const
{
    CCharString name(TeleporterRegionName);
    if (name != "") {
        long region = GetWorldMap()->GetRegionNumberFromName(name);
        if (region > 0) {
            if (GetWorldMap()->GetNextRegionOnRouteTo(GetWorldMap()->GetCurrentRegion(), region) != -1)
                name = "";
        }
    }
    return name;
}