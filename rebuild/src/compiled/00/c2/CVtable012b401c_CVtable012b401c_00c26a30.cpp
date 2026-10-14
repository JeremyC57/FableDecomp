// CVtable_012b401c::CVtable_012b401c(void* owner) -- a script-module object holding an inner
// polymorphic link {vptr, owner} at +4, a worker constructed from that link at +0x0c, and the owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26A30, length 45 bytes (+ int3 padding).
// Callee: the EH-framed worker constructor @ 0x00C26C10 (takes the link's address; it builds a
// critical section @ 0x00C1C810 and more). Vtables (script module, no RTTI, named by address):
// 0x012B401C = this class, 0x012B4018 = the link member at +4 (inline constructor storing the
// owner at +8). No base constructor call. `this`'s vtable is stored first, then the members in
// declaration order; the owner is kept in edi for the final store at +0x3c. The twin @ 0x00C26E90
// is the same shape with vtables 0x012B4030 / 0x012B402C and worker @ 0x00C27070.
// The manifest label "CConsoleVariable::CConsoleVariable" is propagated and rejected.
struct CLink_012b4018 {                        // vtable 0x012B4018, 8 bytes
    virtual void v00();
    void* m_pOwner;                            // +0x04
    CLink_012b4018(void* owner) : m_pOwner(owner) { }
};

struct CWorker_00c26c10 {                      // 0x30 bytes
    unsigned char _body[0x30];
    CWorker_00c26c10(CLink_012b4018* link);    // 0x00C26C10
};

struct CVtable_012b401c {                      // vtable 0x012B401C
    virtual void v00();
    CLink_012b4018   m_link;                   // +0x04
    CWorker_00c26c10 m_worker;                 // +0x0c
    void*            m_pOwner;                 // +0x3c
    CVtable_012b401c(void* owner);
};

CVtable_012b401c::CVtable_012b401c(void* owner)
    : m_link(owner), m_worker(&m_link), m_pOwner(owner)
{
}