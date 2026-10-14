// CBufferOwner_00c4b010::ReturnAllBuffers() -- hand back buffers (ReturnBuffer(0)) until the outstanding count at +0x68 is zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4B010, length 34 bytes.
// Callees: ReturnBuffer @ 0x00C4AC70 (on `this`, one argument: 0; manifest "PKCBufferedReader::ReturnBuffer"). Shape: the count is re-read after every call;
// the loop head is aligned with a `lea ebx,[ebx]` nop. The chunk label "CDrawGuildSeal::ClearMessages" is propagated; the class is named by address.
struct CBufferOwner_00c4b010 {
    unsigned char _pad[0x68];
    long m_outstanding;                            // +0x68
    void ReturnBuffer(long which);                 // 0x00C4AC70
    void ReturnAllBuffers();
};

void CBufferOwner_00c4b010::ReturnAllBuffers()
{
    while (m_outstanding != 0)
        ReturnBuffer(0);
}
