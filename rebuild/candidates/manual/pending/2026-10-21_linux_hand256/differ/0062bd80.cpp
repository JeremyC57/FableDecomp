// Grade: DIFFER(339v354) -- retail keeps the inlined has-weapon bool materialised (mov al,1 / xor al,al ; test al,al) and overlaps the dead UID slot with the action; the lane jump-threads the bool and lays the frame out differently
// CWorld::EAControlledCreatureStrafe(const CGameEvent* event) -- game-event action: unpack the creature UID (unused),
// a 2D strafe vector and the game action, then apply the relative movement acceleration to the event player's hero
// when it is alive, unless it carries a weapon in its primary slot that it cannot use
// Leaf: EAControlledCreatureStrafe
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062BD80, 345 bytes, `ret 4`, /GS (cookie 0x0139C8A8, check 0x00BFE9F9).
//   PDB ?EAControlledCreatureStrafe@CWorld@@QAEXPBVCGameEvent@@@Z.
// Callees: CGameEvent::GetData(CMemoryBuffer&) 0x009F17A0, CWorld::GetPlayer(long) 0x0049D850, CPlayer::
//   GetHeroCreature 0x00487DC0, CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0 (x4), the const CVectorMap::
//   LowerBound 0x004365B0 (inlined GetInterface, carrying 0x46 and weapon 0x48), CTCCarrying::GetThingInPrimarySlot
//   0x006A1440, CTCWeapon::IsUsableBy(const CThing&) 0x005DAB10, CThingPlayerCreature::
//   ApplyRelativeMovementAcceleration(const C3DVector&, EGameAction) 0x006AD3C0.
// Layout: rebuild/include/engine/CWorld.h (`this` only for the GetPlayer call); CMemoryBuffer / CGameEvent as the
//   landed sibling CWorld::EAMoveCreature 0x006295C0; CThing interface bitset +0x20 / map +0x44 / Dead bit +0x91.
// Shape: the UID's 8-byte slot is reused for the action (the UID is dead); the weapon lookup is an inline helper
//   returning whether a primary-slot weapon was found, the weapon pointer preset to null.
#define CWorld CWorld_PDB
#include "engine/CWorld.h"
#undef CWorld
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
};
struct CGameEvent {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
enum EGameAction { GAME_ACTION_NONE = 0 };
struct CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                         // +0x91 bit 0 Dead
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingPlayerCreature : CThing {
    void ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action);   // 0x006AD3C0
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();               // 0x006A1440
};
struct CTCWeapon {
    bool IsUsableBy(const CThing& thing) const;    // 0x005DAB10
};
__forceinline bool GetPrimaryWeapon(const CThing* creature, CTCWeapon** weapon)
{
    CTCCarrying* carrying;
    if (creature->GetInterface(&carrying, 0x46)) {
        CThing* item = carrying->GetThingInPrimarySlot();
        if (item && item->GetInterface(weapon, 0x48))
            return true;
    }
    return false;
}
struct CPlayer {
    CThing* GetHeroCreature();                     // 0x00487DC0
};
struct CWorld : CWorld_PDB {
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAControlledCreatureStrafe(const CGameEvent* event);
};
void CWorld::EAControlledCreatureStrafe(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    CThingPlayerCreature* creature = (CThingPlayerCreature*)GetPlayer(event->Player)->GetHeroCreature();
    {
        __int64 uid;
        buffer.Read(8, &uid);
    }
    float x;
    buffer.Read(4, &x);
    float y;
    buffer.Read(4, &y);
    EGameAction action;
    buffer.Read(4, &action);
    if (creature && !(creature->Flags91 & 1)) {
        CTCWeapon* weapon = 0;
        if (GetPrimaryWeapon(creature, &weapon)) {
            if (!weapon || !weapon->IsUsableBy(*creature))
                return;
        }
        creature->ApplyRelativeMovementAcceleration(C3DVector(x, y, 0.0f), action);
    }
}
