// NScript::CV_TourGuideScript::CTourGuideGuide::Init() -- give the guide information, make it a permanent friend and reset its waypoint and speech indices to -1
// Leaf: ?Init@CTourGuideGuide@CV_TourGuideScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE4C60, 86 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012F511C, stored by the entity Alloc 0x00EE4BB0 (`NScript::CV_TourGuideScript::CTourGuideGuide::Alloc`); ?Init@CTourGuideGuide@CV_TourGuideScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00ee4c60" is a gap-scan placeholder.
// Callees: interface slots 360 SetThingHasInformation, 619 EntitySetPersonalityOverride; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0. Data: "OPINION_PERSONALITY_PERMANENT_FRIEND" @ 0x012F5138.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets: saveWaypointIdx +0x1c, lastRandomSpeechIdx +0x20.
// Shape: the implicit const char* -> CCharString temporary is pushed by a reloaded `lea` (an explicit CCharString(...) would push the ctor's eax).
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
class CEntityScriptBase {                             // Ego_r.pdb, 20 bytes: {vptr, Game +4, CScriptThing Me +8}
public:
    virtual ~CEntityScriptBase();
private:
    virtual void Init();                          // slot 2 (+0x8)
public:
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TourGuideScript {
public:
    class CTourGuideGuide;
};
class CV_TourGuideScript::CTourGuideGuide : public CEntityScriptBase {
public:
    CV_TourGuideScript*     ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    long                    saveWaypointIdx;            // +0x1c
    long                    lastRandomSpeechIdx;        // +0x20
private:
    virtual void Init();
};
}

void NScript::CV_TourGuideScript::CTourGuideGuide::Init()
{
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetPersonalityOverride(Me, "OPINION_PERSONALITY_PERMANENT_FRIEND");
    saveWaypointIdx = -1;
    lastRandomSpeechIdx = -1;
}