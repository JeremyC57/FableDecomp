// CNOVI_Villager::GetVillagerSpeechIndex(long) -- pick a random villager speech index different from the last one used (waiting a frame on a repeat), record it in the quest script and return it; 0 once the script is done
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAEBF0, 107 bytes (+ int3 padding; `ret 4`).
// Callees: CEntityScriptBase::IsDone() @ 0x00F35B30 (manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated),
//   rand @ 0x00BFEB16, the game script interface's slot 7 (+0x1c, the frame update / yield).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14 (CQ_NewOakValeIntroScript; its
//   lastVillagerSpeechIdx sits at retail +0x98, PDB +0xac: the retail quest-script base is 0x14 bytes smaller).
// Shape: quest-script style -- every statement after a possible yield re-tests IsDone(); the final
//   `IsDone() ? 0 : index` is the `neg al ; sbb eax,eax ; not eax ; and eax,edi` tail.
struct CGameScriptInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void Update();                         // slot 7 (+0x1c)
};
struct CScriptThing {
    unsigned char _body[12];
};
struct CQ_NewOakValeIntroScript {
    unsigned char _pad00[0x98];
    long          lastVillagerSpeechIdx;           // +0x98 (retail)
};
extern "C" int __cdecl rand();                     // 0x00BFEB16

struct CNOVI_Villager {
    void*                     __vftable;           // +0x00
    CGameScriptInterface*     Game;                // +0x04
    CScriptThing              Me;                  // +0x08
    CQ_NewOakValeIntroScript* ParentClass;         // +0x14
    void*                     PMasterData;         // +0x18
    bool                      HeroDidHitMe;        // +0x1c
    bool IsDone();                                 // 0x00F35B30
    long GetVillagerSpeechIndex(long count);
};

long CNOVI_Villager::GetVillagerSpeechIndex(long count)
{
    while (!IsDone()) {
        long index = rand() % count;
        if (index != ParentClass->lastVillagerSpeechIdx) {
            if (IsDone())
                return 0;
            ParentClass->lastVillagerSpeechIdx = index;
            return IsDone() ? 0 : index;
        }
        Game->Update();
    }
    return 0;
}