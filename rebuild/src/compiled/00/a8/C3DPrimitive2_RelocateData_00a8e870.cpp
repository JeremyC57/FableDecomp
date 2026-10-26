// C3DPrimitive2::RelocateData(long, void* oldBase, void* newBase, long) -- rebase the StaticBlocks and AnimatedBlocks pointers when they are set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8E870, oracle length 39 bytes.
// Callees: none. Virtual (the manifest signature `RelocateData(long, void*, void*, long)` is shared with
// C3DMeshStats::RelocateData @ 0x00A2E820, which rebases its own pointers the same way).
//
// Retail bytes:  mov eax,[ecx+44h] ; test eax,eax ; mov edx,[esp+0Ch] ; push esi ; mov esi,[esp+0Ch] ; je A ; sub eax,esi ; add eax,edx ;
//                mov [ecx+44h],eax ; A: mov eax,[ecx+4Ch] ; test eax,eax ; je B ; sub eax,esi ; add eax,edx ; mov [ecx+4Ch],eax ; B: pop esi ; ret 10h
// Retail layout (PDB struct_layouts_egor.tsv, 4 bytes lower after the 12-byte retail CArray at +0x20):
// +0x44 StaticBlocks (CStaticBlock*), +0x4C AnimatedBlocks (CAnimatedBlock*). A non-null block pointer is
// moved by the same delta as the owning memory block: p = p - oldBase + newBase. The handle and size
// arguments are unused by this override.
struct CStaticBlock;
struct CAnimatedBlock;

class C3DPrimitive2 { public:
    virtual void v00();
    unsigned char   _pad_0x04[0x40];       // +0x04 .. +0x44: MaterialIndex .. StaticBlockCount
    CStaticBlock*   StaticBlocks;          // +0x44
    unsigned long   AnimatedBlockCount;    // +0x48
    CAnimatedBlock* AnimatedBlocks;        // +0x4C
    virtual void RelocateData(long handle, void* oldBase, void* newBase, long size);
};

void C3DPrimitive2::RelocateData(long /*handle*/, void* oldBase, void* newBase, long /*size*/)
{
    if (StaticBlocks)
        StaticBlocks = reinterpret_cast<CStaticBlock*>(reinterpret_cast<char*>(StaticBlocks) - reinterpret_cast<char*>(oldBase) + reinterpret_cast<char*>(newBase));
    if (AnimatedBlocks)
        AnimatedBlocks = reinterpret_cast<CAnimatedBlock*>(reinterpret_cast<char*>(AnimatedBlocks) - reinterpret_cast<char*>(oldBase) + reinterpret_cast<char*>(newBase));
}