// NScript::CQ_PrisonRaceScript::Init() -- reset the race: flags false, speeds and indices 0, nearest-marker distance 1000, conversation index -1
// Leaf: ?Init@CQ_PrisonRaceScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDAD60, 35 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the quest vtable 0x012DC88C (slot 1 = `NScript::CQ_PrisonRaceScript::RegisterMain`), stored by the quest ctor 0x00DDAD30; ?Init@CQ_PrisonRaceScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00ddad60" is a gap-scan placeholder.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets until the first STL container. HeroWonRace +0x48, RaceStarted +0x49, RaceEnded +0x4a, RacerSpeed +0x4c, HeroWaypointIndex +0x50, NearestRaceMarkerDistanceToHero +0x54, NearestRaceMarker +0x58, ConversationIndex +0x5c.
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
class CBaseClass {                                       // vtable 0x01231710
public:
    CBaseClass() { }
    CBaseClass(const CBaseClass&) { }
    virtual ~CBaseClass();                                // 0x0099A2E0
};
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
class CScriptBase {                                      // retail 0x40 bytes (Ego_r.pdb 0x54 with Dinkumware containers)
public:
    virtual ~CScriptBase();
    virtual void RegisterMain();                          // slot 1
    virtual void Main();                                  // slot 2
    virtual void Init();                                  // slot 3 (+0x0c)
    unsigned char _body[0x3c];
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonRaceScript : public CScriptBase {
public:
    virtual void Init();
    CGameScriptInterfaceBase* Game;                     // +0x40 (the ctor's CGameScriptInterfaceBase&; descriptive name)
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44 (the ctor's CScriptDataBase*; descriptive name)
    bool HeroWonRace;                                   // +0x48
    bool RaceStarted;                                   // +0x49
    bool RaceEnded;                                     // +0x4a
    unsigned char _pad04b[0x1];
    long RacerSpeed;                                    // +0x4c
    long HeroWaypointIndex;                             // +0x50
    float NearestRaceMarkerDistanceToHero;              // +0x54
    long NearestRaceMarker;                             // +0x58
    long ConversationIndex;                             // +0x5c
};
}

void NScript::CQ_PrisonRaceScript::Init()
{
    HeroWonRace = false;
    RaceStarted = false;
    RaceEnded = false;
    RacerSpeed = 0;
    HeroWaypointIndex = 0;
    NearestRaceMarkerDistanceToHero = 1000.0f;
    NearestRaceMarker = 0;
    ConversationIndex = -1;
}