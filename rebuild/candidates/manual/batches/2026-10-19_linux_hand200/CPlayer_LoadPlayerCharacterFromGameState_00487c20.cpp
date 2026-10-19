// CPlayer::LoadPlayerCharacterFromGameState(const CPlayerSave& save) -- after a load: bring in the saved character's region, find the character by UID, move the player's map position onto it and adopt it as the player character
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487C20, 192 bytes (+ int3 padding), `ret 4`.
// Callees: the world's GetWorldMap (vtable slot 12, +0x30, three times) and GetThingManager (slot 3, +0xC),
//   CWorldMap::GetRegionNumberFromName(const CCharString&) 0x004FC210, CWorldMap::LoadRegion 0x00500540,
//   CThingManager::GetThingWithUID(__int64) 0x0051ED80 (catalogued "CStopWatch::GetTicks"), CThing::GetPosition 0x004C73D0
//   (catalogued "CVertexBufferWin32::DoSizeof"), CWorldMap::SetPlayerPos(long, const C3DVector&, ELoadType) 0x005063E0,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10.
// Shape: CPlayerSave (Ego_r.pdb: CharacterUID ulong64 +0, RegionName CCharString +8). Every world-map call goes back
//   through the world's virtual GetWorldMap (the arguments are pushed first, then the vtable read). The success path is nested
//   (`if (region != 0) { ... if (thing != 0) { ...; return true; } } return false;`) so both failures share the one
//   short epilogue at the end. The adopt step is
//   `PlayerCharacter = CIntelligentPointer<CThingPlayerCreature>(thing)`: a temporary intelligent pointer (vtable
//   0x01231C4C) built in the frame, copied with the inline operator= (`SetPItem(other.GetPItem())`) and destroyed. The
//   world is held in ebx for the SetPlayerPos call: VC reads `World` before evaluating the GetPosition argument.
//   Members (World +0x1C, Number +0x28, PlayerCharacter +0x34) from engine/CPlayer.h.
#include "engine/CPlayer.h"

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
};
struct CPlayerSave {
    unsigned __int64 CharacterUID;                          // +0x00
    CCharString      RegionName;                            // +0x08
};

struct C3DVector;
struct CBaseObject;
struct CThing {
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CThingPlayerCreature;
struct CThingManager {
    CThing* GetThingWithUID(__int64 uid);                   // 0x0051ED80
};

typedef long ELoadType;
struct CWorldMap {
    long GetRegionNumberFromName(const CCharString& name) const;          // 0x004FC210
    void LoadRegion(long region, bool keepOthers, bool immediately);      // 0x00500540
    void SetPlayerPos(long player, const C3DVector& position, ELoadType loadType);   // 0x005063E0
};

struct CWorldCalls {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();               // slot 3 (+0xC)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                       // slot 12 (+0x30)
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01231C4C for CThingPlayerCreature
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CPlayer_Methods : CPlayer {
    CWorldCalls* GetWorld() { return reinterpret_cast<CWorldCalls*>(World); }
    CIntelligentPointer<CThingPlayerCreature>& PlayerCharacterPointer() { return *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(this->PlayerCharacter); }
    bool LoadPlayerCharacterFromGameState(const CPlayerSave& save);
};

bool CPlayer_Methods::LoadPlayerCharacterFromGameState(const CPlayerSave& save)
{
    long region = GetWorld()->GetWorldMap()->GetRegionNumberFromName(save.RegionName);
    if (region != 0) {
        GetWorld()->GetWorldMap()->LoadRegion(region, false, true);
        CThing* thing = GetWorld()->GetThingManager()->GetThingWithUID(save.CharacterUID);
        if (thing != 0) {
            GetWorld()->GetWorldMap()->SetPlayerPos(Number, thing->GetPosition(), 0);
            PlayerCharacterPointer() = CIntelligentPointer<CThingPlayerCreature>((CThingPlayerCreature*)thing);
            return true;
        }
    }
    return false;
}
