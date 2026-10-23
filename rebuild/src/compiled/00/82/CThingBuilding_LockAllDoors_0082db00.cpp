// CThingBuilding::LockAllDoors(bool lock, bool checkBlocking) -- walks the building's door list and opens or closes
// (locks) every live door thing whose door component is lockable by its owner; a door the main player is standing near is
// left alone (unless the player is inside this building), and a closing door that would block the player stays open
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082DB00, `ret 8`.
// Layout: CThing presence bits +0x20, Components +0x44, Dead +0x91 bit 0; door-list component keeps its list at +0x0C;
//         CTCDoor +0x0C owner-lock word (descriptive).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing;
struct CIntelligentPointer {
    unsigned char _pad00[8];
    CThing* GetPItem() const;                        // 0x00A01B50
};
struct SDoorNode {
    SDoorNode*          _M_next;
    SDoorNode*          _M_prev;
    CIntelligentPointer _M_data;                     // +0x08
};
struct CTCDoor {                                     // interface type 0x26
    unsigned char _pad00[0x0c];
    long          OwnerLock;                         // +0x0c (descriptive)
    bool IsLockableByOwner();                        // 0x00730610
    bool IsClosed();                                 // 0x0082CF40
    void ScriptTriggerOpen(bool open);               // 0x007305D0
};
struct CTCDoorList {                                 // interface type 0x21
    unsigned char _pad00[0x0c];
    SDoorNode*    Doors;                             // +0x0c
};
struct CTCCurrentBuilding {                          // interface type 0x75 (descriptive)
    unsigned char _pad00[0x0c];
    CIntelligentPointer Building;                    // +0x0c
};
struct C3DVector { float X, Y, Z; };
struct CTCSwitchableNavigation {                     // interface type 0x79
    bool WouldBlockPos(const C3DVector& pos);        // 0x00787620
};
struct CPlayer;
struct CThingCreatureBase;
struct CPlayerManager {
    const CPlayer* GetMainPlayer() const;            // 0x00449960
    CPlayer* GetMainPlayer();                        // 0x00449970
};
struct CPlayer {
    const CThingCreatureBase* GetCharacter() const;  // 0x00487DD0
    CThingCreatureBase* GetCharacter();              // 0x00487DC0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                          // +0x91 bit 0
    const CPlayerManager* GetPlayerManager() const;  // 0x004C79F0
    CPlayerManager* GetPlayerManager();              // 0x004C7A00
    const C3DVector& GetPosition() const;            // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(const T** out, long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {};
extern const float G_DoorPlayerRadiusSq;             // 0x0125D108
struct CThingBuilding : CThing {
    void LockAllDoors(bool lock, bool checkBlocking);
};
void CThingBuilding::LockAllDoors(bool lock, bool checkBlocking)
{
    CTCDoorList* doors;
    if (GetInterface(&doors, 0x21)) {
        for (SDoorNode* node = doors->Doors->_M_next; node != doors->Doors; node = node->_M_next) {
            const CIntelligentPointer& door = node->_M_data;
            CTCDoor* tc;
            if (door.GetPItem() && !door.GetPItem()->Dead && door.GetPItem() && !door.GetPItem()->Dead
                && door.GetPItem()->GetInterface(&tc, 0x26)) {
                if (!tc->IsLockableByOwner())
                    continue;
                const CThingCreatureBase* hero = static_cast<const CThingBuilding*>(this)->GetPlayerManager()->GetMainPlayer()->GetCharacter();
                if (hero && !hero->Dead && lock && tc->OwnerLock) {
                    const C3DVector& a = door.GetPItem()->GetPosition();
                    const C3DVector& b = hero->GetPosition();
                    float dx = b.X - a.X;
                    float dy = b.Y - a.Y;
                    float dz = b.Z - a.Z;
                    if (dz * dz + dy * dy + dx * dx < G_DoorPlayerRadiusSq)
                        continue;
                    const CTCCurrentBuilding* current;
                    if (hero->GetInterface(&current, 0x75) && current->Building.GetPItem() == this)
                        continue;
                    tc->OwnerLock = 0;
                }
                if (!tc->IsClosed() && lock && checkBlocking) {
                    CThingCreatureBase* player = GetPlayerManager()->GetMainPlayer()->GetCharacter();
                    CTCSwitchableNavigation* nav;
                    if (door.GetPItem()->GetInterface(&nav, 0x79) && nav->WouldBlockPos(player->GetPosition()))
                        continue;
                }
                tc->ScriptTriggerOpen(!lock);
            }
        }
    }
}