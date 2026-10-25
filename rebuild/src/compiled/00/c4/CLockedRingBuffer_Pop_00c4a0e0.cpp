// CLockedRingBuffer::Pop() -- EH-framed: under the ring's own critical section (PKCProtectionInstance over +0), null when the ring is empty (count +0x20), else the frame at the head slot (index from GetHeadIndex 0x00C49DD0, slot array +0x1c) with the count decremented
// Leaf: ?Pop@CLockedRingBuffer@@QAEPAUCCodaFrame@@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4A0E0, 144 bytes, __thiscall `ret`; built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[esi+20h] ; test eax,eax ; mov dword ptr [esp+14h],0 ; jne pop ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; xor eax,eax ; pop esi ; ... ret ; pop: push edi ; mov ecx,esi ; call GetHeadIndex ; mov ecx,[esi+1Ch] ; mov edi,[ecx+eax*4] ; mov eax,[esi+20h] ; dec eax ; lea ecx,[esp+8] ; mov [esi+20h],eax ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+10h] ; mov eax,edi ; pop edi ; pop esi ; ... ret
// Naming (sweep1, 2026-10-25): was landed as CEntranceStack_00c4a0e0::Pop (manifest "EntranceManager_SetAsLocked",
//   propagated); FINDINGS chunk 439 ("0xC4A0E0 CEntranceStack = CLockedRingBuffer::Pop"). The layout is that of the
//   landed CLockedRingBuffer::Push 0x00C49D50 (critical section +0, capacity +0x18, slots +0x1c, count +0x20, tail
//   +0x24) and the head index comes from the landed CLockedRingBuffer::GetHeadIndex 0x00C49DD0; the caller 0x00C497E8 is
//   in the same CCodaFrame streamer TU. The scoped lock is the sweep's PKCProtectionInstance (".\PKCProtectionInstance.cpp"
//   trace in its destructor 0x00C0FFE0); the ring's section is the PKCCriticalSection at +0. Descriptive (no PDB).
// Shape: the edi save is sunk into the non-empty path; the frame survives the lock's destructor in edi.
class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
};

class PKCProtectionInstance {                   // scoped guard: section + entry tick
public:
    PKCCriticalSection* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);   // 0x00C0FFC0
    ~PKCProtectionInstance();                   // 0x00C0FFE0
};

struct CCodaFrame;

struct CLockedRingBuffer {
    PKCCriticalSection m_lock;                  // +0x00
    long          m_capacity;                   // +0x18
    CCodaFrame**  m_pSlots;                     // +0x1c
    long          m_count;                      // +0x20
    long          m_tail;                       // +0x24
    void Push(CCodaFrame* frame);               // 0x00C49D50
    long GetHeadIndex();                        // 0x00C49DD0
    CCodaFrame* Pop();
};

CCodaFrame* CLockedRingBuffer::Pop()
{
    PKCProtectionInstance lock(&m_lock);
    if (m_count == 0)
        return 0;
    CCodaFrame* frame = m_pSlots[GetHeadIndex()];
    --m_count;
    return frame;
}
