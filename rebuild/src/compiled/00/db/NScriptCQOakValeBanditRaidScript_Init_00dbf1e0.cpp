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
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv)
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5..9, 11..17 simplified: not called here)
    virtual void GetPos() const;                          // slot 6
    virtual void GetHomePos() const;                      // slot 7
    virtual void GetCurrentMapName() const;               // slot 8
    virtual void GetHomeMapName() const;                  // slot 9
    virtual float GetAngleXY() const;                     // slot 10 (+0x28)
    virtual void GetPThing() const;                       // slot 11
    virtual void GetPThingUniqueID() const;               // slot 12
    virtual void GetCurrentStateGroupType() const;        // slot 13
    virtual void GetCurrentScriptPriority() const;        // slot 14
    virtual void IsSneaking() const;                      // slot 15
    virtual void IsAwareOfHero() const;                   // slot 16
    virtual void MsgIsTriggeredBy() const;                // slot 17
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};
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