// CAIStateGroup_MinionGuardFollowPlayer::IsStillValidInternal(bool) -- a guard keeps following the live player only while the player is inside a building (thing type 3) and the guard belongs to a village: a closed shop must carry interface 0x53, any other building must not and must have an owned sim definition; then the village's follow frame must have come and a building whose 0xD5 component is flagged must carry interface 0x53
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090C2D0, length 612 bytes (+ int3 padding), `ret 4` (virtual).
// Callees: CWorld::GetCurrentPlayer @ 0x0049D9D0 (the main game component @ 0x013B86A0, world at +0x24), CPlayer::GetPControlledCreature
// @ 0x00487DC0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0x75 on the player, 0x23 CTCVillageMember, 0xBD the
// shop sign, 0xD5 on the building), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCVillageMember::GetPVillage @ 0x007EE540
// (twice; manifest "IS_THING_ALIVE" is wrong), CThingBuilding::PeekPSimDefinition @ 0x0082D2D0 (CDefPointer by hidden pointer,
// two conditional temporaries; manifest "PeekPMaterial" is the propagated twin label), the def's virtual slot 1, GetPWorld @
// 0x008FCFC0 (evaluated for the static frame getter), CWorld::GetWorldFrame @ 0x0049D870.
// Layout: no engine header for this class; the player's 0x75 component +0x0c building pointer, the village's +0xc8 frame,
// the sim definition's +0x44 owner and the 0xD5 component's +0x11 flag are descriptive; CThing +0x10 is the thing type.
// Shape: the landed CIsHouseLargerThan / LightHomeCandle CDefPointer model (inline ReleaseTemporary over a CRefCounted at +4).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte entries).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char ThingType;                            // +0x10 (3: building)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
struct CRefCounted {                                    // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CBuildingSimDefinition {
    virtual void v00();
    virtual void Destroy();                             // slot 1 (+0x04)
    CRefCounted Ref;                                    // +0x04
    unsigned char _pad08[0x44 - 0x08];
    void* Owner;                                        // +0x44 (descriptive)
    __forceinline void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    T* operator->() const { return Object; }
};
class CThingBuilding : public CThing { public:
    CDefPointer<CBuildingSimDefinition> PeekPSimDefinition() const;   // 0x0082D2D0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
__forceinline bool IsAlive(const CThing* thing)
{
    if (thing && !(thing->m_flags91 & 1))
        return true;
    return false;
}
struct CTCPlayerBuilding {                              // interface 0x75 on the player (descriptive)
    unsigned char _pad[0x0c];
    CIntelligentPointer<CThing> PBuilding;                  // +0x0c
};
struct CTCShopSign {                                    // interface 0xbd
    unsigned char _pad[0x0c];
    bool m_closed;                                      // +0x0c
};
struct CTCBuildingGuarded {                             // interface 0xd5 (descriptive)
    unsigned char _pad[0x11];
    bool Guarded;                                       // +0x11
};
struct CVillage {
    unsigned char _pad[0xc8];
    long NextFollowFrame;                               // +0xc8 (descriptive)
};
class CTCVillageMember { public:                               // interface 0x23
    CVillage* GetPVillage() const;                      // 0x007EE540
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                            // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;                  // 0x0049D9D0
    static long __fastcall GetFrame();                        // 0x0049D870
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld* PWorld;                                     // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                                  // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                   // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                                // 0x008FCFC0
};
struct CAIStateGroup_MinionGuardFollowPlayer : CAIStateGroupBase {
    virtual bool IsStillValidInternal(bool flag);
};

bool CAIStateGroup_MinionGuardFollowPlayer::IsStillValidInternal(bool)
{
    CThing* player = g_pMainGameComponent->PWorld->GetCurrentPlayer()->GetPControlledCreature();
    if (player && !(player->m_flags91 & 1)) {
        CTCPlayerBuilding* inside;
        if (player->GetInterface(&inside, 0x75) && IsAlive(inside->PBuilding.GetPItem())) {
            CThingBuilding* building = (CThingBuilding*)inside->PBuilding.GetPItem();
            CTCVillageMember* member;
            if (building->ThingType == 3 && GetCreature()->GetInterface(&member, 0x23) && member->GetPVillage()) {
                CVillage* village = member->GetPVillage();
                CTCShopSign* sign;
                if (building->GetInterface(&sign, 0xbd) && sign->m_closed) {
                    if (!building->HasTC(0x53))
                        return false;
                } else {
                    if (building->HasTC(0x53))
                        return false;
                    bool unowned = !building->PeekPSimDefinition().Object || !building->PeekPSimDefinition()->Owner;
                    if (unowned)
                        return false;
                }
                CWorld* world = GetPWorld();
                long nextFrame = village->NextFollowFrame;
                if (world->GetFrame() < nextFrame)
                    return false;
                CTCBuildingGuarded* guarded;
                if (building->GetInterface(&guarded, 0xd5) && guarded->Guarded && !building->HasTC(0x53))
                    return false;
                return true;
            }
        }
    }
    return false;
}