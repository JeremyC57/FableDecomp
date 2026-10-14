// CDragSource_00c20340::BeginDrag(long) -- submit the drag either on this object directly (when the ready flag at +0xc is set) or on a 12-byte descriptor built from this object and the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20340, length 38 bytes.
// Callees: the descriptor builder @ 0x00C257B0 (`__fastcall`: hidden 12-byte result in ecx, this in edx, the argument on the stack; manifest
// "CGuiWindow::OnBeginDrag" is propagated), the submit @ 0x00C25970 (`__fastcall` on a pointer; manifest "Math_Vector3Length" is propagated).
// Shape: the two submit calls are tail-merged: the flag path reaches the call with ecx still = `this`, the other with `lea ecx,[esp]` (the
// descriptor local in the 12-byte frame). The class is named by address (the chunk label "CDraw2DText::ClearPrimitivesIfNecessary" is propagated).
struct CDragSource_00c20340;

struct SDragDescriptor_00c20340 {
    long m_a, m_b, m_c;
    SDragDescriptor_00c20340() { }
    SDragDescriptor_00c20340(const SDragDescriptor_00c20340& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

SDragDescriptor_00c20340 __fastcall BuildDragDescriptor_00c257b0(CDragSource_00c20340* source, long arg);   // 0x00C257B0
void __fastcall SubmitDrag_00c25970(const void* dragTarget);                                               // 0x00C25970

struct CDragSource_00c20340 {
    unsigned char _pad[0x0c];
    unsigned char m_ready;                         // +0x0c
    void BeginDrag(long arg);
};

void CDragSource_00c20340::BeginDrag(long arg)
{
    if (m_ready) {
        SubmitDrag_00c25970(this);
    } else {
        SDragDescriptor_00c20340 descriptor = BuildDragDescriptor_00c257b0(this, arg);
        SubmitDrag_00c25970(&descriptor);
    }
}