// NScript::CV_ParanoidWhispersScript::Init() -- paranoid state 1, digging spot hidden
// Leaf: ?Init@CV_ParanoidWhispersScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBEEC0, 12 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012F0F38, which the landed NScript::CV_ParanoidWhispersScript::CV_ParanoidWhispersScript 0x00EBEE90 (and the landed Alloc 0x00EC11B0) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_ParanoidWhispersScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_ParanoidWhispersScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
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
class CV_ParanoidWhispersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long ParanoidState;                                 // +0x48
    bool HideDiggingSpot;                               // +0x4c
    virtual void Init();
};

void CV_ParanoidWhispersScript::Init()
{
    ParanoidState = 1;
    HideDiggingSpot = true;
}
}