// CVtable_012B5C10::~CVtable_012B5C10() -- destructor: own vtable; unless the stopped flag (bit 31 of +0x2c) is already set, Stop() runs (the virtual slot 2, devirtualised); then the list-node base destructor
// Leaf: ??1CVtable_012B5C10
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F3D0, length 91 bytes (EH frame; built under /EHsc).
// Callees: CVtable_012B5C10::Stop() @ 0x00C59710 (vtable slot 2: `mov dword ptr [ecx+2Ch],80000000h`),
//          the list-node base destructor @ 0x00C4CF40 (unlinks prev/next at +4/+8, stores 0x012B8778
//          then 0x012B5868; manifest label "CTCWeapon::FrameUpdate" is propagated).
// Identity: the manifest label "CParticleEffect::~CParticleEffect" is propagated. Vtable 0x012B5C10
// has no complete-object locator (the 0x00C3.. no-RTTI module): slot 0 is the deleting destructor
// 0x00C3F470 (calls this body, then the class operator delete 0x00C3C370), slot 2 the Stop above.
// The derived class with vtable 0x012B5C60 (destructor 0x00C3F490) repeats the same test with its
// own slot-2 override 0x00C5A1F0 and then calls this body as its base destructor.
// Shape: the EH state 0 store (base sub-object live) sits between the flag test and its branch; the
// flag is a one-bit field read through an inline accessor that NEGATES it into a bool
// (`bool IsRunning() const { return !m_stopped; }` -> `shr eax,1Fh ; not al ; test al,1`; testing
// `!IsStopped()`, `!(bool)m_stopped` or an unsigned char accessor drops the `not al`);
// the virtual Stop() called on `this` inside the destructor is devirtualised to a direct call.

struct CListNode {                                                        // base: intrusive list node
    virtual ~CListNode();                                                 // 0x00C4CF40
    CListNode* m_prev;                                                    // +0x04
    CListNode* m_next;                                                    // +0x08
};

struct CVtable_012B5C10 : CListNode {                                     // vtable 0x012B5C10 (no RTTI)
    unsigned char _body[0x20];                                            // +0x0c .. +0x2c
    unsigned long m_state   : 31;                                         // +0x2c bits 0..30
    unsigned long m_stopped : 1;                                          // +0x2c bit 31
    virtual void v01();                                                   // slot 1 (0x00C596E0)
    virtual void Stop();                                                  // slot 2 (0x00C59710)
    bool IsRunning() const { return !m_stopped; }
    virtual ~CVtable_012B5C10();
};

CVtable_012B5C10::~CVtable_012B5C10()
{
    if (IsRunning())
        Stop();
}
