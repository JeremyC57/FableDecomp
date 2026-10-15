// CLockedUpdater_00caf820::Set(long first, long second) -- under the object's critical section (+8): store the two values (+0x68, +0x6c), raise the dirty flag (+0x64 = 1), run the update at 0x00CAF750; returns 0
// Grade: DIFFER(104v104) -- retail stores +0x6c then +0x68 BEFORE `mov ecx,esi` and the EH state store; the lane sinks the +0x68 store below the state store
// Leaf: Set
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAF820, 104 bytes (`ret 8`, fs:[0] frame).
// Label: the catalog's "CModifierGraphic::CModifierGraphic" is propagated: the body is no constructor (no vtable, it
// returns 0 in eax, not `this`), and the retail CModifierGraphic is a 12-byte record; class and name are descriptive.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10), the update @ 0x00CAF750
// (catalog "CModifierGraphic_UpdatePosition", `__thiscall`).
// Shape: a scoped lock object (its section pointer in the `push ecx` frame word, EH state 0 while it lives) whose
// inline destructor leaves the section; the second argument is loaded and stored first (source in that order).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10

struct CScopedSectionLock {
    void* m_pSection;
    CScopedSectionLock(void* section) : m_pSection(section) { EnterCriticalSection(section); }
    ~CScopedSectionLock() { LeaveCriticalSection(m_pSection); }
};

struct CLockedUpdater_00caf820 {
    unsigned char _pad00[8];
    unsigned char m_section[0x18];          // +0x08 CRITICAL_SECTION
    unsigned char _pad20[0x44];
    long          m_dirty;                  // +0x64
    long          m_first;                  // +0x68
    long          m_second;                 // +0x6c
    void Update();                          // 0x00CAF750
    long Set(long first, long second);
};

long CLockedUpdater_00caf820::Set(long first, long second)
{
    CScopedSectionLock lock(m_section);
    m_first = first;
    m_second = second;
    m_dirty = 1;
    Update();
    return 0;
}
