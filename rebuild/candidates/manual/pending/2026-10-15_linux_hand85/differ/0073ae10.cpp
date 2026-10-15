// CTCCreatureModeManager::AddMode(long modeType) -- allocate a mode of the given type and add it (by-value counted pointer)
// Grade: DIFFER(99v92) -- the local's count block is re-read from its (address-taken) frame slot after the AddMode call
// (`mov eax,[esp+0Ch] ; mov esi,eax ; dec [eax] ; cmp [esi],0`); retail keeps it in esi from the copy (`dec [esi] ; jne`), this in edi.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AE10, length 92 bytes (+4 int3). Called by SetupDefaultMode
// (0x0073AF60) with the def's DefaultCreatureMode.
// Callees: CTCCreatureModeManager::AllocateMode(long) @ 0x0073A8E0 (returns CCountedPointer<CModeBase> through the hidden
// pointer = the 8-byte frame local), CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0 (by-value
// argument, destroyed by the callee), operator delete @ 0x00BFE9BC.
// Shape: the by-value argument is copy-constructed in the argument area (`sub esp,8 ; mov eax,esp`, the count block
// add-ref'd with `inc [esi]`); the bool result rides in bl across the local's destructor. The count block {references,
// __fastcall destroy function, object} is the CCountedPointer layout of the landed Reset family (0x00836580): the release
// is `--references == 0` -> destroy(object) -> delete block, with the block pointer cached in esi from the copy.
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CModeBase;

struct CModePointer {                       // CCountedPointer<CTCCreatureModeManager::CModeBase>
    CModeBase* m_pObject;                   // +0
    SCountBlock* m_pCount;                  // +4
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

struct CThing;

struct CTCCreatureModeManager {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CModePointer AllocateMode(long modeType);           // 0x0073A8E0
    bool AddMode(CModePointer mode);                    // 0x0073ACC0
    bool AddMode(long modeType);
};

bool CTCCreatureModeManager::AddMode(long modeType)
{
    CModePointer mode = AllocateMode(modeType);
    return AddMode(mode);
}
