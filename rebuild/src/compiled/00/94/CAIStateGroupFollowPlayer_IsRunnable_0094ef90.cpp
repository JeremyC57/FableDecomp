// CAIStateGroup_FollowPlayer::IsRunnable() -- runnable while the main player's creature exists and is alive; remembers it as the thing to follow
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094EF90, length 111 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90 (PBrain->+0x20; the manifest's
// CPixelShader::CountReferences label is propagated), CPlayerManager::GetMainPlayer @ 0x00449970 (landed name),
// CPlayer::GetPControlledCreature @ 0x00487DC0 (landed name), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
// @ 0x00A01B10/90/50/0x00A01C10.
// Shape: the chained getters into a named local (esi), `hero && !dead` (bit 0 of +0x91) as nested ifs, then
// `PThingToFollow = CIntelligentPointer<..>(hero)` -- a frame temporary through the inline converting constructor
// (vtable 0x01231C4C), the inline operator= and the base destructor; `mov al,1` / shared `xor al,al`.
// Model: local (engine/CAIStateGroup_FollowPlayer.h flattens the base).
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
};
class CThingPlayerCreature;

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;      // 0x00487DC0
};

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<..>, vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
    protected:
    CPlayerManager& GetPlayerManager() const;            // 0x008FCF90
    public:
};

struct CAIStateGroup_FollowPlayer : CAIStateGroupBase {
    CIntelligentPointerThing PThingToFollow;        // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_FollowPlayer::IsRunnable()
{
    CThing* hero = (CThing*)GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero != 0 && !(hero->m_flags91 & 1)) {
        PThingToFollow = CIntelligentPointerThing(hero);
        return true;
    }
    return false;
}