// NScript::CV_GuildMasterScript::Init() -- clear the five GuildMasterDialogue flags
// Leaf: ?Init@CV_GuildMasterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E90780, 11 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012EC138, which the landed constructor 0x00E90750 (catalog CVtable_012EC138::CVtable_012EC138) and the landed Alloc 0x00E92900 (Alloc_CVtable_012ec138) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_GuildMasterScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_GuildMasterScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Identity, second source: slot 2 (Main 0x00E90830) binds "GuildMasterGameFlow"; Ego_r.pdb CV_GuildMasterScript is
//   {bool GuildMasterDialogue[5] +0x5c, CCharString LastDialogueSaid +0x64} = retail +0x48 / +0x50 (the landed constructor).
// Shape: the five-byte clear is one dword store and one byte store through `lea eax,[ecx+0x48]`.
class CGameScriptInterface;
class CCharString {
public:
    void* PData;
};
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
class CV_GuildMasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool        GuildMasterDialogue[5];                 // +0x48
    CCharString LastDialogueSaid;                       // +0x50 (the CCharString the landed constructor builds at +0x50)
    virtual void Init();
};

void CV_GuildMasterScript::Init()
{
    for (long i = 0; i < 5; ++i)
        GuildMasterDialogue[i] = false;
}
}