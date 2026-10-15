// Grade: DIFFER(ours v retail) -- 144v140: the lane sinks the `% 120` (xor edx,edx ; div) below the GFGetCurrentFrame call, keeping the rotated seed in ebx; retail divides before the call and keeps the remainder in edi and g_framesPerSecond in ebx
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
// Tried: the single expression (div sunk past the frame call, g_fps as an imul memory operand), these two locals
// (best: only the `% 120` div is sunk below the GFGetCurrentFrame call and the registers rename), a precomputed
// delay local (div before the call but add/imul hoisted too, 136), a frame local (= this).
class CTCBase;

long __fastcall GFGetCurrentFrame();                        // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890
extern long g_framesPerSecond;                              // 0x01375550

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
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

struct CTCIdleScheduler {                       // interface type 0xd3
    void ChooseNewIdle();                       // 0x00801A50
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
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
    unsigned long extraSeconds = Random(120);
    long framesPerSecond = g_framesPerSecond;
    FrameToLook = GFGetCurrentFrame() + (extraSeconds + 240) * framesPerSecond;
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetInterface(&scheduler, 0xd3))
        scheduler->ChooseNewIdle();
}
