// C3DPrimitive2::UploadStaticBuffers(unsigned long flags) -- upload the vertex and index buffers once, optionally drop the device-independent data, mark uploaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A93320, oracle length 83 bytes.
// Callees: C3DPrimitive2::UploadToVertexBufferStatic(ulong) @ 0x00A931F0, C3DPrimitive2::UploadToIndexBuffer(ulong) @ 0x00A8F9E0,
//          the virtual deleting destructor of N3DPrimitive::CDeviceIndependent (slot 0, `push 1 ; call [eax]`).
// Caller: C3DPrimitive2::UploadBuffers @ 0x00A93380 calls this one-argument overload for the plain (non-animated,
// non-repeating) case; the manifest label "UploadRepeatedBuffers" is propagated from the byte-identical two-argument
// body @ 0x00A932C0 (which differs only by its `ret 8`), so this file names it for the path it serves.
//
// Retail bytes:  push ebx ; mov ebx,[esp+8] ; push esi ; mov esi,ecx ; test byte ptr [esi+38h],1 ; jne V ; push ebx ; call UploadToVertexBufferStatic ;
//                test al,al ; je F ; V: test byte ptr [esi+3Ch],1 ; jne I ; push ebx ; mov ecx,esi ; call UploadToIndexBuffer ; test al,al ; jne I ;
//                F: pop esi ; xor al,al ; pop ebx ; ret 4 ; I: test bl,8 ; je D ; mov ecx,[esi+2Ch] ; test ecx,ecx ; mov dword ptr [esi+2Ch],0 ; je D ;
//                mov eax,[ecx] ; push 1 ; call [eax] ; D: mov byte ptr [esi+64h],1 ; pop esi ; mov al,1 ; pop ebx ; ret 4
// Retail layout (PDB struct_layouts_egor.tsv, 4 bytes lower after the 12-byte retail CArray at +0x20):
// +0x2C Data (scoped_ptr<N3DPrimitive::CDeviceIndependent>), +0x38 VertexBufferStatus, +0x3C IndexBufferStatus
// (bit 0 = already uploaded), +0x64 UploadedFlag. Flag bit 3 of the init flags asks for the
// device-independent copy to be released once uploaded: the scoped_ptr reset nulls the member first and
// then deletes the old object through its virtual destructor.
struct CDeviceIndependent {
    virtual ~CDeviceIndependent();         // slot 0: the deleting destructor is called through the vtable
};

class C3DPrimitive2 { public:
    virtual void v00();
    unsigned char       _pad_0x04[0x2c - 0x04];
    CDeviceIndependent* Data;              // +0x2C  scoped_ptr<N3DPrimitive::CDeviceIndependent>
    unsigned char       _pad_0x30[0x38 - 0x30];
    unsigned long       VertexBufferStatus;// +0x38
    unsigned long       IndexBufferStatus; // +0x3C
    unsigned char       _pad_0x40[0x64 - 0x40];
    bool                UploadedFlag;      // +0x64

    bool UploadStaticBuffers(unsigned long flags);
    bool UploadToVertexBufferStatic(unsigned long flags);   // 0x00A931F0
    bool UploadToIndexBuffer(unsigned long flags);          // 0x00A8F9E0
};

bool C3DPrimitive2::UploadStaticBuffers(unsigned long flags)
{
    if (!(VertexBufferStatus & 1)) {
        if (!UploadToVertexBufferStatic(flags))
            return false;
    }
    if (!(IndexBufferStatus & 1)) {
        if (!UploadToIndexBuffer(flags))
            return false;
    }
    if (flags & 8) {
        CDeviceIndependent* old = Data;    // scoped_ptr::reset(): detach first, then delete
        Data = 0;
        delete old;
    }
    UploadedFlag = true;
    return true;
}