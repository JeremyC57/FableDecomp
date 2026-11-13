#ifndef FABLE_ENGINE_CQ_HOBBECAVESCRIPT_H
#define FABLE_ENGINE_CQ_HOBBECAVESCRIPT_H
// Retail opt-in (chunk 788): NScript::CQ_HobbeCaveScript, new header (no includer before chunk 788).
// Layout: Ego_r.pdb CQ_HobbeCaveScript (struct_layouts_egor.tsv, 140 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): bools +0x48..+0x50, CTimer BanditComment +0x54 (the Alloc's
// destructor 0x00D99E10 runs ~CTimer on this+0x54), GotTreasure +0x58 .. WaitForJamesReleased +0x74, sizeof 0x78. Checked against
// retail: Main 0x00D8DD10 reads SavedBeforeCaves +0x49, QuestStartScreened +0x69, sets ReachedHobbeCave +0x6a; the landed
// WatchForTermination 0x00D8E990 reads MissionSucceeded +0x4b, MissionFailed +0x4c, ReasonForFailure +0x70.
// Retail methods: Init 0x00D8D640 (slot 3), Main 0x00D8DD10 (slot 2 of the script vtable 0x012D5540, which the "Q_HobbeCave"
// Alloc 0x00D99DC0 stores); the four threads Main spawns under their thread-name literals (Ego_r.pdb
// ?<Name>@CQ_HobbeCaveScript@NScript@@QAEXXZ): RescuedBandit 0x00D91740, WatchForTermination 0x00D8E990, WatchForHavingKey
// 0x00D8EE50, WatchForGrannysRegion 0x00D8F040.
// Nested entity scripts: the fifteen classes Main binds (Ego_r.pdb ??0<Class>@CQ_HobbeCaveScript@NScript@@...; the binding-name
// literal is the class name less its `C`), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, the one CLevelScriptBase instance in the Ego_r.pdb name set.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_HobbeCaveScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeCaveScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D8DC00 (slot 1)
    virtual void Main();                                                 // 0x00D8DD10 (slot 2)
    virtual void Init();                                                 // 0x00D8D640 (slot 3)
#ifdef FABLE_RETAIL_CQ_HOBBECAVESCRIPT_ONPERSIST
    // Retail opt-in (chunk 858): slot 4 of the script vtable 0x012D5540 is 0x00D8DCC0 (.rdata ref 0x012D5550), Ego_r.pdb /
    //   FableWin.pdb ?OnPersist@CQ_HobbeCaveScript@NScript@@UAEXAAVCPersistContext@@@Z (overrides CScriptBase slot 4; no layout change).
    virtual void OnPersist(CPersistContext&);                            // 0x00D8DCC0 (slot 4)
#endif
    void RescuedBandit();                                                // 0x00D91740
    void WatchForTermination();                                          // 0x00D8E990
    void WatchForHavingKey();                                            // 0x00D8EE50
    void WatchForGrannysRegion();                                        // 0x00D8F040
    void CheckIfGotTreasure();
    void CheckJamesKilled(CScriptThing);
    bool MakeBanditComment(const CCharString&, const CScriptThing&);
    bool DoneIntro;                                                      // +0x48
    bool SavedBeforeCaves;                                               // +0x49
    bool BratInCutscene;                                                 // +0x4a
    bool MissionSucceeded;                                               // +0x4b
    bool MissionFailed;                                                  // +0x4c
    bool AlarmTriggered;                                                 // +0x4d
    bool KilledSideRoomHobbes;                                           // +0x4e
    bool ReleaseBrat;                                                    // +0x4f
    bool BratReturned;                                                   // +0x50
    CTimer BanditComment;                                                // +0x54
    long GotTreasure;                                                    // +0x58
    long BanditHitCount;                                                 // +0x5c
    bool KingGruntedOrders;                                              // +0x60
    long HobbesToSpawn;                                                  // +0x64
    bool InCutscene;                                                     // +0x68
    bool QuestStartScreened;                                             // +0x69
    bool ReachedHobbeCave;                                               // +0x6a
    bool HobbesCloseby;                                                  // +0x6b
    bool NymphAngry;                                                     // +0x6c
    long ReasonForFailure;                                               // +0x70
    long WaitForJamesReleased;                                           // +0x74
    class CHobbeSpawn;                                                   // Alloc 0x00D8F310
    class CHobbeTorrent;                                                 // Alloc 0x00D8F3F0
    class CHobbeRoaming;                                                 // Alloc 0x00D8F4F0
    class CHobbeLeader;                                                  // Alloc 0x00D8F950
    class CHobbeLarder;                                                  // Alloc 0x00D8F5E0
    class CGranny;                                                       // Alloc 0x00D8FC70
    class CNymph;                                                        // Alloc 0x00D8FE30
    class CJames;                                                        // Alloc 0x00D8FFA0
    class CJamesCommentMarker;                                           // Alloc 0x00D906D0
    class CJamesCommentMarkerAdv;                                        // Alloc 0x00D90A50
    class CBanditCommentMarker;                                          // Alloc 0x00D901B0
    class CBanditSacrifice;                                              // Alloc 0x00D90E90
    class CBanditInRoomSample;                                           // Alloc 0x00D90FF0
    class CCheckTheSilverKeyChest;                                       // Alloc 0x00D91250
    class CInvisibleCage;                                                // Alloc 0x00D91470
};
#ifdef FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBETORRENT
// Retail opt-in (chunk 2046): the entity script CHobbeTorrent (Ego_r.pdb CHobbeTorrent, 32 bytes: ParentClass +0x14 (pointer),
//   PMasterData +0x18, NumberToGenerate +0x1C; retail vtable 0x012D5960 stored by the landed Alloc 0x00D8F3F0: slot 1 Main
//   0x00D92380, slot 4 OnPersist 0x00D91930). Method spellings: Ego_r.pdb / FableWin.pdb ?<Name>@CHobbeTorrent@CQ_HobbeCaveScript@NScript@@.
class CQ_HobbeCaveScript::CHobbeTorrent : public CEntityScriptBase {
public:
    CHobbeTorrent(CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me)
        : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
    {
        PMasterData = (CQ_SunnyvaleMasterData*)data;
    }
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HobbeCaveScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D8F3F0
    virtual CScriptBase* GetParentScript() const;                        // slot 3
private:
    virtual void Init();                                                 // slot 2
    virtual void Main();                                                 // slot 1  0x00D92380
    virtual void OnPersist(CPersistContext&);                            // slot 4  0x00D91930
public:
    CQ_HobbeCaveScript* ParentClass;                                     // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    long NumberToGenerate;                                               // +0x1C
};
#endif
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HobbeCaveScript) == 0x78);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_HOBBECAVESCRIPT_H
