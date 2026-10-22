// Grade: DIFFER(815v815) -- register pair and frame slots only: retail keeps the holy site in esi and the player number in edi (ours swaps them), and lays the frame out holySite/name +0, up +4, forward +0x10, pos +0x1c (ours: holySite +4 sharing up, pos +0x10, forward +0x1c); tried about 30 probes (ctor/member/named rh forms, scope blocks, declaration orders, named number locals)
// CPlayer::CreateCharacter(const CCharString& defName) -- find the character's start holy site (or use the debug start position), place a co-op player one unit off the main player's character, set the world map's player position, create the player creature from the named (or the player def's) character def, set its orientation, and take it as the player character
// Leaf: ?CreateCharacter@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00489D40, length 815 bytes, `ret 4`.
// Callees: CBaseIntelligentPointer GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
// CPlayer::LocateCharacterStartHolySite 0x00488B20, the world's slot 12 GetWorldMap (+0x30) and the map's slot 16 (+0x40,
// position test), CThingCreatureBase-copy GetPosition 0x006A4D00 and CThing::GetPosition 0x004C73D0, physics TC slots 72
// (+0x120 GetRHSet) and 32 (+0x80 SetRHSet), CPlayerManager::GetMainPlayerID 0x00449700 / GetMainPlayer 0x00449970,
// CPlayer +4 getter 0x0099A330 (manifest label GetCanItemBeQuickAssigned is a byte-identical bool getter),
// CWorldMap::SetPlayerPos 0x005063E0, CCharString copy ctor 0x0099EC30 / operator=(char) 0x0099F040 / ~ 0x0099EAE0 /
// (const char*, long) 0x0099EBF0, CBasicString<char>::operator== 0x004115A0, GetDefGlobalIndexFromName 0x009AD410,
// CThingPlayerCreature::Create 0x006AC910 (__fastcall), DisplayProgress 0x009E9F40 (__fastcall).
// Globals: debug start flag 0x013B8647, debug start position 0x013B8650, 1.0f 0x0122DED8, "" 0x0122D70E,
// "CPlayer::CreateCharacter" 0x012380D8, CIntelligentPointer<CThingPlayerCreature> vtable 0x01231C4C.
// Layout: engine/CPlayer.h for PlayerManager +0x10, DefinitionManager +0x18, World +0x1c, Number +0x28, ControlledCreature
// +0x2c, PlayerCharacter +0x34, RespawnInfo +0xe8 (DeadInfoSet +0xc = +0xf4). Retail reads the player def pointer at
// +0x21c where the header (PDB) has PlayerDef_Object at +0x224 (8 bytes of debug-only members precede it); its long at
// +0x3c (PDB CharacterDef +0x40) is passed through operator=(char). Retail CThing PhysicsTC +0x60 (PDB +0x70).
// Shape: holy site read through a block-scoped out local copied to a register local; debug branch builds named up/forward
// vectors; named player-number locals for both GetMainPlayerID compares; def index in a named local before Create.
#include "engine/CPlayer.h"
#include <string.h>

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;           // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(char c);                    // 0x0099F040
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void SetRHSet(const CRightHandedSet& rh);  // slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHSet() const;   // slot 72 (+0x120)
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                         // +0x60
    const C3DVector& GetPosition() const;              // 0x004C73D0
};
struct CThingHolySite : CThing {
    const C3DVector& GetPosition() const;              // 0x006A4D00 (byte-identical copy)
};
struct CThingPlayerCreatureInit {
    CThing* OriginalThing;
    CThing* PreviousThing;
    bool    PlaceAtEndOfList;
};
struct CThingPlayerCreature : CThing {
    static CThingPlayerCreature* __fastcall Create(long def, const C3DVector& pos, long number, CThingPlayerCreatureInit* init);   // 0x006AC910
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
struct CIntelligentPointerPC : CBaseIntelligentPointer {   // vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointerPC(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CWorldMap_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long IsPositionOnMap(const C3DVector* pos);   // slot 16 (+0x40)
    void SetPlayerPos(long number, const C3DVector& pos, long loadType);   // 0x005063E0
};
struct CWorld_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap_Retail* GetWorldMap();           // slot 12 (+0x30)
    unsigned char _pad04[0x174];
    long          PlayerCharacterCreated;              // +0x178
};
struct CGameDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CPlayerDef_Retail {
    unsigned char _pad00[0x3c];
    long          CharacterDef;                        // +0x3c
};

