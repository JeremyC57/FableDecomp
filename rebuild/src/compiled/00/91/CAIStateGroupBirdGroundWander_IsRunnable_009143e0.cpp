// CAIStateGroup_BirdGroundWander::IsRunnable() -- runnable when the creature's mode component is in the ground-wander mode and the bird never walked or finished walking more than two seconds ago
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009143E0, length 126 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 7 =
// bitset word 0 bit 7 -> `mov cl,[eax+20h] ; test cl,cl ; jns`), the component's mode getter @ 0x006CB6A0
// (`movzx` of [+0xc]->+0x10; the manifest's CIsHitEventBlocked::operator() label is propagated),
// CAIStateGroupBase::GetPWorld @ 0x008FCFA0 (result unused: object expression of a static call),
// CWorld::GetFrame @ 0x0049D870. Globals: the compared mode @ 0x0129CBA4, frames per second @ 0x01375550.
// Shape: the MazeTeleport (0x009543B0) form -- the lookup's bool is not tested, so the miss path reads the
// uninitialised out slot (`mov eax,[esp+8]`); `FrameLastFinishedWalking == 0 ||` short-circuits to the shared
// `mov al,1`; the frame rate is read into a local (esi) before the frame call and doubled with `lea`, the elapsed
// frames compared unsigned (`jbe`).
// Model: local (engine/CAIStateGroup_BirdGroundWander.h agrees: FrameLastFinishedWalking +0x14, but flattens the base).
class CTCBase;

extern long g_framesPerSecond;                  // 0x01375550
extern long g_groundWanderMode;                 // 0x0129CBA4

class CWorld { public:
    static long GetFramesPerSecond() { return g_framesPerSecond; }
public:
    static long __fastcall GetFrame();   // 0x0049D870
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

class CTCCreatureModeManager { public:                 // interface 7
    long GetCurrentModeType();                  // 0x006CB6A0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
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
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
};

struct CAIStateGroup_BirdGroundWander : CAIStateGroupBase {
    unsigned long FrameLastFinishedWalking;     // +0x14
    bool IsRunnable();
};

bool CAIStateGroup_BirdGroundWander::IsRunnable()
{
    CTCCreatureModeManager* modes;
    PBrain->Creature->GetInterface(&modes, 7);
    if (modes->GetCurrentModeType() == g_groundWanderMode) {
        if (FrameLastFinishedWalking == 0)
            return true;
        long framesPerSecond = PeekWorld().GetFramesPerSecond();
        if (CWorld::GetFrame() - FrameLastFinishedWalking > (unsigned long)(framesPerSecond * 2))
            return true;
    }
    return false;
}