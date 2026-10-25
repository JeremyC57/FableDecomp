// CEAXManager::SetListenerProperty(const void* set, unsigned long id, void* data, unsigned long size) -- write a global (listener) EAX property: in software mode create the listener state on first use (from the manager's block), set the property on the listener effect and queue the commit of the flags it reports; in hardware mode forward to the hardware EAX object; 0 for any other mode
// Leaf: ?SetListenerProperty@CEAXManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52980, length 134 bytes (+ int3 padding); `ret 10h`; thiscall on the
// EAX manager (the class of this chunk's 0x00C4E380 / 0x00C51F00; global @ 0x0143BB70). No direct callers (a vtable or
// pointer use).
// Callees: the factory 0x00C3DDE0 on the global object 0x0143BB58 (landed as "CTCHeroStats_00c3dde0::
// CreateScreenEffectAction", propagated), the new state's 0x00C40830 (landed "SetProjection", copies the 0x6c-byte block)
// and 0x00C40D10, the listener effect's 0x00C68F90 (manifest "CDrawGuildSeal_SetPropertyEx"), QueueCommit 0x00C51A20
// with a null voice, and in hardware mode a tail jump to 0x00C3EB60 on the pointer 0x0143BB5C (landed
// "CScriptThing::func_00c3eb60").
// The catalog label "NormalizeProjectionAndSerialize" is propagated; descriptive name (libpk, no PDB/RTTI); "listener"
// is inferred from the null voice handed to the effect and to QueueCommit.
// Shape: the result accumulator with `break`s gives retail's `xor eax,eax ; sub ecx,eax` dispatch and the shared
// `ret 10h`; case 1 becomes `pop esi ; jmp` (identical stack arguments); `dirty` is homed in the unused `set` slot.
struct SListenerBlock { unsigned char _body[0x6c]; };

struct CListenerState {
    void SetBlock(const SListenerBlock* block);     // 0x00C40830
    void Refresh();                                 // 0x00C40D10
};

struct CListenerStateFactory {
    CListenerState* Create();                       // 0x00C3DDE0
};
extern CListenerStateFactory g_listenerStateFactory;   // 0x0143BB58

struct CHardwareEAX {
    long SetProperty(const void* set, unsigned long id, void* data, unsigned long size);   // 0x00C3EB60
};
extern CHardwareEAX* g_pHardwareEAX;                // 0x0143BB5C

struct CEAXListenerEffect {
    long SetProperty(void* voice, unsigned long id, void* data, unsigned long size, unsigned long* dirty);   // 0x00C68F90
};

struct CEAXVoice;

struct CEAXManager {
    unsigned char       _pad0[4];
    CListenerState*     m_pListenerState;           // +0x04
    SListenerBlock      m_listenerBlock;            // +0x08
    unsigned char       _pad74[0xe4 - 0x74];
    CEAXListenerEffect* m_pListenerEffect;          // +0xe4
    long                m_mode;                     // +0xe8 (0: software EAX, 1: hardware)
    void QueueCommit(unsigned long dirty, CEAXVoice* voice);   // 0x00C51A20
    long SetListenerProperty(const void* set, unsigned long id, void* data, unsigned long size);
};

long CEAXManager::SetListenerProperty(const void* set, unsigned long id, void* data, unsigned long size)
{
    long result = 0;
    switch (m_mode) {
    case 0: {
        if (!m_pListenerState) {
            m_pListenerState = g_listenerStateFactory.Create();
            m_pListenerState->SetBlock(&m_listenerBlock);
            m_pListenerState->Refresh();
        }
        unsigned long dirty = 0;
        result = m_pListenerEffect->SetProperty(0, id, data, size, &dirty);
        QueueCommit(dirty, 0);
        break;
    }
    case 1:
        result = g_pHardwareEAX->SetProperty(set, id, data, size);
        break;
    }
    return result;
}