// CVtable_012bf014::CVtable_012bf014(long) -- own vtable (0x012BF014, a no-RTTI module), a zeroed long at +8, 44100 at +0xc and the float 0.0626894 at +4; the argument is unused (ret 4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73EF0, length 32 bytes. No callees.
// The manifest label (C3DGroup2::C3DGroup2) is propagated; the vtable has no complete-object
// locator so the class is named by its vtable address. 0xAC44 = 44100 (a sample rate) and
// 0x3D80634A is the float constant stored as an immediate; retail writes +8, +0xc and then +4.
struct CVtable_012bf014 {
    virtual void v00();
    float m_scale;                         // +4 = 0.062689379f (0x3D80634A)
    long m_position;                       // +8 = 0
    long m_sampleRate;                     // +0xc = 44100
    CVtable_012bf014(long unused);
};

CVtable_012bf014::CVtable_012bf014(long unused)
{
    m_position = 0;
    m_sampleRate = 44100;
    m_scale = 0.062689379f;
}