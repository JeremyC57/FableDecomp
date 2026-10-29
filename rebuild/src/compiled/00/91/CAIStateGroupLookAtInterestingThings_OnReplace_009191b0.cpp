// Retry 10b (B-12 sweep): the random delay is bound to a const REFERENCE (`const unsigned long& extraSeconds = Random(120);`, an address-taken temporary): the `% 120` div stays before the GFGetCurrentFrame call with the remainder in edi and the frame rate in ebx (a value local lets VC7.1 sink the div below the call).
// CAIStateGroup_LookAtInterestingThings::OnReplace() -- schedule the next look 240..359 seconds from now and let the idle scheduler choose a new idle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009191B0, length 140 bytes (+ int3 padding). Vtable slot 10 of
// CAIStateGroup_LookAtInterestingThings (0x01289BBC).
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890 (__fastcall), GFGetCurrentFrame
// @ 0x0049D870 (__fastcall), CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0xd3 = the idle
// scheduler), CTCIdleScheduler::ChooseNewIdle @ 0x00801A50. Global: the frame-rate long @ 0x01375550.
//
// Shape: `FrameToLook = GFGetCurrentFrame() + (Random(120) + 240) * g_framesPerSecond` -- the right operand (the
// inlined seed step `seed = GFROR13(seed*0x24A1 + 0x24DF)`, `% 120` as an unsigned `div` by a constant range so
// no zero test, the frame rate held in ebx) is evaluated before the frame call. Then the out-parameter
// GetInterface whose result guards the ChooseNewIdle call (component loaded straight into ecx).
// PDB: FrameToLook +0x14. The engine header CAIStateGroup_LookAtInterestingThings.h is flattened data-only:
// local model.
// Earlier forms: the single expression and value locals sink the div past the frame call.
class CTCBase;

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                        // 0x0049D870
long __fastcall GFROR13(unsigned long);      // 0x00497890
extern long g_framesPerSecond;                              // 0x01375550

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCIdleScheduler { public:                       // interface type 0xd3
    void ChooseNewIdle();                       // 0x00801A50
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        if (range != 0)
            return rotated % range;
        return 0;
    }
    virtual void OnReplace();
};

struct CAIStateGroup_LookAtInterestingThings : CAIStateGroupBase {
    long FrameToLook;                           // +0x14
    virtual void OnReplace();
};

void CAIStateGroup_LookAtInterestingThings::OnReplace()
{
    const unsigned long& extraSeconds = Random(120);
    long framesPerSecond = g_framesPerSecond;
    FrameToLook = CWorld::GetFrame() + (extraSeconds + 240) * framesPerSecond;
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetInterface(&scheduler, 0xd3))
        scheduler->ChooseNewIdle();
}