// CTCAnimatingObject::PlayAnimation(long anim, long priority, bool looping) -- unless an animation of at least this priority is playing, drop the queued animations and play `anim` (update mode 1, type 6, priority 100 in the init record) at `priority`
// Grade: DIFFER(131v131) -- register choice only: retail keeps `priority` in ebx (push ebx first), ours in ebp; everything else identical | tried: temporary vs named CAnimInit (named fixed the push-0x64/head-store order), default ctor + Set, early-return form
// Leaf: ?PlayAnimation@CTCAnimatingObject@@QAEXJJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F9010, length 131 bytes (+13 int3 padding). `ret 0Ch`.
// Callees: free @ 0x00BFEA14 (the inline list clear: ClearQueuedAnims), CAnimInit::Set(type, typeFlags, anim, updateMode,
// looping, flags, priority) @ 0x0070C050 (landed as CInitRumble::Set), the private CTCAnimatingObject::PlayAnimation
// (const CAnimInit&, long) @ 0x007F8DC0.
// Shape: `!Animating || priority > CurrentAnimationPriority` with the priority kept in ebx; the STLport list<CAnimInit> clear
// re-reads the head after every free and relinks it to itself; CAnimInit's inline 7-argument ctor zeroes its CAnimFrameInfo
// (+0x18/+0x1C, between the pushes and the call) and calls the out-of-line Set.
// Layout: PDB CTCAnimatingObject (0x20): Animating +0xC, CurrentAnimationPriority +0x10, QueuedAnims +0x14 (STLport list, one
// node pointer); CAnimInit (0x2C): UpdateMode, Type, Looping, Anim, Speed, Alpha, LastAlpha, Frame +0x18, Flags, TypeFlags, Priority.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
class CThing;

struct CAnimFrameInfo {
    long Frame;
    long SubFrame;
    CAnimFrameInfo() : Frame(0), SubFrame(0) {}
};
struct CAnimInit {
    long UpdateMode;                            // +0x00 EAnimUpdateMode
    unsigned char Type;                         // +0x04
    bool Looping;                               // +0x05
    long Anim;                                  // +0x08
    float Speed;                                // +0x0c
    float Alpha;                                // +0x10
    float LastAlpha;                            // +0x14
    CAnimFrameInfo Frame;                       // +0x18
    unsigned long Flags;                        // +0x20
    unsigned long TypeFlags;                    // +0x24
    long Priority;                              // +0x28
    CAnimInit(long type, long typeFlags, long anim, long updateMode, bool looping, long flags, long priority)
    {
        Set(type, typeFlags, anim, updateMode, looping, flags, priority);
    }
    void Set(long type, long typeFlags, long anim, long updateMode, bool looping, long flags, long priority);   // 0x0070C050
};

struct CAnimInitListNode {
    CAnimInitListNode* _M_next;
    CAnimInitListNode* _M_prev;
};
struct CAnimInitList {                          // STLport list<CAnimInit>
    CAnimInitListNode* _M_node;
    void clear()
    {
        CAnimInitListNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            CAnimInitListNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCAnimatingObject : public CTCBase {
public:
    bool Animating;                             // +0x0c
    long CurrentAnimationPriority;              // +0x10
    CAnimInitList QueuedAnims;                  // +0x14
    void ClearQueuedAnims() { QueuedAnims.clear(); }
    void PlayAnimation(const CAnimInit& init, long priority);   // 0x007F8DC0
    void PlayAnimation(long anim, long priority, bool looping);
};

void CTCAnimatingObject::PlayAnimation(long anim, long priority, bool looping)
{
    if (!Animating || priority > CurrentAnimationPriority) {
        ClearQueuedAnims();
        CAnimInit init(6, 0, anim, 1, looping, 0, 100);
        PlayAnimation(init, priority);
    }
}
