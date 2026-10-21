// CTeamSpawn::Init() -- pick a random "EitherTeamSpawn" marker as the other spawn point, make this the bandit or the guard team by Me's script name, and set the member limit and respawn time by whether this is the hero's team
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCD1D0, length 370 bytes (plain `ret`; entity-script slot "Init").
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 / CCharString::operator=(const char*)
//   @ 0x0099EFE0, ~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC (the counted-pointer info block),
//   Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CBasicString<char>::operator==), game
//   interface slot 74 GetRandomThingWithScriptName (ghidra_out/cgsi_slots_aligned.tsv), CScriptThing slot 1 GetName
//   (0x004AA8B0, rebuild/manifest/rtti-vtables.tsv).
// Data: "EitherTeamSpawn" @ 0x012DB30C, "BanditTeamSpawn" @ 0x012DB2AC, "" @ 0x0122D70E, "BanditTeamMember" @ 0x012DB268,
//   "CREATURE_BANDIT_GRUNT" @ 0x012D23A0, "GuardTeamMember" @ 0x012DB27C, "CREATURE_ORCHARD_FARM_GUARD" @ 0x012DB2F0;
//   g_pScriptDef @ 0x0143E90C whose +0xD6C/+0xD70 longs are GuardReinforcementsYourTeam/EnemyTeam (PDB +0xE1C/+0xE20).
// Layout: Ego_r.pdb CTeamSpawn (0x40, unshifted): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, OtherSpawnPoint +0x1C, TeamID +0x28, TeamMemberLimit +0x2C, TeamRespawnTime +0x30,
//   TeamMemberName +0x34, TeamMemberDefName +0x38, BanditsLeftID +0x3C; the parent CQ_OrchardFarmRaidScript's HeroTeam
//   is retail +0x78 (PDB +0x90 less the narrower STLport vector and string members before it).
// Shape: CCountedPointer::operator= reads the source Info then Data into locals before the self test (retail loads both
//   up front); the inline CCharString ==(const char*) null arm is the strcmp intrinsic over the pooled "" (as landed
//   CV_ChapelOfEvilScript::WatchForExpressions 0x00E6A000).
#include <string.h>

long __fastcall Compare(const char* a, const char* b);   // 0x00411570

#pragma pack(push, 1)
struct CBasicStringChar {                                // CBasicString<char>, 13 bytes
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const { return Compare(PData, text) ? false : true; }
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    const CCharString& operator=(const char* text);      // 0x0099EFE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T*              data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CBaseClass {                                      // vtable 0x01231710
    CBaseClass() {}
    virtual ~CBaseClass();                               // 0x0099A2E0
};

struct CScriptThingImpl;
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImpl> PImp;              // +0x04 Data, +0x08 Info
    ~CScriptThing() {}
    virtual const CCharString& GetName() const;          // slot 1 (+0x04) 0x004AA8B0
};
struct CGameScriptInterface {                 // vtable slots per ghidra_out/cgsi_slots_aligned.tsv
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073();
    virtual CScriptThing GetRandomThingWithScriptName(const CCharString& name) const; // slot 74 (+0x128)
};

struct CScriptDef {
    unsigned char _pad000[0xd6c];
    long GuardReinforcementsYourTeam;                    // +0xd6c (PDB +0xE1C)
    long GuardReinforcementsEnemyTeam;                   // +0xd70 (PDB +0xE20)
};
extern CScriptDef* g_pScriptDef;                         // 0x0143E90C

struct CQ_OrchardFarmRaidScript {
    unsigned char _pad00[0x78];
    long HeroTeam;                                       // +0x78 (PDB +0x90)
};

class CTeamSpawn {
public:
    void*                     __vftable;                 // +0x00
    CGameScriptInterface*     Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
    CQ_OrchardFarmRaidScript* ParentClass;               // +0x14
    void*                     PMasterData;               // +0x18 CQ_SunnyvaleMasterData*
    CScriptThing              OtherSpawnPoint;           // +0x1c
    long                      TeamID;                    // +0x28
    long                      TeamMemberLimit;           // +0x2c
    long                      TeamRespawnTime;           // +0x30
    CCharString               TeamMemberName;            // +0x34
    CCharString               TeamMemberDefName;         // +0x38
    long                      BanditsLeftID;             // +0x3c
    void Init();
};

void CTeamSpawn::Init()
{
    OtherSpawnPoint = Game->GetRandomThingWithScriptName("EitherTeamSpawn");
    if (Me.GetName() == "BanditTeamSpawn") {
        TeamID = 1;
        TeamMemberName = "BanditTeamMember";
        TeamMemberDefName = "CREATURE_BANDIT_GRUNT";
    } else {
        TeamID = 0;
        TeamMemberName = "GuardTeamMember";
        TeamMemberDefName = "CREATURE_ORCHARD_FARM_GUARD";
    }
    if (TeamID == ParentClass->HeroTeam) {
        TeamMemberLimit = 2;
        TeamRespawnTime = g_pScriptDef->GuardReinforcementsYourTeam;
    } else {
        TeamMemberLimit = 3;
        TeamRespawnTime = g_pScriptDef->GuardReinforcementsEnemyTeam;
    }
    BanditsLeftID = 0;
}
