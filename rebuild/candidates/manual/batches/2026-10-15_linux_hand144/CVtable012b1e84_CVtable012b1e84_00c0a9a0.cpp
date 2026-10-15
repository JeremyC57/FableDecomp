// CVtable012b1e84::~CVtable012b1e84() -- restore the vtable and destroy the two embedded 4-byte handle arrays (8 at +0x3a4, then 16 at +0x364) through the EH vector destructor iterator
// Leaf: ??1CVtable012b1e84
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A9A0, length 109 bytes. Built under /EHsc.
// Label correction: the catalog's "CJoystick::~CJoystick" is propagated: CJoystick's vtable is 0x0129C0EC, while this body
// stores 0x012B1E84 (no RTTI row; no-RTTI 0x00C0xxxx module), so the class is named by its vtable.
// Callees: `eh vector destructor iterator' @ 0x00BFEED8 (element size 4, counts 8 and 16), the element destructor
// @ 0x00C0BA40 (a 4-byte handle whose release goes through a global hook @ 0x0143BB2C).
// Shape: member arrays of a class with a destructor are destroyed last-declared first; the EH state is 0 while the first
// array is still alive and -1 for its own destruction; `this` is spilled to the frame for the unwind funclet. A virtual
// function with a non-virtual destructor keeps the plain `??1` (as the landed 0x00C23FE0).
struct SHandle_00c0ba40 {                          // 4 bytes
    void* m_p;
    ~SHandle_00c0ba40();                           // 0x00C0BA40
};
struct CVtable012b1e84 {                           // vtable 0x012B1E84
    virtual void v00();
    unsigned char    _pad04[0x364 - 4];
    SHandle_00c0ba40 m_first[16];                  // +0x364
    SHandle_00c0ba40 m_second[8];                  // +0x3a4
    ~CVtable012b1e84();
};

CVtable012b1e84::~CVtable012b1e84()
{
}
