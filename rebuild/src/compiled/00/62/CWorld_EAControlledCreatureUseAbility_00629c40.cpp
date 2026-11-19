// CWorld::EAControlledCreatureUseAbility(const CGameEvent*) -- game-event action: make the event player's hero use the ability from the event (main player only) and flash the GUI attack event
// Leaf: ?EAControlledCreatureUseAbility@CWorld@@QAEXPBVCGameEvent@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Flags: /GS
// Shape: one CPlayer* local holds both GetPlayer results (the mode test, then the hero lookup), which gives retail's
//   callee-saved assignment (event in ebp, creature in edi).
// NF95 2026-11-19 (q11320): the unpacking buffer is Ego_r.pdb CGameEventDataPackage (engine/CGameEventDataPackage.h layout, 0x2C bytes)
//   read through ?GetData@CGameEventDataPackage@@QAEXJPAX@Z 0x009F18F0 (Ego_r.pdb 0x00B454F0 / FableWin.pdb 0x030A2200) and filled by
//   ?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z 0x009F17A0 (Ego_r.pdb 0x00B453F0); were the stand-ins
//   CMemoryBuffer::Read(unsigned long, void*) and CGameEvent::GetData(CMemoryBuffer&).
// NF97 2026-11-19 (port): the read buffer is the shared engine/CGameEventDataPackage.h class (FABLE_RETAIL_CGAMEEVENTDATAPACKAGE:
//   inline ctor zeroing Length / StartPos / EndPos, GetData 0x009F18F0), no longer a local model.
//   The local made-up reader helper is replaced by the FableWin.pdb typed reader (opt-in FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPED_READERS:
//   GetUBYTE / GetUWORD / GetFloat / GetSLONG, expanded in place); a direct GetData read into the local differs.
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPED_READERS
#include "fable_compiler.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#include "engine/CGameEventDataPackage.h"
class CGameEvent {
public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    void GetDataPackage(CGameEventDataPackage* package) const;     // 0x009F17A0
};
class CThingCreatureBase { public:
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    void UseAbility(long ability, float charge);   // 0x00665510
public:
    bool IsBlocking() const;   // 0x00664D00
};
class CThingPlayerCreature : public CThingCreatureBase { };
enum EPlayerMode { PLAYER_MODE_0x2F = 0x2f };
class CPlayer { public:
    bool HasMode(EPlayerMode mode) const;                // 0x00634160
    CThingPlayerCreature* GetPControlledCreature() const;         // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CDefinitionManager { public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void OnEventAttack();                          // 0x004379B0
};
class CWorld { public:
    unsigned char       _pad00[0xc];
    CPlayerManager*     PlayerManager;             // +0x0C
    CDefinitionManager* DefinitionManager;         // +0x10
protected:
    CPlayer* GetPlayer(unsigned long number);               // 0x0049D850 ?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z (Ego_r/FableWin, protected)
public:
    void EAControlledCreatureUseAbility(const CGameEvent* event);
};
void CWorld::EAControlledCreatureUseAbility(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->HasMode(PLAYER_MODE_0x2F))
        return;
    player = GetPlayer(event->Player);
    CThingCreatureBase* creature = player->GetPControlledCreature();
    if (!creature || creature->Dead || creature->IsBlocking())
        return;
    signed char playerNumber = event->Player;
    if (playerNumber != PlayerManager->GetMainPlayerNumber())
        return;
    CGameEventDataPackage buffer;
    event->GetDataPackage(&buffer);
    unsigned short ability = buffer.GetUWORD();
    float charge = buffer.GetFloat();
    creature->UseAbility(ability, 0.0f);
    unsigned short attack = (unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_ATTACK");
    if (attack != 0)
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().OnEventAttack();
}