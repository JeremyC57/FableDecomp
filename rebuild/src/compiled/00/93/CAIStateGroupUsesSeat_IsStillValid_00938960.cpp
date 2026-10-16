// CAIStateGroup_UsesSeat::IsStillValid() -- the seat state stays valid while the creature (when seated) is
// within sqrt(0.1) of its seat position and its mode component (interface 0x42) is in neither mode 1 nor mode 6.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00938960, length 168 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated),
// CVectorMap::LowerBound @ 0x004365B0 (inlined out-parameter GetInterface, 0x42 = bit 2 of +0x28; the miss path
// reads the never-written slot), the component's HasMode(long) @ 0x00811340 (twice). Float 0.1f @ 0x0122EE58.
// Shape: the seat delta stays on the x87 stack; the squared length is accumulated in retail's order
// (`s = z*z; s += y*y; s += x*x`, chunk-177 lesson) and compared against the literal; shared `xor al,al` tail.
// No engine header for this class; members named descriptively (+0x1d seated flag, +0x24 seat position).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct C3DVector { float X, Y, Z; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CTCCreatureModeManager {                 // interface 0x42
    bool HasMode(long mode);                    // 0x00811340 (manifest "CPlayer::HasMode", propagated)
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    unsigned char _pad_0x14[9];
    bool m_seated;                              // +0x1d
    unsigned char _pad_0x1e[6];
    C3DVector m_seatPos;                        // +0x24
    virtual bool IsStillValid();
};

bool CAIStateGroup_UsesSeat::IsStillValid()
{
    if (m_seated) {
        const C3DVector& pos = PBrain->PCreature->GetPosition();
        float dx = m_seatPos.X - pos.X;
        float dy = m_seatPos.Y - pos.Y;
        float dz = m_seatPos.Z - pos.Z;
        float s = dz * dz;
        s += dy * dy;
        s += dx * dx;
        if (s > 0.1f)
            return false;
    }
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x42);
    if (modes->HasMode(1) || modes->HasMode(6))
        return false;
    return true;
}