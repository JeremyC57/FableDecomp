// CThing::vfunc_30() -- CThing vtable slot 30: forwards to the virtual DrawIsDrawable (slot 27) as a tail jump
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD1E0, 5 bytes. CThing vtable 0x01239ABC slot 30.
// Callees: CThing::DrawIsDrawable (virtual slot 27, +0x6C; base body 0x004C7790).
// Shape: `return DrawIsDrawable();` through the vtable becomes `mov eax,[ecx] ; jmp [eax+6Ch]`.
// Slots 0..26 are declared only to place DrawIsDrawable at slot 27; no constructor, so no vtable is emitted.
struct CThing {
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03();
    virtual void Slot04(); virtual void Slot05(); virtual void Slot06(); virtual void Slot07();
    virtual void Slot08(); virtual void Slot09(); virtual void Slot10(); virtual void Slot11();
    virtual void Slot12(); virtual void Slot13(); virtual void Slot14(); virtual void Slot15();
    virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual void Slot20(); virtual void Slot21(); virtual void Slot22(); virtual void Slot23();
    virtual void Slot24(); virtual void Slot25(); virtual void Slot26();
    virtual bool DrawIsDrawable();     // slot 27, 0x004C7790
    virtual void Slot28(); virtual void Slot29();
    virtual bool vfunc_30();           // slot 30
};

bool CThing::vfunc_30()
{
    return DrawIsDrawable();
}