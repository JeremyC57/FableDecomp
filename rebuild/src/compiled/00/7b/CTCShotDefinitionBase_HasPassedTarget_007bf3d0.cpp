// CTCShotDefinitionBase::HasPassedTarget() const -- true when the shot's last step (physics Position - OldPosition) points away from the target (dot with TargetPos - Position below zero)
// Leaf: HasPassedTarget
// Retail: Fable.exe 0x007BF3D0, 139 bytes (plain `ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (inline GetInterface for TC_PHYSICS,
// interface 2 = bit 2 of +0x20; the uninitialised out pointer shares the frame word with the lookup key, as the
// landed CTCDegradableBase::ChangeMesh 0x008EC9C0). Global 0.0f @ 0x0122DEDC.
// Layout: retail CTCShotDefinitionBase sits 4 bytes below the PDB after the HitThingsList vector: TargetPos +0x51
// (PDB 0x55, packed C3DVector at an odd offset); CTCPhysicsBase (PDB): Position +0x0C, OldPosition +0x18.
#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
};
#pragma pack(pop)
extern const float g_zero_0122dedc;                 // 0x0122DEDC (0.0f)
class CTCBase;
struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
__forceinline CTCPhysicsBase* GetPhysics(CThing* thing)
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
    if (toTarget.Dot(moved) < g_zero_0122dedc)
        return true;
    return false;
}