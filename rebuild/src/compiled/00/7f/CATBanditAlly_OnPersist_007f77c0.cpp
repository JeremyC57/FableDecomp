// CAT_BanditAlly::OnPersist(CPersistContext*, long) -- persists DeactivateAfterSetTime (+0x20, signed char) and FramesAfterActivationToDeactivate (+0x24, long) through two typed context transfers with zero defaults
// Leaf: OnPersist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F77C0, oracle length 70 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,[context] ; mov esi,ecx ; lea eax,[esp+0Bh] ; push eax ; lea ecx,[esi+0x20] ; push ecx ; push offset "DeactivateAfterSetTime" ; mov ecx,edi ; mov byte ptr [default],0 ; call Transfer_004045c0 ; lea edx,[default2] ; push edx ; add esi,0x24 ; push esi ; push offset "FramesAfterActivationToDeactivate" ; mov ecx,edi ; mov dword ptr [default2],0 ; call Transfer_004106f0 ; pop edi ; pop esi ; pop ecx ; ret 8
// Persist hook: two members are handed by reference to typed CPersistContext transfer helpers
// (@ 0x004045C0 and 0x004106F0, labels propagated) together with their names and a zero default
// built in a frame temporary (both defaults share the one frame slot: their lifetimes do not
// overlap). The second member's address is formed in place (`add esi,0x24 ; push esi`,
// GOTCHAS). The second argument is unused by this body. Member types follow the default's
// store width. One of 9 bodies of this shape.
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004045C0 Transfer<bool>, 0x004106F0 Transfer<unsigned long> (agentNF19: Ego_r/FableWin ??$Transfer@_N@.. / ??$Transfer@K@..)
};

struct CAT_BanditAlly {
    unsigned char _pad[0x20];
    bool m_20;                        // +0x20 "DeactivateAfterSetTime"
    unsigned char _pad2[0x3];
    unsigned long m_24;               // +0x24 "FramesAfterActivationToDeactivate"
    void OnPersist(CPersistContext* context, long unused);
};

void CAT_BanditAlly::OnPersist(CPersistContext* context, long unused)
{
    context->Transfer<bool>("DeactivateAfterSetTime", m_20, false);
    context->Transfer<unsigned long>("FramesAfterActivationToDeactivate", m_24, 0);
}