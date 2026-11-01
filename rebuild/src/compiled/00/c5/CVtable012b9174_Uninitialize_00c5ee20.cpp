// CVtable_012b9174::Uninitialize() -- vtable slot 2 of the silent audio output: shut the output down by calling its own ReleaseBuffer (slot 5)
// Leaf: ?Uninitialize@CVtable_012b9174@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5EE20, 5 bytes (+ int3 padding), thiscall, `mov eax,[ecx] ; jmp [eax+14h]`:
//   slot 2 of the vtable 0x012B9174 (word at 0x012B917C, its only reference; vptr stored by the landed ctor 0x00C5EEC0).
// Class: the audio library's silent (null) output, no RTTI / PDB, named by its vtable as the landed rows of the class
//   (ctor 0x00C5EEC0, IsChunkReady 0x00C5EE60, WriteBuffer 0x00C5EEB0) spell it.
// Name: descriptive (no PDB/RTTI). The four output vtables share one slot order: 1 Initialize(device, window, bool)
//   (0x00C5EE10 here; DirectSound 0x00C56F70 calls CoInitializeEx), 2 this slot, 3 CreateAndPlay (DirectSound 0x00C57380),
//   4 CreateLoopingBuffer (DirectSound 0x00C56D70; 0x00C572C0 `return false` in the other three), 5 ReleaseBuffer
//   (DirectSound 0x00C57040, OpenAL 0x00C579C0: the slot names the landed rows of those classes use).
//   Slot 2 is Initialize's counterpart: the DirectSound output's slot 2 (0x00C57010) calls slot 5 and then releases the
//   IDirectSound device and calls CoUninitialize; the waveOut (0x00C5D600) and OpenAL (0x00C57490) outputs, like this
//   one, only call slot 5. The manifest label `_gapscan::sub_00c5ee20` is a placeholder.
// Callee: this->ReleaseBuffer() (slot 5; 0x00C5EE50 for this class zeroes the block count at +0x604).
// Shape: a virtual member whose body is one call of another virtual on `this` compiles to a tail jump through the vtable.
// Layout: vptr only is touched; the 0x600-byte body and the +0x604 block count as in the landed class rows.
class CVtable_012b9174 {
public:
    virtual ~CVtable_012b9174();                                                    // slot 0 (0x00C5EF40 deleting dtor)
    virtual bool Initialize(const void* device, void* window, bool multithreaded);  // slot 1 (0x00C5EE10)
    virtual void Uninitialize();                                                     // slot 2 (0x00C5EE20)
    virtual bool CreateAndPlay(unsigned long channels, unsigned long bytes);        // slot 3 (0x00C5EE30)
    virtual bool CreateLoopingBuffer(unsigned long rate);                            // slot 4 (0x00C572C0, shared)
    virtual void ReleaseBuffer();                                                    // slot 5 (0x00C5EE50)
    unsigned char _body[0x600];                                                      // +0x004
    unsigned long m_blockCount;                                                      // +0x604
};

void CVtable_012b9174::Uninitialize()
{
    ReleaseBuffer();
}