// CAIStateGroup_GameMaster::IsRunnable() -- refresh the tavern from the creature's work building; runnable when the hero is not following the creature (AI or region follower), the tavern lives, the creature belongs to a village and the group's own checks pass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094C900, length 384 bytes.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
//          (CIntelligentPointer<CThingBuilding> vtable 0x01244344), CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90,
//          CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap::LowerBound const
//          @ 0x004365B0 (interfaces 0xBC CTCFollowed, 0x7A CTCRegionFollower, 0x23 CTCVillageMember),
//          CTCFollowed::IsFollower @ 0x006D4D70, CTCRegionFollower::IsRegionFollower @ 0x006AEB60, the group's virtual
//          slots 14 (+0x38) and 10 (+0x28, result unused), CTCVillageMember::GetPVillage @ 0x007EE540.
// Shape: the tavern is the intelligent pointer's inline operator= from a temporary built from the raw work building;
//          each interface is the masked-dword HasTC + one-ternary GetTC on the const map into a named component (fetched
//          before the creature argument); the building liveness test is the raw-pointer IS_THING_ALIVE (one GetPItem).
//          A same-TU const LowerBound stand-in lets the keys share the temporary's slot (8-byte frame).
// Layout: CAIStateGroupBase modelled locally (as engine/CAIStateGroupBase.h: vptr +0, PBrain +4, 0x14 bytes) so the virtual
//          slots can be declared; the group's CIntelligentPointer<CThingBuilding> at +0x14 (descriptive);
//          retail CThingAICreature WorkBuilding +0x170 (landed 0x00830360).

class CTCBase;
class CThing;
class CThingBuilding;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0 (stand-in body below)
};
#pragma auto_inline(off)
// Same-TU stand-in for the const CVectorMap::LowerBound (0x004365B0, STLport lower_bound; retail's LTCG saw its body): the
// key's address does not escape, so the three lookup keys share the building temporary's frame slot as in retail.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CBuildingPointer : CBaseIntelligentPointer {          // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    CBuildingPointer(CThingBuilding* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
    CBuildingPointer& operator=(const CBuildingPointer& other) { SetPItem(other.CBaseIntelligentPointer::GetPItem()); return *this; }
    CThingBuilding* GetPItem() const { return (CThingBuilding*)CBaseIntelligentPointer::GetPItem(); }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CThingPlayerCreature : public CThing { };
class CThingAICreature : public CThing { public:
    unsigned char _pad2[0x170 - sizeof(CThing)];
    CBuildingPointer WorkBuilding;              // +0x170
};
class CTCFollowed { public:                            // interface 0xbc
    bool IsFollower(const CThing* thing) const;      // 0x006D4D70 (Ego_r.pdb)
};
class CTCRegionFollower { public:                      // interface 0x7a
    bool IsRegionFollower(CThing* thing);       // 0x006AEB60
};
struct CVillage;
class CTCVillageMember { public:                       // interface 0x23
    CVillage* GetPVillage();                    // 0x007EE540
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
__forceinline bool IS_THING_ALIVE(CThing* const& thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIStateGroupBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool v10();                         // +0x28 (result unused)
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool v14();                         // +0x38
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
};
struct CAIStateGroup_GameMaster : CAIStateGroupBase {
    CBuildingPointer PTavern;                   // +0x14 (descriptive)
    bool IsRunnable();
};

bool CAIStateGroup_GameMaster::IsRunnable()
{
    PTavern = CBuildingPointer(GetCreature()->WorkBuilding.GetPItem());
    CThing* hero = GetPPlayerManager()->GetMainPlayer().GetPControlledCreature();
    if (hero) {
        if (hero->HasTC(0xbc)) {
            CTCFollowed* followed = hero->GetTC<CTCFollowed>(0xbc);
            if (followed->IsFollower(GetCreature()))
                return false;
        }
        if (hero->HasTC(0x7a)) {
            CTCRegionFollower* regionFollower = hero->GetTC<CTCRegionFollower>(0x7a);
            if (regionFollower->IsRegionFollower(GetCreature()))
                return false;
        }
    }
    if (IS_THING_ALIVE(GetCreature()->WorkBuilding.CBaseIntelligentPointer::GetPItem()) && GetCreature()->HasTC(0x23)) {
        CTCVillageMember* member = GetCreature()->GetTC<CTCVillageMember>(0x23);
        if (v14()) {
            v10();
            if (member->GetPVillage())
                return true;
        }
    }
    return false;
}