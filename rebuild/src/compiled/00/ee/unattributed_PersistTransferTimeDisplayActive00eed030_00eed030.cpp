// CPersistable_00eed030::PersistTransfer_TimeDisplayActive_00eed030(CPersistContext* context) -- persists the "TimeDisplayActive" flag at +0x44
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EED030, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; lea eax,[esp+3] ; push eax ; add ecx,0x44 ; push ecx ; mov ecx,[esp+10h] ; push offset "TimeDisplayActive" ; mov byte ptr [esp+0Fh],0 ; call CPersistContext::Transfer<signed char> ; pop ecx ; ret 4
// Virtual persist hook (slot 1 of this class's vtable): transfers the single byte member at
// +0x44 through CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0,
// with a zero default built in a one-byte stack temporary. The member name is taken
// from the persisted key. One of 44 bodies of this shape.
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004045C0 = Transfer<bool> (agentNF19: Ego_r/FableWin ??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z; CPersistTraits<bool> callee 0x00403EB0)
};

struct CPersistable_00eed030 {
    unsigned char _pad[0x40];        // after the vptr; members not touched here
    bool m_bTimeDisplayActive;                // +0x44
    virtual void v00();
    virtual void PersistTransfer_TimeDisplayActive_00eed030(CPersistContext* context);
};

void CPersistable_00eed030::PersistTransfer_TimeDisplayActive_00eed030(CPersistContext* context)
{
    bool defaultValue = false;
    context->Transfer("TimeDisplayActive", m_bTimeDisplayActive, defaultValue);
}