// CTCFollowed::RemoveFollower(CThing& follower) -- drop a follower (and, for an AI creature, its AI-follower entry; a homeless, jobless AI follower also stops being persisted), then remove its emote icon and refresh the GUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D5180, length 218 bytes (+ int3 padding). __fastcall, `ret 4`.
//         Ego_r.pdb: `?RemoveFollower@CTCFollowed@@QAEXAAVCThing@@@Z`.
// Callees: CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C, CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0);
//          list<CIntelligentPointer<CThing>>::remove @ 0x0065DE8A, list<CIntelligentPointer<CThingAICreature>>::remove
//          @ 0x0065DE48; CTCFollowed::IsAIFollower @ 0x006D4DB0, RemoveEmoteIcon(CThing&) @ 0x006D5050;
//          CThing::SetAsGameStatePersisted(bool) @ 0x004C7880; CPlayerGui::Refresh(long) @ 0x004378C0 on g_pPlayerGui.
// Shape: the list entries are converting temporaries passed by reference (the follower reference is read after the
//        first base ctor); the persistence drop needs neither HomeBuilding nor WorkBuilding alive (two inlined
//        single-fetch alive tests) and clears the LevelPersistent bit (`and byte ptr [esi+92h],0EFh`).
// Layout: retail CTCFollowed Followers +0x0C, AIFollowers +0x10 (STLport lists); retail CThing ThingClass +0x10 (1 = AI
//         creature), Dead +0x91 bit 0, LevelPersistent +0x92 bit 4; retail CThingAICreature HomeBuilding +0x168,
//         WorkBuilding +0x170 (PDB +0x190/+0x198).
class CThing;
class CBaseObject;
class CThingAICreature;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

template<class T> struct CRetailList {              // retail std::list: one sentinel pointer
    void* m_head;
    void remove(const T& value);                    // 0x0065DE8A (CThing) / 0x0065DE48 (CThingAICreature)
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    unsigned char _pad11[0x91 - 0x11];
    unsigned char Dead : 1;                         // +0x91 bit 0
    unsigned char _flags91 : 7;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;              // +0x92 bit 4
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
};

class CThingAICreature : public CThing {
public:
    unsigned char _pad93[0x168 - 0x93];
    CBaseIntelligentPointer HomeBuilding;           // +0x168 CIntelligentPointer<CThingBuilding>
    CBaseIntelligentPointer WorkBuilding;           // +0x170
};

__forceinline bool IsThingAlive(const CBaseIntelligentPointer& p)   // one GetPItem per pointer here
{
    CThing* thing = ((CThing*)p.GetPItem());
    if (thing && !thing->Dead)
        return true;
    return false;
}

class CPlayerGui { public:
    void Refresh(long flag);                        // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

class CTCFollowed { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    CRetailList< CIntelligentPointer<CThing> > Followers;              // +0x0C
    CRetailList< CIntelligentPointer<CThingAICreature> > AIFollowers;  // +0x10
    bool IsAIFollower(const CThing* thing) const;   // 0x006D4DB0
    void RemoveEmoteIcon(CThing& thing);            // 0x006D5050
    void RemoveFollower(CThing& follower);
};

void CTCFollowed::RemoveFollower(CThing& follower)
{
    Followers.remove(CIntelligentPointer<CThing>(&follower));
    if (follower.ThingClass == 1) {
        if (IsAIFollower(&follower)) {
            CThingAICreature& creature = (CThingAICreature&)follower;
            if (!IsThingAlive(creature.HomeBuilding) && !IsThingAlive(creature.WorkBuilding)) {
                follower.SetAsGameStatePersisted(false);
                follower.LevelPersistent = false;
            }
        }
        AIFollowers.remove(CIntelligentPointer<CThingAICreature>((CThingAICreature*)&follower));
    }
    RemoveEmoteIcon(follower);
    g_pPlayerGui->Refresh(0);
}