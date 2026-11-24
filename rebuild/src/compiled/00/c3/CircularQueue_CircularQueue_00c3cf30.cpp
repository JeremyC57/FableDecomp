// CircularQueue::~CircularQueue() -- the queue's out-of-line destructor: Release the queue (VC7.1 tail-calls it, `jmp`)
// Leaf: ??1CircularQueue@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CF30, 5 bytes (`jmp 00C44550`).
// Identity: the owner's destructor 0x00C3CF40 (CAudioObject vtable 0x012B5A88) destroys its +0x54 CircularQueue by calling
//   CircularQueue::Release 0x00C44550 directly (the inline destructor expanded), while its unwind funclet 0x01214F88
//   (`mov ecx,[ebp-10h] ; add ecx,54h ; jmp 00C3CF30`) calls this out-of-line copy; 0x00C3CF40's object built on the
//   sibling model emits exactly this 5-byte body as its pick-any ??1CircularQueue@@QAE@XZ and binds the funclet to it.
//   Class name descriptive (libpk, no PDB), as the siblings 0x00C437C0 / 0x00C43810 / 0x00C442F0.
// Label correction: was the free-function forwarder _Forward -> ForwardTarget_00c44550 (catalog TailJmp_Release_00c3cf30).
class CSoundBuffer;

class CircularQueue {                               // the audio object's buffer queue (+0x54)
public:
    ~CircularQueue();
    void Release();                                 // 0x00C44550
    CSoundBuffer**    m_items;                      // +0x00
    unsigned __int64* m_times;                      // +0x04
    unsigned long     m_capacity;                   // +0x08
    unsigned long     m_head;                       // +0x0c
    unsigned long     m_tail;                       // +0x10
    CSoundBuffer*     m_lastItem;                   // +0x14
    unsigned long     m_stats[3];                   // +0x18
};

CircularQueue::~CircularQueue()
{
    Release();
}
