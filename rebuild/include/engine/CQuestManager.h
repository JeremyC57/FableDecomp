#ifndef FABLE_ENGINE_CQUESTMANAGER_H
#define FABLE_ENGINE_CQUESTMANAGER_H
#ifdef FABLE_RETAIL_CQUESTMANAGER
// Retail branch (chunk 729, rows 0x005FF190 / 0x00603000 CTCInventoryMap::CreateMapMarkers / CreateMiniMapMarkers): the
// polymorphic retail class. RTTI/PDB: CQuestManager : CTBaseSingletonNoDefaultAlloc<CQuestManager> (Ego_r.pdb
// ??_G?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@UAEPAXI@Z; the singleton's PClass is 0x013B89FC, landed
// FindQuestsForActiveRegion 0x005FD0F0), vptr +0x00; the members of the generated default below at the same retail offsets,
// with ActiveQuestCards (+0x5C) typed: Ego_r.pdb list<CIntelligentPointer<CThing> > (PDB +0x88; STLport list = 4 bytes) and
// the inline accessor ?GetActiveQuestCards@CQuestManager@@QBEABV?$list@V?$CIntelligentPointer@VCThing@@@@...@std@@XZ
// (Ego_r.pdb + FableWin.pdb; CreateMapMarkers copies the list it returns: `mov esi,[0x013B89FC]; add esi,5Ch`, then the
// list copy constructor's _M_insert_dispatch 0x00618100). Other members stay opaque storage.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
class CPersistContext;
#ifdef FABLE_RETAIL_CQUESTMANAGER_REGIONLOCK
class CRegion;
namespace std {
// STLport 4.6 _algobase.c: the random-access __find (out of line in retail: 0x004B74E0 for const CRegion* const*, __fastcall);
// declared only (a row that needs VC7.1 to see it writes no memory defines the body in its TU) and _algobase.h find.
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&);
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val) {
    return __find(__first, __last, __val, random_access_iterator_tag());
}
}
#endif
#include "fable_string.h"
class CQuestManager : public CTBaseSingletonNoDefaultAlloc<CQuestManager> {
public:
    // Chunk 732 (row 0x004B05C0 TransferBoasts): the nested boast record. Ego_r.pdb layout CBoast (32 bytes): ID +0, Cost +4,
    // Reward +8, ScriptName +0xC, Text +0x10, Title +0x14, Evil +0x18, Failed +0x19, Completed +0x1A, GroupIndex +0x1C; nested
    // in CQuestManager (Ego_r ??0CBoast@CQuestManager@@QAE@ABV01@@Z, `V` key). The default constructor is inline in retail
    // (TransferBoasts 0x004B094A..0x004B0978: ScriptName, Text, Title, then Failed / Completed / GroupIndex zeroed; ID, Cost,
    // Reward and Evil left alone); the copy constructor is out of line (0x004B73A0).
    class CBoast {
    public:
        long        ID;                                      // +0x00
        long        Cost;                                    // +0x04
        long        Reward;                                  // +0x08
        CCharString ScriptName;                              // +0x0c
        CWideString Text;                                    // +0x10
        CWideString Title;                                   // +0x14
        bool        Evil;                                    // +0x18
        bool        Failed;                                  // +0x19
        bool        Completed;                               // +0x1a
        long        GroupIndex;                              // +0x1c
        CBoast() : Failed(false), Completed(false), GroupIndex(0) {}   // FableWin ??0CBoast@CQuestManager@@QAE@XZ (inline in retail)
        CBoast(const CBoast& other);                         // 0x004B73A0 ??0CBoast@CQuestManager@@QAE@ABV01@@Z
        // FableWin.pdb accessors (no Ego_r copy: inline in retail; TransferBoasts expands them). SetScriptName takes the
        // string by value (?SetScriptName@CBoast@CQuestManager@@QAEXVCCharString@@@Z): retail copies the loaded name into the
        // parameter's temporary and assigns from its re-taken address (0x004B0965..0x004B0997).
        long GetID() const { return ID; }                    // ?GetID@CBoast@CQuestManager@@QBEJXZ
        long GetCost() const { return Cost; }                // ?GetCost@CBoast@CQuestManager@@QBEJXZ
        long GetReward() const { return Reward; }            // ?GetReward@CBoast@CQuestManager@@QBEJXZ
        long GetGroupIndex() const { return GroupIndex; }    // ?GetGroupIndex@CBoast@CQuestManager@@QBEJXZ
        const CCharString& GetScriptName() const { return ScriptName; }   // ?GetScriptName@CBoast@CQuestManager@@QBEABVCCharString@@XZ
        const CWideString& GetText() const { return Text; }  // ?GetText@CBoast@CQuestManager@@QBEABVCWideString@@XZ
        const CWideString& GetTitle() const { return Title; }   // ?GetTitle@CBoast@CQuestManager@@QBEABVCWideString@@XZ
        bool IsEvil() const { return Evil; }                 // ?IsEvil@CBoast@CQuestManager@@QBE_NXZ
        bool IsFailed() const { return Failed; }             // ?IsFailed@CBoast@CQuestManager@@QBE_NXZ
        bool IsCompleted() const { return Completed; }       // ?IsCompleted@CBoast@CQuestManager@@QBE_NXZ
        void SetID(long id) { ID = id; }                     // ?SetID@CBoast@CQuestManager@@QAEXJ@Z
        void SetScriptName(CCharString script_name) { ScriptName = script_name; }   // ?SetScriptName@CBoast@CQuestManager@@QAEXVCCharString@@@Z
        void SetAsFailed() { Failed = true; }                // ?SetAsFailed@CBoast@CQuestManager@@QAEXXZ
        void SetAsCompleted() { Completed = true; }          // ?SetAsCompleted@CBoast@CQuestManager@@QAEXXZ
    };
    const std::list<CIntelligentPointer<CThing> >& GetActiveQuestCards() const { return ActiveQuestCards; }
    const std::list<CIntelligentPointer<CThing> >& GetCompletedQuestCards() const { return CompletedQuestCards; }
    // Ego_r.pdb / FableWin.pdb ?TransferBoasts@CQuestManager@@QAEXAAVCPersistContext@@@Z (retail 0x004B05C0).
    void TransferBoasts(CPersistContext& context);
    // Chunk 734 (row 0x004A5A40 CWorld::UpdateFrame): Ego_r.pdb / FableWin.pdb ?Update@CQuestManager@@QAEXXZ (retail 0x004B4490, landed).
    void Update();                                           // 0x004B4490
    // Chunk 734 (row 0x004A5E90 CWorld::AutoLoadRespawn): Ego_r.pdb / FableWin.pdb ?IsBetweenQuestStartAndFinishScreens@CQuestManager@@QBE_NXZ.
    bool IsBetweenQuestStartAndFinishScreens() const;        // 0x004AF290
    // Chunk 734 (row 0x004A0D90 CWorld::LoadQuestsFromFile): Ego_r.pdb / FableWin.pdb ?RegisterQuest@CQuestManager@@QAEXABVCCharString@@@Z.
    void RegisterQuest(const CCharString& name);             // 0x004B2850
#ifdef FABLE_RETAIL_CQUESTMANAGER_REGIONLOCK
    // Nested opt-in (chunk 763, row 0x004B01B0 IsRegionValidInRegionLockingMode): the nested active-quest record
    // (Ego_r.pdb / FableWin.pdb ?ContainsRegion@CActiveQuest@CQuestManager@@QBE_NABVCRegion@@@Z: `V` key, nested) with its
    // region list at +0x10 (vector<const CRegion*>; the row reads _M_start +0x10 / _M_finish +0x14). ContainsRegion,
    // ContainsAnyRegions and GetRegions (by value: ?GetRegions@CActiveQuest@CQuestManager@@QBE?BV?$vector@PBVCRegion@@...)
    // are expanded in the row (the find is a direct call of std::__find 0x004B74E0, the copy a malloc / memmove / free).
    class CActiveQuest {
    public:
        bool ContainsRegion(const CRegion& region) const {
            return std::find(Regions.begin(), Regions.end(), &region) != Regions.end();
        }
        bool ContainsAnyRegions() const { return !Regions.empty(); }
        const std::vector<const CRegion*> GetRegions() const { return Regions; }
    private:
        unsigned char _pad_0x00[0x10];                       // +0x00 ID, PScriptInfo, PScript (CCountedPointer<CScriptBase>)
        std::vector<const CRegion*> Regions;                 // +0x10
    };
    // FableWin.pdb ?GetActiveQuestFromScriptName@CQuestManager@@QBEPAVCActiveQuest@1@ABVCCharString@@@Z (retail 0x004AF5B0).
    CActiveQuest* GetActiveQuestFromScriptName(const CCharString& name) const;    // 0x004AF5B0
    // Ego_r.pdb / FableWin.pdb ?IsRegionValidInRegionLockingMode@CQuestManager@@QBE_NABVCRegion@@@Z (retail 0x004B01B0).
    bool IsRegionValidInRegionLockingMode(const CRegion& region) const;
private:
    unsigned char _pad_0x04[0x4c - 0x04];                    // +0x04 PGameScriptThingManager .. ActiveQuestsStack
    std::vector<CCharString> BetweenStartAndEndScreenQuests; // +0x4c (Ego_r vector<CCharString>)
    unsigned char _pad_0x58[0x5c - 0x58];                    // +0x58 AvailableQuestCardTypes
#else
private:
    unsigned char _pad_0x04[0x5c - 0x04];                    // +0x04 PGameScriptThingManager .. AvailableQuestCardTypes
#endif
    std::list<CIntelligentPointer<CThing> > ActiveQuestCards;    // +0x5c
    std::list<CIntelligentPointer<CThing> > CompletedQuestCards; // +0x60
    unsigned long FrameBoastsLastUpdated;                    // +0x64
    std::list<CBoast> Boasts;                                // +0x68 (Ego_r list<CQuestManager::CBoast>)
    std::list<CBoast> AcceptedBoasts;                        // +0x6c
    std::list<CBoast> TempBoasts;                            // +0x70
    unsigned char _pad_0x74[0xb4 - 0x74];                    // +0x74 Objectives .. PMostRecentValidUsedTargetName
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CQuestManager, ActiveQuestCards) == 0x5c);
#ifdef FABLE_RETAIL_CQUESTMANAGER_REGIONLOCK
        FABLE_STATIC_ASSERT(offsetof(CQuestManager, BetweenStartAndEndScreenQuests) == 0x4c);
