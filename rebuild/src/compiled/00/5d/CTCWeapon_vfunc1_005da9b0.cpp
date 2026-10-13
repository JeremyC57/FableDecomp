// CTCWeapon::vfunc_1(CPersistContext*, long) -- persists TimeWielded (+0x20, long) and Wielded (+0x24, signed char) through two typed context transfers with zero defaults
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA9B0, oracle length 70 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,[context] ; mov esi,ecx ; lea eax,[esp+8] ; push eax ; lea ecx,[esi+0x20] ; push ecx ; push offset "TimeWielded" ; mov ecx,edi ; mov dword ptr [default],0 ; call Transfer_00410be0 ; lea edx,[default2] ; push edx ; add esi,0x24 ; push esi ; push offset "Wielded" ; mov ecx,edi ; mov byte ptr [default2],0 ; call Transfer_004045c0 ; pop edi ; pop esi ; pop ecx ; ret 8
// Persist hook: two members are handed by reference to typed CPersistContext transfer helpers
// (@ 0x00410BE0 and 0x004045C0, labels propagated) together with their names and a zero default
// built in a frame temporary (both defaults share the one frame slot: their lifetimes do not
// overlap). The second member's address is formed in place (`add esi,0x24 ; push esi`,
// GOTCHAS). The second argument is unused by this body. Member types follow the default's
// store width. One of 9 bodies of this shape.
struct CPersistContext {
    void Transfer_00410be0(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
    void Transfer_004045c0(const char* name, signed char& value, const signed char& defaultValue);   // 0x004045C0
};

struct CTCWeapon {
    unsigned char _pad[0x20];
    long m_20;                        // +0x20 "TimeWielded"
    signed char m_24;                 // +0x24 "Wielded"
    void vfunc_1(CPersistContext* context, long unused);
};

void CTCWeapon::vfunc_1(CPersistContext* context, long unused)
{
    context->Transfer_00410be0("TimeWielded", m_20, 0);
    context->Transfer_004045c0("Wielded", m_24, 0);
}