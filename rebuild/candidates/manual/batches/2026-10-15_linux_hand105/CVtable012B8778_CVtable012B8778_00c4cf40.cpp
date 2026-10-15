// CVtable_012B8778::~CVtable_012B8778() -- destroy an intrusive doubly-linked node: unhook it from its neighbours, clear its links, fall back to the root vtable
// Leaf: ??1CVtable_012B8778
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4CF40, length 49 bytes (+ int3 padding).
// Label correction: the manifest's "CTCWeapon::FrameUpdate" is propagated. The body stores vtable 0x012B8778 (no
// complete-object locator: a no-RTTI module, so the class is named by its vtable), whose constructor is 0x00C4CF20
// (vptr, +4 = +8 = 0). Its neighbours are the node's Link(prev, next) @ 0x00C4CED0 and Unlink() @ 0x00C4CEF0 (landed
// callers 0x00C3CA00 / 0x00C3D610 call this node "CPooledCombatAction"); this destructor is Unlink() inlined
// byte-for-byte (one zero register for the tests and the clears) followed by the root base's vtable 0x012B5868.
// Shape: own vtable first, the inline Unlink, then the root's user-declared empty virtual destructor stores its
// vtable (a compiler-generated one would drop that store).
struct CVtable_012B5868 {                                   // root of the module's polymorphic classes
    virtual ~CVtable_012B5868() { }
};

struct CVtable_012B8778 : CVtable_012B5868 {                // intrusive list node, 12 bytes
    CVtable_012B8778* m_pPrev;                              // +0x04
    CVtable_012B8778* m_pNext;                              // +0x08
    void Unlink()                                           // 0x00C4CEF0 out of line
    {
        if (m_pPrev)
            m_pPrev->m_pNext = m_pNext;
        if (m_pNext)
            m_pNext->m_pPrev = m_pPrev;
        m_pPrev = 0;
        m_pNext = 0;
    }
    virtual ~CVtable_012B8778();
};

CVtable_012B8778::~CVtable_012B8778()
{
    Unlink();
}
