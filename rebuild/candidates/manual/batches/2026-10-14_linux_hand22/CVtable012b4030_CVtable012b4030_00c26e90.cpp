// CVtable_012b4030::CVtable_012b4030(void* owner) -- a script-module object holding an inner
// polymorphic link {vptr, owner} at +4, a worker constructed from that link at +0x0c, and the owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26E90, length 45 bytes (+ int3 padding).
// Callee: the EH-framed worker constructor @ 0x00C27070 (takes the link's address; it builds a
// critical section @ 0x00C1C810 and more). Vtables (script module, no RTTI, named by address):
// 0x012B4030 = this class, 0x012B402C = the link member at +4 (inline constructor storing the
// owner at +8). No base constructor call. `this`'s vtable is stored first, then the members in
// declaration order; the owner is kept in edi for the final store at +0x3c. The twin @ 0x00C26A30
// is the same shape with vtables 0x012B401C / 0x012B4018 and worker @ 0x00C26C10.
// The manifest label "CConsoleVariable::CConsoleVariable" is propagated and rejected.
struct CLink_012b402c {                        // vtable 0x012B402C, 8 bytes
    virtual void v00();
    void* m_pOwner;                            // +0x04
    CLink_012b402c(void* owner) : m_pOwner(owner) { }
};

struct CWorker_00c27070 {                      // 0x30 bytes
    unsigned char _body[0x30];
    CWorker_00c27070(CLink_012b402c* link);    // 0x00C27070
};

struct CVtable_012b4030 {                      // vtable 0x012B4030
    virtual void v00();
    CLink_012b402c   m_link;                   // +0x04
    CWorker_00c27070 m_worker;                 // +0x0c
    void*            m_pOwner;                 // +0x3c
    CVtable_012b4030(void* owner);
};

CVtable_012b4030::CVtable_012b4030(void* owner)
    : m_link(owner), m_worker(&m_link), m_pOwner(owner)
{
}
