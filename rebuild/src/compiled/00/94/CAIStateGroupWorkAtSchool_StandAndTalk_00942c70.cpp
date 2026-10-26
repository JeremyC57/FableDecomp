// CAIStateGroup_WorkAtSchool::StandAndTalk(float seconds) -- put the creature's mode manager (interface 0x31) into
// mode 0x1B, idle until the given time has passed (or the group stops / the idle action ends), then leave the
// mode; true when the idle action finished (2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00942C70, length 193 bytes (+ int3 padding), `ret 4`.
// Callees: GFGetCurrentFrame @ 0x0049D870 (__fastcall, twice; manifest "GFGetPreMainMemoryUsed" propagated),
// _ftol @ 0x00BFEA70, CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface, 0x31 = bit 17 of
// +0x24; the miss path reads the never-written slot), CTCCreatureModeManager::AddMode @ 0x0073AE10 / RemoveMode
// @ 0x0073ABA0, CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, IsStopping @ 0x008FD010; the frame-rate
// global @ 0x01375550.
// Shape: the frame rate is read into a local before the frame call (both go to fild slots); the end frame is
// `(long)(now + fps * seconds)`; the lookup key and the component share the dead `seconds` argument slot; the wait
// is a while loop on `result == 4 && frame <= end && !IsStopping()`; the result bool is full-register `sete`.
// Local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0 / 0x0040F020 (byte-identical instantiations)
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

extern long g_FramesPerSecond;                  // 0x01375550
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870

class CTCCreatureModeManager { public:                 // interface 0x31
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

struct CAIStateGroupBaseA : CAIStateGroupBase {
    long AIAction_DoNothing();                  // 0x0097E200
    bool IsStopping();                          // 0x008FD010
};

struct CAIStateGroup_WorkAtSchool : CAIStateGroupBaseA {
    bool StandAndTalk(float seconds);
};

bool CAIStateGroup_WorkAtSchool::StandAndTalk(float seconds)
{
    long fps = g_FramesPerSecond;
    long endFrame = (long)(CWorld::GetFrame() + fps * seconds);
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    modes->AddMode(0x1b);
    long result = AIAction_DoNothing();
    while (result == 4 && CWorld::GetFrame() <= endFrame && !IsStopping())
        result = AIAction_DoNothing();
    modes->RemoveMode(0x1b);
    return result == 2;
}