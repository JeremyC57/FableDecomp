// CTCRandomAppearanceMorph::vfunc_1(CPersistContext* context, long unused) -- persists the Seed dword at +0x18 with a zero default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00727BC0, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; lea eax,[esp] ; push eax ; add ecx,0x18 ; push ecx ; mov ecx,[esp+10h] ; push offset "Seed" ; mov dword ptr [esp+0Ch],0 ; call CPersistContext::Transfer<long> ; pop ecx ; ret 8
// Virtual persist hook (slot 1 of this class's vtable): transfers the dword member at +0x18
// through CPersistContext::Transfer<long>(name, value, default) @ 0x004106F0, with a zero
// default built in a dword stack temporary. The second argument is not used by this body. The member name is taken from the
// persisted key; the member type is any 4-byte scalar (`long` models it). One of 16 bodies.
struct CPersistContext {
    void Transfer(const char* name, long& value, const long& defaultValue);   // Transfer<long> @ 0x004106F0
};

struct CTCRandomAppearanceMorph {
    unsigned char _pad[0x14];        // after the vptr; members not touched here
    long m_Seed;                        // +0x18
    virtual void v00();
    virtual void vfunc_1(CPersistContext* context, long unused);
};

void CTCRandomAppearanceMorph::vfunc_1(CPersistContext* context, long unused)
{
    long defaultValue = 0;
    context->Transfer("Seed", m_Seed, defaultValue);
}