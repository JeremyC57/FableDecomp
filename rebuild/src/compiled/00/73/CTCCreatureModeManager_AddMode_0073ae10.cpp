// CTCCreatureModeManager::AddMode(long modeType) -- allocate a mode of the given type and add it (by-value counted pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AE10, length 92 bytes (+4 int3). Called by SetupDefaultMode
// (0x0073AF60) with the def's DefaultCreatureMode.
// Callees: CTCCreatureModeManager::AllocateMode(long) @ 0x0073A8E0 (returns CCountedPointer<CModeBase> through the hidden
// pointer = the 8-byte frame local), CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0 (by-value
// argument, destroyed by the callee), operator delete @ 0x00BFE9BC.
// Shape: the by-value argument is copy-constructed in the argument area (`sub esp,8 ; mov eax,esp`, the count block
// add-ref'd with `inc [esi]`); the bool result rides in bl across the local's destructor. The count block {references,
// __fastcall destroy function, object} is the CCountedPointer layout of the landed Reset family (0x00836580): the release
// is `--references == 0` -> destroy(object) -> delete block, with the block pointer cached in esi from the copy.
// AllocateMode gets a same-TU `auto_inline(off)` stand-in body that writes only the returned pointer: VC7.1 then knows
//   the local's address (the hidden return slot) does not escape, so AddMode cannot change it and the count block stays
//   in esi across the call, as retail (an extern declaration re-reads the slot after AddMode).
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CModeBase;

struct CModePointer {                       // CCountedPointer<CTCCreatureModeManager::CModeBase>
    CModeBase* m_pObject;                   // +0
    SCountBlock* m_pCount;                  // +4
    CModePointer(CModeBase* object, SCountBlock* count) : m_pObject(object), m_pCount(count) {}
    CModePointer(const CModePointer& other)
    {
        SCountBlock* count = other.m_pCount;
        m_pObject = other.m_pObject;
        m_pCount = count;
        if (count)
            ++count->m_references;
    }
    ~CModePointer()
    {
        SCountBlock* count = m_pCount;
        if (count) {
            if (--count->m_references == 0) {
                count->m_pfnDestroy(count->m_pObject);
                delete count;
            }
        }
    }
};

class CThing;

class CTCCreatureModeManager { public:
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CModePointer AllocateMode(long modeType);           // 0x0073A8E0
    bool AddMode(CModePointer mode);                    // 0x0073ACC0
    bool AddMode(long modeType);
};

CModeBase* AllocateModeObject_standin(long modeType);   // stand-in callees of the stand-in body below
SCountBlock* AllocateModeCount_standin();
// Stand-in body (the real AllocateMode is out of line in retail): it writes only the returned pointer, so VC7.1
// knows the local's address does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) CModePointer CTCCreatureModeManager::AllocateMode(long modeType)
{
    return CModePointer(AllocateModeObject_standin(modeType), AllocateModeCount_standin());
}
#pragma auto_inline(on)

bool CTCCreatureModeManager::AddMode(long modeType)
{
    CModePointer mode = AllocateMode(modeType);
    return AddMode(mode);
}