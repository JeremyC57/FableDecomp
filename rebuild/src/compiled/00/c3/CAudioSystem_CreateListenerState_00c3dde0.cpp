// CAudioSystem::CreateListenerState() -- EH-framed factory for the software EAX listener state: with a positive budget (+0x78) recycle one from the free chain of the +0x58 node pool (0 when empty); with a budget of -1 allocate a fresh 0x80-byte one through the library allocator; otherwise latch error -9. A created state that fails Init is reported and discarded; the result is pushed on the pool's active chain
// Leaf: ?CreateListenerState@CAudioSystem@@QAEPAVCListenerState@@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3DDE0, 291 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 440 "0xC3DDE0 "CTCHeroStats::CreateScreenEffectAction" = libpk pooled-object
//   factory (0x143BB58)"):
//   - `this` is the audio system singleton 0x0143BB58 (CAudioSystem: SetError 0x00C3D090 latches 0x0143BB48 and calls the
//     error callback at +0x7c = 0x0143BBD4, the two addresses this body reads);
//   - its only caller is agent440's CEAXManager::SetListenerProperty 0x00C52980, which calls it (bound there as
//     ?Create@CListenerStateFactory@@) when the manager has no listener state yet, then SetBlock 0x00C40830 (copies the
//     0x6c-byte EAX listener block into +0x0c) and Refresh 0x00C40D10 on the result;
//   - the product: 0x80 bytes, vtable 0x012B5A84, built on the pooled-node base (ctor 0x00C4CF20: vtable 0x012B8778, prev and
//     next 0; the CNodePool chains link it through CPooledNode::Link 0x00C4CED0), +0x7c zeroed; Init 0x00C40750 creates its
//     effect object through the audio master 0x0143BB5C and resets the 27-word property block to the defaults;
//   - the pool calls are the CNodePool renames GetFree 0x00C3C9A0, PopFree 0x00C3CA30, PushActive 0x00C3C9B0; the discard
//     0x00C3D6A0 (agent438's DiscardScreenEvent) releases the state (0x00C40800) and returns it to the same +0x58 pool.
//   The CTCHeroStats name is a propagated game label; descriptive names (libpk, no PDB/RTTI), agent440's CListenerState.
// Shape: the error latch is the inline SetError on the global object (absolute 0x0143BB48 / 0x0143BBD4 loads), not on
//   `this`; `new` through the class's static operator new 0x00C3C360 with the EH state around the inline constructor.
class CPooledNode {                                 // vtable 0x012B8778: the pooled-node base
public:
    CPooledNode();                                  // 0x00C4CF20
    virtual void v00();
    CPooledNode* m_pPrev;                           // +0x04
    CPooledNode* m_pNext;                           // +0x08
};

class CListenerState : public CPooledNode {         // vtable 0x012B5A84, 0x80 bytes
public:
    CListenerState() : m_7c(0) { }
    virtual void v00();
    long Init();                                    // 0x00C40750: 0 on success, else the error
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360 (the library allocator)
    unsigned char m_body[0x7c - 0x0c];              // +0x0c: the 0x6c-byte EAX listener block, dirty bits +0x78
    long          m_7c;                             // +0x7c
};

class CNodePool {                                   // 0x14 bytes (vtable 0x012B586C)
public:
    CPooledNode* GetFree();                         // 0x00C3C9A0
    void PushActive(CPooledNode* node);             // 0x00C3C9B0
    void PopFree();                                 // 0x00C3CA30
    unsigned char m_body[0x14];
};

typedef void (__stdcall* TAudioErrorCallback)(long code);
extern long g_lastError_0143bb48;                   // 0x0143BB48: the first error, latched

class CAudioSystem {                                // the audio system singleton @ 0x0143BB58
public:
    unsigned char       _pad00[0x58];
    CNodePool           m_listenerPool;             // +0x58
    unsigned char       _pad6c[0x78 - 0x6c];
    long                m_listenerBudget;           // +0x78 (>0: pooled, -1: allocate on demand)
    TAudioErrorCallback m_pfnErrorCallback;         // +0x7c (0x0143BBD4)
    void SetError(long code)                        // out of line at 0x00C3D090
    {
        if (g_lastError_0143bb48 == 0) {
            g_lastError_0143bb48 = code;
            if (m_pfnErrorCallback)
                m_pfnErrorCallback(code);
        }
    }
    void DiscardListenerState(CListenerState* state);   // 0x00C3D6A0
    CListenerState* CreateListenerState();
};
extern CAudioSystem g_audioSystem;                  // 0x0143BB58

CListenerState* CAudioSystem::CreateListenerState()
{
    CListenerState* state = 0;
    if (m_listenerBudget > 0) {
        state = (CListenerState*)m_listenerPool.GetFree();
    } else if (m_listenerBudget == -1) {
        state = new CListenerState;
    } else {
        g_audioSystem.SetError(-9);
    }
    if (!state)
        return 0;
    long error = state->Init();
    if (error) {
        g_audioSystem.SetError(error);
        DiscardListenerState(state);
        state = 0;
    }
    if (m_listenerBudget > 0)
        m_listenerPool.PopFree();
    m_listenerPool.PushActive(state);
    return state;
}
