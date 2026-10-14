// CAnimateThing::ResetTransistionAnimationModes(EAnimUpdateMode from, EAnimUpdateMode to) -- rewrite every queued delayed animation whose mode is `from` to `to`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C0A0, length 35 bytes (+ int3 padding).
// Callees: none.
// Layout: the retail CAnimateThing has CCompleteAnimation at +0x00 and the std::list<CDelayedAnimation>
// DelayedAnimQueue at +0x50 (PDB +0x78, the PDB CCompleteAnimation being wider); the list's head node pointer is
// at +0x54 and each node's value starts at +0x08 with the update mode.
// Shape: the usual rotated list walk -- the head is cached for the first test, but re-read from `this`
// after every iteration because the store into the node may alias it; `ret 8`: two enum arguments.
// VC6-era Dinkumware std::list: a 12-byte object {allocator word, head node, size}; the head's next is begin().
template<class T> struct CRetailList {
    struct SNode { SNode* next; SNode* prev; T value; };
    void* m_allocator;                          // +0x00
    SNode* m_head;                              // +0x04
    unsigned long m_size;                       // +0x08
};

enum EAnimUpdateMode { AUM_NONE };

struct CDelayedAnimation {                      // 12 bytes
    EAnimUpdateMode UpdateMode;                 // +0x00
    long m_value4;
    long m_value8;
};

struct CAnimateThing {
    unsigned char CurrentAnim[0x50];            // +0x00 CCompleteAnimation (retail size)
    CRetailList<CDelayedAnimation> DelayedAnimQueue;    // +0x50 (head at +0x54)
    void ResetTransistionAnimationModes(EAnimUpdateMode from, EAnimUpdateMode to);
};

void CAnimateThing::ResetTransistionAnimationModes(EAnimUpdateMode from, EAnimUpdateMode to)
{
    for (CRetailList<CDelayedAnimation>::SNode* node = DelayedAnimQueue.m_head->next;
         node != DelayedAnimQueue.m_head; node = node->next) {
        if (node->value.UpdateMode == from)
            node->value.UpdateMode = to;
    }
}
