// CVtable_012bf0c8::CVtable_012bf0c8() -- base constructor (vtable 0x012BF050, lazy global init @ 0x00C74420), then six members cleared in the body before the derived vtable store
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77440, length 48 bytes.
// Callees: base constructor @ 0x00C74420 (manifest label "CRefCountedObject_AddRef" is wrong: it stores vtable 0x012BF050 and
// initialises a global on first use). Vtable 0x012BF0C8 has no RTTI locator (a no-RTTI module), hence the CVtable_<va> name;
// manifest label "CGameJoystickManager::CGameJoystickManager" is propagated.
// Shape: stores +0x10, +0x04, +0x08, +0x0c, byte +0x14, byte +0x24 in that order (not offset order) = body assignments in
// source order; all before the vtable store.
// Leaf: ??0CVtable_012bf0c8
struct CVtable_012bf050 {
    virtual ~CVtable_012bf050();
    CVtable_012bf050();                             // 0x00C74420
};

struct CVtable_012bf0c8 : CVtable_012bf050 {
    long m_first;                                   // +0x04
    long m_second;                                  // +0x08
    long m_third;                                   // +0x0c
    long m_count;                                   // +0x10
    bool m_active;                                  // +0x14
    unsigned char _pad15[0xf];                      // +0x15
    bool m_dirty;                                   // +0x24
    CVtable_012bf0c8();
};

CVtable_012bf0c8::CVtable_012bf0c8()
{
    m_count = 0;
    m_first = 0;
    m_second = 0;
    m_third = 0;
    m_active = false;
    m_dirty = false;
}