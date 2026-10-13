// CPersistable_00d5e070::PersistTransfer2_00d5e070(CPersistContext* context) -- persists the "TestFinished" and "BanditsDefeated" byte flags at +0x5c/+0x5d
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D5E070, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,[esp+10h] ; mov esi,ecx ; lea eax,[esp+0Bh] ; push eax ; lea ecx,[esi+0x5c] ; push ecx ; push offset "TestFinished" ; mov ecx,edi ; mov byte ptr [esp+17h],0 ; call Transfer<signed char> ; lea edx,[esp+10h] ; push edx ; add esi,0x1 ; push esi ; push offset "BanditsDefeated" ; mov ecx,edi ; mov byte ptr [esp+1Ch],0 ; call Transfer<signed char> ; pop edi ; pop esi ; pop ecx ; ret 4
// Virtual persist hook: two one-byte members at +0x5c and +0x5d are transferred through
// CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0, each with its own
// zero default materialised as a one-byte temporary right before its call (named locals would be
// hoisted and shared; the second member's address is formed by advancing
// the first, as retail does). Member names are taken from the persisted keys. One of 16 bodies.
struct CPersistContext {
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
};

struct CPersistable_00d5e070 {
    virtual void v00();
    unsigned char _pad[0x58];
    signed char m_bTestFinished;        // +0x5c
    signed char m_bBanditsDefeated;     // +0x5d
    virtual void PersistTransfer2_00d5e070(CPersistContext* context);
};

void CPersistable_00d5e070::PersistTransfer2_00d5e070(CPersistContext* context)
{
    context->Transfer("TestFinished", m_bTestFinished, 0);
    context->Transfer("BanditsDefeated", m_bBanditsDefeated, 0);
}