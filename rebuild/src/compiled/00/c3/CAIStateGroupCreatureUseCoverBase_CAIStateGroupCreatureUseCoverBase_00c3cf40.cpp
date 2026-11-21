// CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase() -- EH-framed destructor: own vtable (0x012B5A88), an out-of-line cleanup on `this` (@ 0x00C44170, state 1), the member at +0x54 destroyed (@ 0x00C44550, state 0), then the out-of-line base destructor (@ 0x00C4CF40)
// Leaf: ??1CAIStateGroup_CreatureUseCoverBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CF40, oracle length 92 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],1 ; call Cleanup ; lea ecx,[esi+54h] ; mov byte ptr [esp+10h],0 ; call ~CMember ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Two tracked objects (the member and the base) give EH state 1 during the body and state 0
// (narrowed to a byte) during the member destructor. The class comes from the manifest label.
// NF219 (q12388): the +0x54 member is the audio object's CircularQueue (the landed sibling model of 0x00C437C0 / 0x00C43810;
//   the vtable 0x012B5A88 is the one 0x00C437C0 names for CAudioObject). Its inline destructor `~CircularQueue() { Release(); }`
//   expands here to the direct call of CircularQueue::Release 0x00C44550, and the unwind funclet 0x01214F88 calls its out-of-line
//   copy ??1CircularQueue@@QAE@XZ, retail 0x00C3CF30 (`jmp Release`): this object's pick-any copy equals those 5 bytes.
//   Was the stand-in member CCoverMember_00c3cf40 (one ~ symbol bound at both 0x00C44550 and 0x00C3CF30).
struct CAIStateGroupBase_00c3cf40 {
    virtual void v00();
    ~CAIStateGroupBase_00c3cf40();         // 0x00C4CF40
};

class CSoundBuffer;

class CircularQueue {                      // +0x54 (the landed sibling 0x00C437C0 / 0x00C43810 model)
public:
    ~CircularQueue() { Release(); }        // out of line: 0x00C3CF30 (`jmp Release`, the unwind funclet 0x01214F88's target)
    void Release();                        // 0x00C44550
    CSoundBuffer**    m_items;             // +0x00
    unsigned __int64* m_times;             // +0x04
    unsigned long     m_capacity;          // +0x08
    unsigned long     m_head;              // +0x0c
    unsigned long     m_tail;              // +0x10
    CSoundBuffer*     m_lastItem;          // +0x14
    unsigned long     m_stats[3];          // +0x18
};

struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase_00c3cf40 {   // vtable 0x012B5A88
    unsigned char _pad[0x50];
    CircularQueue m_queue;                 // +0x54
    void Cleanup();                        // 0x00C44170
    ~CAIStateGroup_CreatureUseCoverBase();
};

CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase()
{
    Cleanup();
}