// CAIStateGroup_UsesSeat::IsStillValid() -- the seat state stays valid while the creature (when seated) is
// within sqrt(0.1) of its seat position and its mode component (interface 0x42) is in neither mode 1 nor mode 6.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00938960, length 168 bytes (+ int3 padding).
// Callees: CThing::GetPos @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated),
// CVectorMap::LowerBound @ 0x004365B0 (inlined out-parameter GetInterface, 0x42 = bit 2 of +0x28; the miss path
// reads the never-written slot), the component's HasMode(long) @ 0x00811340 (twice). Float 0.1f @ 0x0122EE58.
// Shape: the seat delta stays on the x87 stack; the squared length is accumulated in retail's order
// (`s = z*z; s += y*y; s += x*x`, chunk-177 lesson) and compared against the literal; shared `xor al,al` tail.
// No engine header for this class; members named descriptively (+0x1d seated flag, +0x24 seat position).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class C3DVector { public: float X, Y, Z; };

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

class CTCCreatureModeManager { public:                 // interface 0x42
    bool HasMode_00811340(long mode);                    // 0x00811340 (manifest "CPlayer::HasMode", propagated)
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
        const C3DVector& pos = PBrain->PCreature->GetPos();
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
    if (modes->HasMode_00811340(1) || modes->HasMode_00811340(6))
        return false;
    return true;
}