// CTCCreatureModeManager::AllocateMode(unsigned long modeType) -- pick one of the creature modes registered for the type at random and build it for the parent thing, wrapped in a new counted pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A8E0, 200 bytes (`ret 8`: the hidden result pointer and the type).
// Callees: the registry's UpperBound @ 0x0073D250 and LowerBound @ 0x0073D210 (const) / @ 0x0073D0E0 (CVectorMap<unsigned long,
// CTCCreatureModeManager::CRegisteredModeInfo> on the static registry @ 0x013BAD44; two labels carry other instantiations'
// value types), CTCBase::PeekWorld() const @ 0x00686A80, CWorld::GetPRandomSeed() const @ 0x0049D810, GFROR13 @ 0x00497890, the
// registered allocator (`__fastcall`, the thing in ecx), operator new @ 0x00BFEA1A (12-byte count block), the count block's
// deleter 0x007180B0 (as the landed CCombatAction_FallStunned::Initialise 0x008ED950).
// Layout: CTCBase ParentThing +0x04; registry entries are 8 bytes {mode type, allocator}.
// Shape: the count is UpperBound - LowerBound with the upper bound taken first (a named local) and each lookup given the type
// parameter's own address; the parent thing is read into a local before the registry lookup; the random pick is the
// landed WorldRandomRange shape; the result is the raw-pointer CCountedPointer ctor built straight in the return slot
// (`if (object) block = new SCountBlock(object) else block = 0`: three duplicated exits with the new's null test).
void __fastcall Destroy_007180b0(void* object);         // 0x007180B0
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
long __fastcall GFROR13(unsigned long);  // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}
class CThing;
struct CModeBase;
struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_007180b0), m_pObject(object) { }
};
template <class T> class CCountedPointer { public:
    T*           m_pObject;                             // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
};
struct SRegisteredMode {                                // pair<unsigned long, CRegisteredModeInfo>
    unsigned long first;
    CModeBase* (__fastcall* Allocate)(CThing* thing);   // CRegisteredModeInfo
};
struct CModeRegistry {                                  // CVectorMap<unsigned long, CRegisteredModeInfo>
    const SRegisteredMode* UpperBound(const unsigned long& key) const;   // 0x0073D250
    const SRegisteredMode* LowerBound(const unsigned long& key) const;   // 0x0073D210
    SRegisteredMode* LowerBound(const unsigned long& key);               // 0x0073D0E0
};
extern CModeRegistry g_CreatureModeRegistry;            // 0x013BAD44
class CWorld { public:
    unsigned long* GetPRandomSeed() const;              // 0x0049D810
};
inline long CountRegisteredModes(const CModeRegistry& registry, const unsigned long& type)
{
    const SRegisteredMode* last = registry.UpperBound(type);
    return last - registry.LowerBound(type);
}
class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
class CTCCreatureModeManager : public CTCBase { public:
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CCountedPointer<CModeBase> AllocateMode(unsigned long modeType);
};

CCountedPointer<CModeBase> CTCCreatureModeManager::AllocateMode(unsigned long modeType)
{
    long count = CountRegisteredModes(g_CreatureModeRegistry, modeType);
    unsigned long* seed = PeekWorld().GetPRandomSeed();
    unsigned long index = WorldRandomRange(*seed, count);
    CThing* thing = ParentThing;
    return CCountedPointer<CModeBase>(g_CreatureModeRegistry.LowerBound(modeType)[index].Allocate(thing));
}