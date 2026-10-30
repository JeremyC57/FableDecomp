// NScript::CV_SickChildScript::Init() -- reset the sick-child quest state (OwnerAlive = true, the rest false/0)
// Leaf: ?Init@CV_SickChildScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC5420, 22 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the level-script vtable 0x012F17DC (slot 1 = `NScript::CV_SickChildScript::RegisterMain`), stored by 0x00EC53C0; ?Init@CV_SickChildScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00ec5420" is a gap-scan placeholder.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 (and the master data +0x44 where the ctor takes one); the script's own members sit 0x14 below their Ego_r.pdb offsets: FinishedQuest +0x68, GotFishingSpotMushroom +0x69, MotherIntroDone +0x6a, MansLoverState +0x6c, OwnerAlive +0x70, LaughingWomanKilled +0x71.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
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
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv)
public:
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5..17 simplified: not called here)
    virtual void GetPos() const;                          // slot 6
    virtual void GetHomePos() const;                      // slot 7
    virtual void GetCurrentMapName() const;               // slot 8
    virtual void GetHomeMapName() const;                  // slot 9
    virtual float GetAngleXY() const;                     // slot 10
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
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SickChildScript : public CScriptBase {
public:
    virtual void Init();
    CGameScriptInterfaceBase* Game;                     // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    unsigned char seh_Mother[0x10];                     // +0x48 CScriptGameResourceObjectScriptedThingBase
    unsigned char seh_Witch[0x10];                      // +0x58 CScriptGameResourceObjectScriptedThingBase
    bool FinishedQuest;                                 // +0x68
    bool GotFishingSpotMushroom;                        // +0x69
    bool MotherIntroDone;                               // +0x6a
    long MansLoverState;                                // +0x6c
    bool OwnerAlive;                                    // +0x70
    bool LaughingWomanKilled;                           // +0x71
};
}

void NScript::CV_SickChildScript::Init()
{
    FinishedQuest = false;
    GotFishingSpotMushroom = false;
    MotherIntroDone = false;
    MansLoverState = 0;
    OwnerAlive = true;
    LaughingWomanKilled = false;
}