struct CThingHolySite;
struct CPlayer_Methods;
struct CPlayerManager_Retail {
    long GetMainPlayerID() const;                      // 0x00449700
    CPlayer_Methods* GetMainPlayer();                  // 0x00449970
};

extern bool      g_UseDebugStartPos;                   // 0x013B8647
extern C3DVector g_DebugStartPos;                      // 0x013B8650

struct CPlayer_Methods : CPlayer {
    CBaseIntelligentPointer& ControlledCreaturePtr() { return *(CBaseIntelligentPointer*)ControlledCreature; }
    CBaseIntelligentPointer& PlayerCharacterPtr() { return *(CBaseIntelligentPointer*)PlayerCharacter; }
    CPlayerManager_Retail* Manager() { return (CPlayerManager_Retail*)PlayerManager; }
    CWorld_Retail* WorldR() { return (CWorld_Retail*)World; }
    CPlayerDef_Retail* PlayerDef() { return *(CPlayerDef_Retail**)&SuppressFullModeRemoval; }
    bool IsActive() const;                             // 0x0099A330
    bool LocateCharacterStartHolySite(CThingHolySite** site);   // 0x00488B20
    bool CreateCharacter(const CCharString& defName);
};

bool CPlayer_Methods::CreateCharacter(const CCharString& defName)
{
    ControlledCreaturePtr().GetPItem();
    PlayerCharacterPtr().GetPItem();
    C3DVector pos;
    CRightHandedSet rh;
    CThingHolySite* site;
    {
        CThingHolySite* holySite = 0;
        if (!LocateCharacterStartHolySite(&holySite) && !g_UseDebugStartPos)
            return false;
        site = holySite;
    }
    if (g_UseDebugStartPos && WorldR()->GetWorldMap()->IsPositionOnMap(&g_DebugStartPos)) {
        pos = g_DebugStartPos;
        C3DVector up(0.0f, 1.0f, 0.0f);
        C3DVector forward(0.0f, 0.0f, -1.0f);
        rh.Up = up;
        rh.Forward = forward;
    } else {
        pos = site->GetPosition();
        rh = site->PhysicsTC->GetRHSet();
    }
    long number = Number;
    if (number != Manager()->GetMainPlayerID()) {
        if (Manager()->GetMainPlayer()->IsActive() && Manager()->GetMainPlayer()->ControlledCreaturePtr().GetPItem()) {
            const C3DVector& p = static_cast<const CThing*>(Manager()->GetMainPlayer()->ControlledCreaturePtr().GetPItem())->GetPosition();
            pos = C3DVector(p.X + 1.0f, p.Y + 1.0f, p.Z);
        } else {
            const C3DVector& p = site->GetPosition();
            pos = C3DVector(p.X + 1.0f, p.Y + 1.0f, p.Z);
        }
    }
    WorldR()->GetWorldMap()->SetPlayerPos(Number, pos, 0);
    CThingPlayerCreatureInit init;
    init.OriginalThing = 0;
    init.PreviousThing = 0;
    init.PlaceAtEndOfList = false;
    CCharString name(defName);
    if (name == "")
        name = (char)PlayerDef()->CharacterDef;
    long def = ((CGameDefinitionManager_Retail*)DefinitionManager)->GetDefGlobalIndexFromName(name);
    CThingPlayerCreature* creature = CThingPlayerCreature::Create(def, pos, Number, &init);
    DisplayProgress("CPlayer::CreateCharacter", false, -1.0f, false);
    creature->PhysicsTC->SetRHSet(rh);
    PlayerCharacterPtr().SetPItem(CIntelligentPointerPC(creature).GetPItem());
    RespawnInfo[0xc] = false;
    long mainNumber = Number;
    if (mainNumber == Manager()->GetMainPlayerID())
        WorldR()->PlayerCharacterCreated = 1;
    return true;
}
