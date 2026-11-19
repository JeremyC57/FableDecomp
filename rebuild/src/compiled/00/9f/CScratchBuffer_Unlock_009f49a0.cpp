// CScratchBuffer::Unlock() -- release the scratch buffer: Locked (+0x0c) = false
// Leaf: ?Unlock@CScratchBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F49A0 (`mov byte ptr [ecx+0Ch],0 ; ret`).
// NF95 (part 2, q11394): was the anonymous stand-in C009f49a0::M (catalog SetMemberConst_009f49a0). Name: Ego_r.pdb 0x00B48610 +
//   FableWin.pdb ?Unlock@CScratchBuffer@@QAEXXZ; every landed caller (0x00416148, 0x004161A7, 0x00A50C20, 0x00A98910; xref 5 sites)
//   already binds it so, each right after a Lock 0x009F4980 on g_ScratchBuffer 0x013CB498. Layout: Ego_r.pdb struct_layouts
//   (0x14 bytes: vptr, +0x08 Buffer, +0x0c Locked, +0x10 NoBytes; the +0x04 word unnamed). No shared header: local model as the
//   landed 0x00A50C20 one.
class CScratchBuffer {
public:
    virtual ~CScratchBuffer();
    bool Lock(void** buffer);                                // 0x009F4980
    void Unlock();
private:
    long           _pad04;
    unsigned char* Buffer;                                   // +0x08
    bool           Locked;                                   // +0x0c
    long           NoBytes;                                  // +0x10
};

void CScratchBuffer::Unlock()
{
    Locked = false;
}
