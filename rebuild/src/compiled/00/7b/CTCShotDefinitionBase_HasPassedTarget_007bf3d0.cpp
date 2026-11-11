// CTCShotDefinitionBase::HasPassedTarget() const -- true when the shot's last step (physics Position - OldPosition) points away from the target (dot with TargetPos - Position below zero)
// Leaf: HasPassedTarget
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe 0x007BF3D0, 139 bytes (plain `ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (inline GetInterface for TC_PHYSICS,
// interface 2 = bit 2 of +0x20; the uninitialised out pointer shares the frame word with the lookup key, as the
// landed CTCDegradableBase::ChangeMesh 0x008EC9C0). Global 0.0f @ 0x0122DEDC.
// Layout: retail CTCShotDefinitionBase sits 4 bytes below the PDB after the HitThingsList vector: TargetPos +0x51
// (PDB 0x55, packed C3DVector at an odd offset); CTCPhysicsBase (PDB): Position +0x0C, OldPosition +0x18.
#include "fable_compiler.h"
#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector Position;                             // +0x0c
    C3DVector OldPosition;                          // +0x18
};
FABLE_FORCEINLINE CTCPhysicsBase* GetPhysics(CThing* thing)
{
    CTCPhysicsBase* physics;
    thing->GetInterface(&physics, 2);               // TC_PHYSICS
    return physics;
}
#pragma pack(push, 1)
struct CTCShotDefinitionBase {
    void*         __vftable;                        // +0x00
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x51 - 0x08];
    C3DVector     TargetPos;                        // +0x51
    bool HasPassedTarget() const;
};
#pragma pack(pop)

bool CTCShotDefinitionBase::HasPassedTarget() const
{
    CTCPhysicsBase* physics = GetPhysics(ParentThing);
    C3DVector moved = physics->Position - physics->OldPosition;
    C3DVector toTarget = TargetPos - physics->Position;
    if (toTarget.Dot(moved) < 0.0f)
        return true;
    return false;
}