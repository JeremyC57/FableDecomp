// CAudioOwner::WaitForSprite() -- poll the owner's optional sprite object (+0x60) under the library lock until it reports finished, sleeping 100 ms after every poll
// Flags: /EHsc
// Leaf: WaitForSprite
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C08AE0, 133 bytes (`ret`). Built under /EHsc. Descriptive names (no PDB covers
//   this library): the manifest's "EventQueue_ProcessBlockParrySuccessesWithTimeout" is propagated. The only caller is
//   the owner's destructor 0x00C09380 (landed as CCreatureActionHeroJump_00c09380; "Finish"), right after its Stop
//   (0x00C08A80); the +0x60 object is the sprite the landed SetSlot 0x00C0B380 and Update 0x00C0AF20 refresh.
// Callees: PKCProtectionInstance::PKCProtectionInstance @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0 over the global lock @ 0x0143BB14,
//   CSprite::IsFinished @ 0x00C15B00, Sleep (IAT 0x0143FE1C, held in edi across the loop).
// Shape: the lock lives in an inner scope (state 0 while it is alive, -1 before Sleep); the done flag is bl.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // IAT 0x0143FE1C

class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;       // 0x0143BB14

class PKCProtectionInstance { public:                            // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);     // 0x00C0FFC0
    ~PKCProtectionInstance();                             // 0x00C0FFE0
};

struct CSprite {
    bool IsFinished();                          // 0x00C15B00
};

struct CAudioOwner {
    unsigned char _pad00[0x60];
    CSprite* m_pSprite;                         // +0x60
    void WaitForSprite();
};

void CAudioOwner::WaitForSprite()
{
    bool done;
    do {
        done = true;
        {
            PKCProtectionInstance lock(&g_lock_0143bb14);
            if (m_pSprite && !m_pSprite->IsFinished())
                done = false;
        }
        Sleep(100);
    } while (!done);
}