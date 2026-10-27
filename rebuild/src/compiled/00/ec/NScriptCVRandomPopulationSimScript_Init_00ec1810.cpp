// NScript::CV_RandomPopulationSimScript::Init() -- zero the population scales and creature counts, region 1, generator indices 0
// Leaf: ?Init@CV_RandomPopulationSimScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC1810, 43 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012F12D8, which the landed NScript::CV_RandomPopulationSimScript::CV_RandomPopulationSimScript 0x00EC17E0 (and the landed Alloc 0x00EC3BC0) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_RandomPopulationSimScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_RandomPopulationSimScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Shape: the float zeros are integer stores of eax.
class CGameScriptInterface;
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RandomPopulationSimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float         CurrentPopulationScale[4];            // +0x48
    bool          HighDetailSimulationActive;           // +0x58
    long          CurrentPopulationRegion;              // +0x5c
    long          CreaturesAlive[3];                    // +0x60
    unsigned long BanditCreatureGeneratorIndex;         // +0x6c
    unsigned long VillagerCreatureGeneratorIndex;       // +0x70
    unsigned long GuardCreatureGeneratorIndex;          // +0x74
    virtual void Init();
};

void CV_RandomPopulationSimScript::Init()
{
    for (long i = 0; i < 4; ++i)
        CurrentPopulationScale[i] = 0.0f;
    for (long j = 0; j < 3; ++j)
        CreaturesAlive[j] = 0;
    HighDetailSimulationActive = false;
    CurrentPopulationRegion = 1;
    BanditCreatureGeneratorIndex = 0;
    VillagerCreatureGeneratorIndex = 0;
    GuardCreatureGeneratorIndex = 0;
}
}