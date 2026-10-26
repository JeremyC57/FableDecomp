// CTCCoopSpirit::UpdateAttractionToMaster() -- once the spirit has not moved for a while, pull it towards its master's hero: an acceleration along the offset, scaled by how far beyond 1.5 units it is (capped at 2.5) times 1.027
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006701A0, 312 bytes (+ int3 padding), plain `ret`.
// Callees: __ftol2 @ 0x00BFEA70, CPlayer::GetHeroThing @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0 (x2),
//   C3DVector::GetLength @ 0x00A14480 (manifest "C3DVector::GetScaled" is propagated), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of the physics TC, interface 2 = word 0 bit 2),
//   CTCPhysicsBase::ApplyAcceleration(const C3DVector&) @ 0x006B0210.
// Globals: g_framesPerSecond @ 0x01375550, the idle seconds before attraction 3.0f @ 0x0137EA20 (an extern float
//   variable, named descriptively), 1.5f @ 0x0125AADC, 2.5f @ 0x01241D68, 1.027f @ 0x0125B250.
// Layout = Ego_r.pdb CTCCoopSpirit: ParentThing +0x04, NoMovementTimer +0x0c, Master (CPlayer*) +0x24.
// Shape: the master's position is copied into a named C3DVector (three dword moves) before the spirit's own position is
//   read; the offset is a named C3DVector built by `masterPos - GetPosition()` where GetPosition is an inline BY-VALUE
//   accessor and C3DVector has a memberwise user copy ctor (the temporary keeps X/Y on the FPU and copies Z as a dword
//   into a dead frame word, the chunk-138 shape; without the copy ctor the copy is three dword moves, through the
//   reference the subtraction folds into memory operands); the distance a named float (fst); the physics TC
//   through an inline accessor returning an unset local when absent; the excess is clamped with `if (f > max) f = max`
//   and the acceleration is a named `offset * f` vector in the master position's frame words.
extern long g_framesPerSecond;                  // 0x01375550
extern float g_CoopSpiritAttractionDelay;       // 0x0137EA20 (3.0f)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    float GetLength() const;                    // 0x00A14480
};
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                            // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CTCPhysicsBase { public:
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
class CPlayer {
public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
class CTCCoopSpirit { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    long    NoMovementTimer;                    // +0x0c
    long    Score;                              // +0x10
    long    OffscreenCounter;                   // +0x14
    C3DVector MovementVector;                   // +0x18
    CPlayer* Master;                            // +0x24
    C3DVector GetPosition() const { return ParentThing->GetPosition(); }   // by-value inline accessor
    void UpdateAttractionToMaster();
};

void CTCCoopSpirit::UpdateAttractionToMaster()
{
    CPlayer* master = Master;
    if (master && NoMovementTimer > (long)(g_framesPerSecond * g_CoopSpiritAttractionDelay)) {
        CThing* hero = master->GetHeroThing();
        if (hero) {
            C3DVector masterPos = hero->GetPosition();
            C3DVector offset = masterPos - GetPosition();
            float distance = offset.GetLength();
            if (distance > 1.5f) {
                CTCPhysicsBase* physics = (CTCPhysicsBase*)ParentThing->GetTCIfPresent(2);
                float pull = distance - 1.5f;
                if (pull > 2.5f)
                    pull = 2.5f;
                C3DVector acceleration = offset * (pull * 1.027f);
                physics->ApplyAcceleration(acceleration);
            }
        }
    }
}