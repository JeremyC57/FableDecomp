// CVtable_012b8ed4::Close() -- vtable slot 2 of the OpenAL streaming buffer: closing the stream only has to release its source and buffers (forwards to the virtual ReleaseBuffer)
// Leaf: ?Close@CVtable_012b8ed4@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C57490, 5 bytes (`mov eax,[ecx]; jmp [eax+0x14]`), thiscall; slot 2 of the vtable
//   0x012B8ED4 (cell 0x012B8EDC) and of the derived 0x012B8E20 (cell 0x012B8E28).
// Identity (descriptive; libpk has no PDB or RTTI): the class is named by its vtable as the landed Initialize 0x00C57460 /
//   ReleaseBuffer 0x00C579C0 rows do; the DirectSound counterpart's slot 2 is 0x00C57010. Slot 5 (+0x14) is the landed
//   ReleaseBuffer 0x00C579C0. `Close` is a descriptive name (NOTES).
// Extent: the catalog row ran through the int3 padding into slot 7 (0x00C574A0, `lea eax,[ecx+4]; ret`); see SPLITS.tsv.
class CVtable_012b8ed4 {                            // the OpenAL streaming buffer (vtable 0x012B8ED4, ctor 0x00C57780)
public:
    virtual void v00();                                                              // slot 0: deleting destructor 0x00C57D60
    virtual bool Initialize(const void* device, void* window, bool multithreaded);   // slot 1: 0x00C57460
    virtual void Close();                                                            // slot 2: 0x00C57490
    virtual unsigned char CreateAndPlay(unsigned long a, unsigned long b);           // slot 3: 0x00C57800
    virtual bool Create(unsigned long a);                                            // slot 4: 0x00C572C0
    virtual void ReleaseBuffer();                                                    // slot 5: 0x00C579C0
};

void CVtable_012b8ed4::Close()
{
    ReleaseBuffer();
}