#endif
        FABLE_STATIC_ASSERT(offsetof(CQuestManager, AcceptedBoasts) == 0x6c);
        FABLE_STATIC_ASSERT(offsetof(CQuestManager, TempBoasts) == 0x70);
        FABLE_STATIC_ASSERT(sizeof(CBoast) == 0x20);
    }
};
FABLE_STATIC_ASSERT(sizeof(CQuestManager) == 0xb4);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CQuestManager = 0x114.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CQuestManager = 0xb4.
// Retail evidence: 3 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CActiveQuest;
class CCPPointerInfo;
class CCharStringData;
class CGameScriptInterface;
class CGameScriptThingManager;
class CScriptInfoManager;
class CWorld;

#pragma pack(push, 1)
class CQuestManager {
public:
    void*                    __vftable;                      // +0x00 vptr, or first dword of a flattened base subobject
    CGameScriptThingManager* PGameScriptThingManager_Data;   // +0x04 CCountedPointer<CGameScriptThingManager>::Data
    CCPPointerInfo*          PGameScriptThingManager_Info;   // +0x08 CCountedPointer<CGameScriptThingManager>::Info
    unsigned char            RuntimeQuestPersistence[0x20];  // +0x0c CRuntimeQuestPersistence
    unsigned char            RegisteredQuestNames[0xc];      // +0x2c vector<CCharString,std::allocator<CCharString>_>
    unsigned char            ActiveQuests[0x4];              // +0x38 list<CCountedPointer<CQuestManager::CActiveQuest>,std::allocator<CCountedPointer<CQuestManager::CActiveQuest>_>_>
    unsigned char            PendingQuestsToDeactivate[0x4]; // +0x3c list<std::pair<CCharString,unsigned_long>,std::allocator<std::pair<CCharString,unsigned_long>_>_>
    unsigned char            CreatureGenerationDisabledQuests[0xc];  // +0x40 vector<CCharString,std::allocator<CCharString>_>
    unsigned char            BetweenStartAndEndScreenQuests[0xc];  // +0x4c vector<CCharString,std::allocator<CCharString>_>
    unsigned char            AvailableQuestCardTypes[0x4];   // +0x58 list<CQuestManager::CQuestCard,std::allocator<CQuestManager::CQuestCard>_>
    unsigned char            ActiveQuestCards[0x4];          // +0x5c list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char            CompletedQuestCards[0x4];       // +0x60 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned long            FrameBoastsLastUpdated;         // +0x64
    unsigned char            Boasts[0x4];                    // +0x68 list<CQuestManager::CBoast,std::allocator<CQuestManager::CBoast>_>
    unsigned char            AcceptedBoasts[0x4];            // +0x6c list<CQuestManager::CBoast,std::allocator<CQuestManager::CBoast>_>
    unsigned char            TempBoasts[0x4];                // +0x70 list<CQuestManager::CBoast,std::allocator<CQuestManager::CBoast>_>
    unsigned char            Objectives[0x4];                // +0x74 list<CQuestManager::CObjective,std::allocator<CQuestManager::CObjective>_>
    CScriptInfoManager*      ScriptInfoManager;              // +0x78
    CWorld*                  World;                          // +0x7c
    CGameScriptInterface*    GameInterface;                  // +0x80
    long                     CurrentQuestID;                 // +0x84
    CActiveQuest*            PQuestCurrentlyRunning;         // +0x88
    bool                     DayActive;                      // +0x8c
    bool                     NightActive;                    // +0x8d
    bool                     WorldDestructed;                // +0x8e
    bool                     QuestCardsNeedUpdating;         // +0x8f
    long                     HeroMapNumber;                  // +0x90
    long                     LastAutoSaveFrame;              // +0x94
    long                     NumberOfQuestsBetweenStartAndFinishScreens; // +0x98
    unsigned char            QuestsWithResourcesLoadedThatAreNotActivated[0xc];  // +0x9c set<CCharString,std::less<CCharString>,std::allocator<CCharString>_>
    unsigned char            PMostRecentValidUsedTarget[0x8]; // +0xa8 CIntelligentPointer<CThing>
    CCharStringData*         PMostRecentValidUsedTargetName_PStringData; // +0xb0 CCharString::PStringData
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CQuestManager) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, PGameScriptThingManager_Data) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, PGameScriptThingManager_Info) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, RuntimeQuestPersistence) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, RegisteredQuestNames) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, ActiveQuests) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, PendingQuestsToDeactivate) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, CreatureGenerationDisabledQuests) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, BetweenStartAndEndScreenQuests) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, AvailableQuestCardTypes) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, ActiveQuestCards) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, CompletedQuestCards) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, FrameBoastsLastUpdated) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, Boasts) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, AcceptedBoasts) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, TempBoasts) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, Objectives) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, ScriptInfoManager) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, World) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, GameInterface) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, CurrentQuestID) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, PQuestCurrentlyRunning) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, DayActive) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, NightActive) == 0x8d);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, WorldDestructed) == 0x8e);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, QuestCardsNeedUpdating) == 0x8f);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, HeroMapNumber) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, LastAutoSaveFrame) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, NumberOfQuestsBetweenStartAndFinishScreens) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, QuestsWithResourcesLoadedThatAreNotActivated) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, PMostRecentValidUsedTarget) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CQuestManager, PMostRecentValidUsedTargetName_PStringData) == 0xb0);

#endif // FABLE_RETAIL_CQUESTMANAGER
#endif // FABLE_ENGINE_CQUESTMANAGER_H
