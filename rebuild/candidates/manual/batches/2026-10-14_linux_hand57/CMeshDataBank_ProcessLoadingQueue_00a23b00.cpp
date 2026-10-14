// CMeshDataBank::ProcessLoadingQueue(void* context, double time) -- run the three loading queues embedded at +0x170, +0x2e4 and +0x268 (0x009FC410 with (context, time) each)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23B00, length 80 bytes.
// Leaf: ProcessLoadingQueue
// Callees: 0x009FC410 (thiscall on each queue; the manifest label `_Cons_val<...>` is propagated). Shape: the context in edi, the double re-pushed through the FPU for each call, `ret 0Ch`.
struct CLoadingQueue_00a23b00 {
    unsigned char m_body[4];
    void Process(void* context, double time);      // 0x009FC410
};

struct CMeshDataBank {
    unsigned char          _pad00[0x170];
    CLoadingQueue_00a23b00 m_queueA;               // +0x170
    unsigned char          _pad174[0xf4];
    CLoadingQueue_00a23b00 m_queueC;               // +0x268
    unsigned char          _pad26c[0x78];
    CLoadingQueue_00a23b00 m_queueB;               // +0x2e4
    void ProcessLoadingQueue(void* context, double time);
};

void CMeshDataBank::ProcessLoadingQueue(void* context, double time)
{
    m_queueA.Process(context, time);
    m_queueB.Process(context, time);
    m_queueC.Process(context, time);
}
