// CAIStateGroup_MinionCuriosity::IsRunnable() -- runnable when the creature has a perceive component (interface
// 0x34) and FindNewCuriosityPos() (inlined) finds a live curiosity thing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933AE0, length 70 bytes (+ int3 padding).
// Callees: the creature's curiosity-target query @ 0x0082FEC0 (CThing*), CThing::GetPosition @ 0x006A4D00.
// Shape: the interface presence test is the inline bitset test alone (type 0x34 = bit 20 of the dword at
// +0x24, tested on the creature already in ecx as a BRANCH on the raw masked dword), then the FindNewCuriosityPos body inlined from the same
// translation unit: `if (t && !(t->flags91 & 1)) { CuriosityPos = t->GetPosition(); return true; } return
// false;` with the implicit C3DVector operator= (`add esi,14h` + three dword moves) and one shared `xor al,al`.
// Base layout local (PDB names); no engine header for this class.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    const C3DVector& GetPosition();         // 0x006A4D00
};

struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    // raw masked value, not a bool: the caller branches on it (a bool result comes out as `shr ; test dl,1`)
    unsigned long HasInterface(long interfaceType) { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    CThing* GetCuriosityThing();            // 0x0082FEC0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase {
    C3DVector CuriosityPos;                 // +0x14
    bool FindNewCuriosityPos();             // 0x00933AA0 (inlined here)
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionCuriosity::FindNewCuriosityPos()
{
    CThing* thing = PBrain->Creature->GetCuriosityThing();
    if (thing && !(thing->m_flags91 & 1)) {
        CuriosityPos = thing->GetPosition();
        return true;
    }
    return false;
}

bool CAIStateGroup_MinionCuriosity::IsRunnable()
{
    if (PBrain->Creature->HasInterface(0x34))
        return FindNewCuriosityPos();
    return false;
}
