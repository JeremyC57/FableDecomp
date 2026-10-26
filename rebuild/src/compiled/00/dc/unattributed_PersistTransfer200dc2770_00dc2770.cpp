// CPersistable_00dc2770::PersistTransfer2_00dc2770(CPersistContext* context) -- persists the "QuestStartScreenShown" and "HaveOpenedDemonDoor" byte flags at +0x71/+0x72
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC2770, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,[esp+10h] ; mov esi,ecx ; lea eax,[esp+0Bh] ; push eax ; lea ecx,[esi+0x71] ; push ecx ; push offset "QuestStartScreenShown" ; mov ecx,edi ; mov byte ptr [esp+17h],0 ; call Transfer<signed char> ; lea edx,[esp+10h] ; push edx ; add esi,0x1 ; push esi ; push offset "HaveOpenedDemonDoor" ; mov ecx,edi ; mov byte ptr [esp+1Ch],0 ; call Transfer<signed char> ; pop edi ; pop esi ; pop ecx ; ret 4
// Virtual persist hook: two one-byte members at +0x71 and +0x72 are transferred through
// CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0, each with its own
// zero default materialised as a one-byte temporary right before its call (named locals would be
// hoisted and shared; the second member's address is formed by advancing
// the first, as retail does). Member names are taken from the persisted keys. One of 16 bodies.
class CPersistContext { public:
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
};

struct CPersistable_00dc2770 {
    virtual void v00();
    unsigned char _pad[0x6d];
    signed char m_bQuestStartScreenShown;// +0x71
    signed char m_bHaveOpenedDemonDoor; // +0x72
    virtual void PersistTransfer2_00dc2770(CPersistContext* context);
};

void CPersistable_00dc2770::PersistTransfer2_00dc2770(CPersistContext* context)
{
    context->Transfer("QuestStartScreenShown", m_bQuestStartScreenShown, 0);
    context->Transfer("HaveOpenedDemonDoor", m_bHaveOpenedDemonDoor, 0);
}