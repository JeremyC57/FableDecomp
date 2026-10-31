// CAudioSystem::CreateExclusiveAudioObject(unsigned long flags) -- EH-framed libpk factory: reject flags outside the two configuration records' masks (error -8); with a positive limit at +0x70 recycle the free head of the secondary node pool (+0x30) when it is the derived (exclusive) kind (error -9 when the pool is empty); with a limit of -1 allocate a fresh 0x1DC-byte exclusive audio object; any other limit is error -9; then Initialise it with the flags (discard it on failure), pop the free node for a positive limit and push the object on the pool's active chain
// Grade: DIFFER(335v335) -- the two vtable loads swap registers: retail eax for IsDerived (slot 1) and edx for Initialise (slot 3), ours edx / eax (equal-use colouring). Renamed this round onto the landed libpk names (CAudioSystem, CNodePool, CAudioObject / CUnprovenExclusiveAudioObject_00C3D910, the 0x0139C9A0 / 0x0139C9D0 records); named Init result and an if-form cast tried (unchanged / worse)
// Leaf: ?CreateExclusiveAudioObject@CAudioSystem@@QAEPAVCUnprovenExclusiveAudioObject_00C3D910@@K@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3DB80, 335 bytes, thiscall `ret 4`, /EHsc (state 0 frees the block if
//   the base constructor throws). Sole caller: the libpk API 0x00C33DC0 (`mov ecx,0143BB58h ; call ; pop ebx ; ret 4`, so
//   the object pointer is passed through to the API's caller).
// Names (libpk: no PDB, no RTTI; all descriptive): the class built here stores vtable 0x012B5AB0 after the base constructor
//   0x00C42D60, i.e. the landed CUnprovenExclusiveAudioObject_00C3D910 over CAudioObject (AttachBackend 0x00C418C0 identity:
//   slot 1 IsDerived, slot 3 Initialise(unsigned long), 0x1DC stride); the pooled-node helpers are the landed CNodePool
//   GetFree 0x00C3C9A0 / PopFree 0x00C3CA30 / PushActive 0x00C3C9B0, the discard is the landed CAudioSystem::DiscardEvent
//   0x00C3D610 (its parameter type CAudioEvent is that row's name for the same pooled object), the allocator is
//   CPKObject::operator new 0x00C3C360. The method name is descriptive (the binding ?SetModeFlags@CAudioSystem@@QAEXK@Z of
//   the API row returns void, but this body returns the object).
// Data: the configuration records 0x0139C9A0 / 0x0139C9D0 (their third words are the allowed-flag masks), the latched
//   error 0x0143BB48 and the report hook 0x0143BBD4 (the bound sibling names).
// Shape: as the landed sibling 0x00C3DCD0; the pooled node is cast with `IsDerived() ? node : 0` (neg/sbb/and) and only
//   the pooled path has the empty-pool error; both -9 reports and the -8 report share one `push e ; call eax` tail.
typedef void (__stdcall* PFNREPORT_00c3db80)(long error);
extern long g_lastError_0143bb48;                           // 0x0143BB48
extern void (__stdcall* g_pfnReport_0143bbd4)(long error);  // 0x0143BBD4
struct CUnprovenAudioConfiguration24 { unsigned int UnprovenWords[6]; };
extern CUnprovenAudioConfiguration24 g_UnprovenAudioConfig_0139C9A0, g_UnprovenAudioConfig_0139C9D0;

class CPKObject {                                           // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject();
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};
struct CPooledNode : public CPKObject {                     // intrusive doubly-linked node
    CPooledNode* Prev;                                      // +0x04
    CPooledNode* Next;                                      // +0x08
};
struct CAudioEvent;
class CAudioObject : public CPooledNode {
public:
    CAudioObject();                                         // 0x00C42D60
    virtual bool IsDerived();                               // slot 1 (+0x04)
    virtual bool IsInitialised();                           // slot 2
    virtual bool Initialise(unsigned long flags);           // slot 3 (+0x0C)
    unsigned char _body[0x1dc - 0x0c];
};
class CUnprovenExclusiveAudioObject_00C3D910 : public CAudioObject {   // vtable 0x012B5AB0, 0x1DC bytes
public:
    virtual bool IsDerived();
    virtual bool Initialise(unsigned long flags);
};
struct CNodePool {                                          // 20 bytes: an active chain and a free chain
    unsigned char _body[0x14];
    CPooledNode* GetFree();                                 // 0x00C3C9A0
    void PushActive(CPooledNode* node);                     // 0x00C3C9B0
    void PopFree();                                         // 0x00C3CA30
};
struct CAudioSystem {                                       // the libpk audio system singleton (0x0143BB58)
    unsigned char _pad00[0x30];
    CNodePool m_secondaryPool;                              // +0x30
    unsigned char _pad44[0x70 - 0x44];
    long m_secondaryLimit;                                  // +0x70 (> 0 recycle, -1 allocate)
    void DiscardEvent(CAudioEvent* event);                  // 0x00C3D610
    CUnprovenExclusiveAudioObject_00C3D910* CreateExclusiveAudioObject(unsigned long flags);
};

static inline void ReportError(long error)
{
    if (g_lastError_0143bb48 == 0) {
        g_lastError_0143bb48 = error;
        if (g_pfnReport_0143bbd4)
            g_pfnReport_0143bbd4(error);
    }
}

CUnprovenExclusiveAudioObject_00C3D910* CAudioSystem::CreateExclusiveAudioObject(unsigned long flags)
{
    unsigned long invalidFlags = flags & ~(g_UnprovenAudioConfig_0139C9A0.UnprovenWords[2] | g_UnprovenAudioConfig_0139C9D0.UnprovenWords[2]);
    if (invalidFlags) {
        ReportError(-8);
        return 0;
    }
    CUnprovenExclusiveAudioObject_00C3D910* object = 0;
    if (m_secondaryLimit > 0) {
        CAudioObject* node = (CAudioObject*)m_secondaryPool.GetFree();
        if (node)
            object = node->IsDerived() ? (CUnprovenExclusiveAudioObject_00C3D910*)node : 0;
        else
            ReportError(-9);
    } else if (m_secondaryLimit == -1) {
        object = new CUnprovenExclusiveAudioObject_00C3D910;
    } else {
        ReportError(-9);
    }
    if (!object)
        return 0;
    if (!object->Initialise(flags)) {
        DiscardEvent((CAudioEvent*)object);
        object = 0;
    }
    if (m_secondaryLimit > 0)
        m_secondaryPool.PopFree();
    m_secondaryPool.PushActive(object);
    return object;
}
