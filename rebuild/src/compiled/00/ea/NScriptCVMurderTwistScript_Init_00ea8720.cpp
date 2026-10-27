// NScript::CV_MurderTwistScript::Init() -- no items found yet, state and event both 1
// Leaf: ?Init@CV_MurderTwistScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA8720, 19 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012EE634, which its constructor 0x00EA86F0 (manifest label CEngineSubPrimitiveBase::CEngineSubPrimitiveBase, propagated) and Alloc 0x00EAE890 stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_MurderTwistScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_MurderTwistScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Identity, second source: slot 2 (Main 0x00EA8830) binds the MurderTwistLeader / MurderTwistBrother* entities.
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
class CV_MurderTwistScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long MurderTwistItemsFound;                         // +0x48
    long MurderTwistState;                              // +0x4c
    long MurderTwistEvent;                              // +0x50
    virtual void Init();
};

void CV_MurderTwistScript::Init()
{
    MurderTwistItemsFound = 0;
    MurderTwistState = 1;
    MurderTwistEvent = 1;
}
}