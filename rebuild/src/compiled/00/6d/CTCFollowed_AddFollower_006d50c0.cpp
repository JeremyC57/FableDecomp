// CTCFollowed::AddFollower(CThing& follower, bool aiFollower) -- add a follower (with its emote icon and a GUI refresh) unless already following; an AI creature asked to follow as an AI follower also joins the AI list and is persisted with the game state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D50C0, length 181 bytes (+ int3 padding). __fastcall, `ret 8`.
//         Ego_r.pdb: `?AddFollower@CTCFollowed@@QAEXAAVCThing@@_N@Z`.
// Callees: CTCFollowed::IsFollower(const CThing*) const @ 0x006D4D70, IsAIFollower @ 0x006D4DB0,
//          AddEmoteIcon(CThing&) @ 0x006D4FD0; CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C, CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0);
//          list<CIntelligentPointer<CThing>>::push_back @ 0x004BA4E0, list<CIntelligentPointer<CThingAICreature>>::push_back
//          @ 0x006604EB; CPlayerGui::Refresh(long) @ 0x004378C0 on g_pPlayerGui @ 0x013B8790;
//          CThing::SetAsGameStatePersisted(bool) @ 0x004C7880.
// Shape: each list entry is a converting temporary (inline ctor: base ctor, vtable, SetPItem) passed by reference and
//        destroyed through the base dtor; both temporaries share the 8-byte frame.
// Layout: retail CTCFollowed (STLport 4-byte lists): Followers +0x0C, AIFollowers +0x10; retail CThing +0x10 is the
//         thing class byte (1 = AI creature).
class CThing;
class CBaseObject;
class CThingAICreature;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CIntelligentPointerAICreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>, vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerAICreature(CThingAICreature* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CThingPointerList {                          // retail std::list: one sentinel pointer
    void* head;
    void push_back(const CIntelligentPointer& value);   // 0x004BA4E0
};
struct CAICreaturePointerList {
    void* head;
    void push_back(const CIntelligentPointerAICreature& value);   // 0x006604EB
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
};

class CPlayerGui { public:
    void Refresh(long flag);                        // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

class CTCFollowed { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    CThingPointerList Followers;                    // +0x0C
    CAICreaturePointerList AIFollowers;             // +0x10
    bool IsFollower(const CThing* thing) const;     // 0x006D4D70
    bool IsAIFollower(const CThing* thing) const;   // 0x006D4DB0
    void AddEmoteIcon(CThing& thing);               // 0x006D4FD0
    void AddFollower(CThing& follower, bool aiFollower);
};

void CTCFollowed::AddFollower(CThing& follower, bool aiFollower)
{
    if (!IsFollower(&follower)) {
        Followers.push_back(CIntelligentPointer(&follower));
        AddEmoteIcon(follower);
        g_pPlayerGui->Refresh(0);
    }
    if (aiFollower && follower.ThingClass == 1) {
        if (!IsAIFollower(&follower))
            AIFollowers.push_back(CIntelligentPointerAICreature((CThingAICreature*)&follower));
        follower.SetAsGameStatePersisted(true);
    }
}