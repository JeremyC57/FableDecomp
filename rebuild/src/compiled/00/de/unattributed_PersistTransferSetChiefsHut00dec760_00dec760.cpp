// CPersistable_00dec760::PersistTransfer_SetChiefsHut_00dec760(CPersistContext* context) -- persists the "SetChiefsHut" flag at +0x48
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DEC760, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; lea eax,[esp+3] ; push eax ; add ecx,0x48 ; push ecx ; mov ecx,[esp+10h] ; push offset "SetChiefsHut" ; mov byte ptr [esp+0Fh],0 ; call CPersistContext::Transfer<signed char> ; pop ecx ; ret 4
// Virtual persist hook (slot 1 of this class's vtable): transfers the single byte member at
// +0x48 through CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0,
// with a zero default built in a one-byte stack temporary. The member name is taken
// from the persisted key. One of 44 bodies of this shape.
class CPersistContext { public:
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
};

struct CPersistable_00dec760 {
    unsigned char _pad[0x44];        // after the vptr; members not touched here
    signed char m_bSetChiefsHut;                // +0x48
    virtual void v00();
    virtual void PersistTransfer_SetChiefsHut_00dec760(CPersistContext* context);
};

void CPersistable_00dec760::PersistTransfer_SetChiefsHut_00dec760(CPersistContext* context)
{
    signed char defaultValue = 0;
    context->Transfer("SetChiefsHut", m_bSetChiefsHut, defaultValue);
}