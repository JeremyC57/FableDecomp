// CTCActionUseBed::vfunc_1(CPersistContext* context, long unused) -- persists the "UseableByHero" and "OwnedByHero" byte flags at +0x20/+0x21
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EBE50, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,[esp+10h] ; mov esi,ecx ; lea eax,[esp+0Bh] ; push eax ; lea ecx,[esi+0x20] ; push ecx ; push offset "UseableByHero" ; mov ecx,edi ; mov byte ptr [esp+17h],0 ; call Transfer<signed char> ; lea edx,[esp+10h] ; push edx ; add esi,0x1 ; push esi ; push offset "OwnedByHero" ; mov ecx,edi ; mov byte ptr [esp+1Ch],0 ; call Transfer<signed char> ; pop edi ; pop esi ; pop ecx ; ret 8
// Virtual persist hook: two one-byte members at +0x20 and +0x21 are transferred through
// CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0, each with its own
// zero default materialised as a one-byte temporary right before its call (named locals would be
// hoisted and shared; the second member's address is formed by advancing
// the first, as retail does). Member names are taken from the persisted keys. One of 16 bodies.
struct CPersistContext {
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
};

struct CTCActionUseBed {
    virtual void v00();
    unsigned char _pad[0x1c];
    signed char m_bUseableByHero;       // +0x20
    signed char m_bOwnedByHero;         // +0x21
    virtual void vfunc_1(CPersistContext* context, long unused);
};

void CTCActionUseBed::vfunc_1(CPersistContext* context, long unused)
{
    context->Transfer("UseableByHero", m_bUseableByHero, 0);
    context->Transfer("OwnedByHero", m_bOwnedByHero, 0);
}
