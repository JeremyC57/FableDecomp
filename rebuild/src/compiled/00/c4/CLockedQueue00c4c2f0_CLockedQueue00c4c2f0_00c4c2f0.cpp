// CLockedQueue_00c4c2f0::CLockedQueue_00c4c2f0(void* owner, void* target) -- a cleared three-dword
// container, the owner pointer, an embedded critical section, a cleared flag and the target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C2F0, length 55 bytes (+ int3 padding).
// Callee: CCriticalSection_00c1c810::CCriticalSection_00c1c810() @ 0x00C1C810 (InitializeCriticalSection
// on `this`, 0x18 bytes) for the member at +0x10. No vtable. The first argument is loaded before
// the prologue push and stored at +0x0c after the three zero stores; the second is read after the
// member constructor and stored at +0x2c behind the cleared byte at +0x28. `ret 8`.
// The manifest label "CConsoleVariable::CConsoleVariable" is propagated and rejected; the class is
// named by shape (script module).
struct CCriticalSection_00c1c810 {             // RTL_CRITICAL_SECTION wrapper, 0x18 bytes
    unsigned char _body[0x18];
    CCriticalSection_00c1c810();               // 0x00C1C810
};

struct CRetailVector_Retail {                  // first / last / end
    void* m_first;
    void* m_last;
    void* m_end;
    CRetailVector_Retail() : m_first(0), m_last(0), m_end(0) { }
};

struct CLockedQueue_00c4c2f0 {
    CRetailVector_Retail      m_items;         // +0x00
    void*                     m_pOwner;        // +0x0c
    CCriticalSection_00c1c810 m_lock;          // +0x10
    unsigned char             m_flag;          // +0x28
    void*                     m_pTarget;       // +0x2c
    CLockedQueue_00c4c2f0(void* owner, void* target);
};

CLockedQueue_00c4c2f0::CLockedQueue_00c4c2f0(void* owner, void* target)
    : m_items(), m_pOwner(owner), m_lock(), m_flag(0), m_pTarget(target)
{
}