// NScript::CQ_OakValeBanditRaidScript::Init() -- reset the bandit-raid state (FirstSetup = true, the rest false/0)
// Leaf: ?Init@CQ_OakValeBanditRaidScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DBF1E0, 34 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the quest vtable 0x012D9E18 (slot 1 = `NScript::CQ_OakValeBanditRaidScript::RegisterMain`), stored by the quest ctor 0x00DBF190; ?Init@CQ_OakValeBanditRaidScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb. The quest Alloc 0x00DC24E0 stores the same vtable.
// Label: the manifest's "sub_00dbf1e0" is a gap-scan placeholder.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets until the first STL container. Before HerosFollowers (+0x4c, 12-byte STLport vector) members sit 0x14 lower, after it 0x18 lower: BanditsAwareOfPlayerAgain +0x49, GateClosed +0x4a, OpenGates +0x58, FirstSetup +0x59, SoundPlayed +0x5c, StopWhistling +0x60, BanditsKilled +0x61, IsBanditInRange +0x62, BanditInRange +0x64, BanditsStopChasing +0x70, NoPersistence +0x74.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
class CGameScriptInterfaceBase;
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OakValeBanditRaidScript : public CScriptBase {
public:
    virtual void Init();
    CGameScriptInterfaceBase* Game;                     // +0x40 (the ctor's CGameScriptInterfaceBase&; descriptive name)
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44 (the ctor's CScriptDataBase*; descriptive name)
    bool BanditsAwareOfPlayer;                          // +0x48
    bool BanditsAwareOfPlayerAgain;                     // +0x49
    bool GateClosed;                                    // +0x4a
    unsigned char _pad04b[0xd];
    bool OpenGates;                                     // +0x58
    bool FirstSetup;                                    // +0x59
    unsigned char _pad05a[0x2];
    long SoundPlayed;                                   // +0x5c
    bool StopWhistling;                                 // +0x60
    bool BanditsKilled;                                 // +0x61
    bool IsBanditInRange;                               // +0x62
    unsigned char _pad063[0xd];
    long BanditsStopChasing;                            // +0x70
    bool NoPersistence;                                 // +0x74
};
}

void NScript::CQ_OakValeBanditRaidScript::Init()
{
    GateClosed = false;
    OpenGates = false;
    BanditsAwareOfPlayerAgain = false;
    FirstSetup = true;
    SoundPlayed = 0;
    StopWhistling = false;
    BanditsKilled = false;
    IsBanditInRange = false;
    BanditsStopChasing = 0;
    NoPersistence = false;
}