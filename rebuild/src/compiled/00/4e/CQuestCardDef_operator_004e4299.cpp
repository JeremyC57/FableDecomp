// CQuestCardDef::operator=(const CQuestCardDef&) -- def copy: CDefClassBase part, then the CQuestCardDef fields memberwise in declaration order
// Leaf: ??4CQuestCardDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E4299, 141 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004E4294 in slot 19 of vtable 0x01241E44 = CQuestCardDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<long>::operator= @ 0x00454886, map<EQuestCardPrerequisite,long>::operator= @ 0x004E4326.
// Layout (engine/CQuestCardDef.h, PDB minus 4 after RewardObjects and 8 after Prerequisites): QuestName +0x28, QuestSummary +0x2c, QuestObjective +0x30, SuccessSummary +0x34, RegionName +0x38, TeleporterRegionName +0x3c, InventoryCategory +0x40, RenownReward +0x44, GoldReward +0x48, RewardObjects +0x4c, IsCore +0x58, IsVignette +0x59, IsExclusive +0x5a, MakeVignetteRouteAppearOnMinimap +0x5b, Prerequisites +0x5c, NumBoasts +0x68, CanPlayerCancel +0x6c, QuestEpilogue +0x70.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
#pragma optimize("s", on)

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CLongVector {                       // retail std::vector<long> (retail 0xc bytes)
    unsigned long m_words[3];
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};
struct CPrerequisiteMap {                       // retail std::map<EQuestCardPrerequisite,long> (12 bytes) (retail 0xc bytes)
    unsigned long m_words[3];
    CPrerequisiteMap& operator=(const CPrerequisiteMap& other);   // 0x004E4326
};
class CDefString { public:
    long TablePos;                      // index into the def string table (trivially copied)
};

class CQuestCardDef : public CDefClassBase { public:
    long                     QuestName;                   // +0x28
    long                     QuestSummary;                // +0x2c
    long                     QuestObjective;              // +0x30
    long                     SuccessSummary;              // +0x34
    CDefString               RegionName;                  // +0x38
    CDefString               TeleporterRegionName;        // +0x3c
    long                     InventoryCategory;           // +0x40
    long                     RenownReward;                // +0x44
    long                     GoldReward;                  // +0x48
    CLongVector              RewardObjects;               // +0x4c
    bool                     IsCore;                      // +0x58
    bool                     IsVignette;                  // +0x59
    bool                     IsExclusive;                 // +0x5a
    bool                     MakeVignetteRouteAppearOnMinimap; // +0x5b
    CPrerequisiteMap         Prerequisites;               // +0x5c
    long                     NumBoasts;                   // +0x68
    bool                     CanPlayerCancel;             // +0x6c
    unsigned long            QuestEpilogue;               // +0x70
    CQuestCardDef& operator=(const CQuestCardDef& other);
};

CQuestCardDef& CQuestCardDef::operator=(const CQuestCardDef& other)
{
    CDefClassBase::operator=(other);
    QuestName = other.QuestName;
    QuestSummary = other.QuestSummary;
    QuestObjective = other.QuestObjective;
    SuccessSummary = other.SuccessSummary;
    RegionName = other.RegionName;
    TeleporterRegionName = other.TeleporterRegionName;
    InventoryCategory = other.InventoryCategory;
    RenownReward = other.RenownReward;
    GoldReward = other.GoldReward;
    RewardObjects = other.RewardObjects;
    IsCore = other.IsCore;
    IsVignette = other.IsVignette;
    IsExclusive = other.IsExclusive;
    MakeVignetteRouteAppearOnMinimap = other.MakeVignetteRouteAppearOnMinimap;
    Prerequisites = other.Prerequisites;
    NumBoasts = other.NumBoasts;
    CanPlayerCancel = other.CanPlayerCancel;
    QuestEpilogue = other.QuestEpilogue;
    return *this;
}