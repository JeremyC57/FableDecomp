// CPersistable_00eb6980::PersistTransfer2_00eb6980(CPersistContext* context) -- persists the "HasBeenThreatened" and "HasGivenIntroToHero" byte flags at +0x30/+0x38
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB6980, oracle length 67 bytes.
// Grade: candidate -- still DIFFER(67v67) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,[esp+10h] ; mov esi,ecx ; lea eax,[esp+0Bh] ; push eax ; lea ecx,[esi+0x30] ; push ecx ; push offset "HasBeenThreatened" ; mov ecx,edi ; mov byte ptr [esp+17h],0 ; call Transfer<signed char> ; lea edx,[esp+10h] ; push edx ; add esi,0x8 ; push esi ; push offset "HasGivenIntroToHero" ; mov ecx,edi ; mov byte ptr [esp+1Ch],0 ; call Transfer<signed char> ; pop edi ; pop esi ; pop ecx ; ret 4
// Virtual persist hook: two one-byte members at +0x30 and +0x38 are transferred through
// CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0, each with its own
// zero default materialised as a one-byte temporary right before its call (named locals would be
// hoisted and shared; the second member's address is formed by advancing
// the first, as retail does). Member names are taken from the persisted keys. One of 16 bodies.
struct CPersistContext {
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
};

struct CPersistable_00eb6980 {
    virtual void v00();
    unsigned char _pad[0x2c];
    signed char m_bHasBeenThreatened;   // +0x30
    signed char m_bHasGivenIntroToHero; // +0x38
    virtual void PersistTransfer2_00eb6980(CPersistContext* context);
};

void CPersistable_00eb6980::PersistTransfer2_00eb6980(CPersistContext* context)
{
    context->Transfer("HasBeenThreatened", m_bHasBeenThreatened, 0);
    context->Transfer("HasGivenIntroToHero", m_bHasGivenIntroToHero, 0);
}
