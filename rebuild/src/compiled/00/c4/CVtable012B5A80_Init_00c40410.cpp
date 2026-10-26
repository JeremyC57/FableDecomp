// CVtable_012B5A80::Init() -- reset a pooled character-morph action: clear its "unused" bit, zero two vectors and set the three axis vectors to the reversed identity; always succeeds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40410, length 62 bytes (+ int3 padding).
// Label correction: the manifest's "CTCWife::ClearAllAppearanceChangedNoticed" is propagated. The only caller is the
// landed factory CTCHeroStats::CreateCharacterMorphAction @ 0x00C3DCD0 (call site 0x00C3DD96), which builds this 0x4c-byte
// action (base constructor 0x00C4CF20 = the intrusive list node CVtable_012B8778: vptr, prev +4, next +8; own vtable
// 0x012B5A80, a no-RTTI module, so the class is named by its vtable; +0x48 starts as 0x80000000) and discards the action
// when this returns false. The five vectors' roles are not known; the names are descriptive.
// Shape: the first two vectors are cleared with chained assignments (`X = Y = Z = 0.0f` stores Z, Y, X), the axes through
// an inline Set(x, y, z); VC7.1 shares one zero register and one 1.0f register (`mov edx,3F800000h`) for all the float
// stores and schedules each axis's zero stores before its 1.0f; `mov al,1` returns true.
class C3DVector { public:
    float X, Y, Z;
    void Set(float x, float y, float z) { X = x; Y = y; Z = z; }
};

struct CVtable_012B5A80 {
    void*         __vftable;                                // +0x00
    void*         m_pPrev;                                  // +0x04
    void*         m_pNext;                                  // +0x08
    C3DVector     m_position;                               // +0x0c
    C3DVector     m_velocity;                               // +0x18
    C3DVector     m_axisX;                                  // +0x24
    C3DVector     m_axisY;                                  // +0x30
    C3DVector     m_axisZ;                                  // +0x3c
    unsigned long m_flags;                                  // +0x48 (bit 31: not initialised)
    bool Init();
};

bool CVtable_012B5A80::Init()
{
    m_flags &= 0x7fffffff;
    m_position.X = m_position.Y = m_position.Z = 0.0f;
    m_velocity.X = m_velocity.Y = m_velocity.Z = 0.0f;
    m_axisX.Set(0.0f, 0.0f, 1.0f);
    m_axisY.Set(0.0f, 1.0f, 0.0f);
    m_axisZ.Set(1.0f, 0.0f, 0.0f);
    return true;
}