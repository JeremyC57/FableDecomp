// CAIStateGroup_FollowPlayer::IsRunnable() -- runnable while the main player's creature exists and is alive; remembers it as the thing to follow
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

class CPlayerManager { public:
    CPlayer& GetMainPlayer();                       // 0x00449970
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<..>, vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
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