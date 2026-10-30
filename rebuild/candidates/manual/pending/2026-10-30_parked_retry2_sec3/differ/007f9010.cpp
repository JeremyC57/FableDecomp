// CTCAnimatingObject::PlayAnimation(long anim, long priority, bool looping) -- unless an animation of at least this priority is playing, drop the queued animations and play `anim` (update mode 1, type 6, priority 100 in the init record) at `priority`
// Grade: DIFFER(131v131) -- wave 2 sec3: PDB spellings applied (inline ctor CAnimInit(unsigned char, unsigned long, long, EAnimUpdateMode, bool, unsigned long, long) calling CAnimInit::Init 0x0070C050; private ClearQueuedAnims / PlayAnimation(const CAnimInit&, long) AAE); residue unchanged: priority in ebp (retail ebx). Tried: PDB signatures; parked tries: temporary vs named CAnimInit, default ctor + Set, early return
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

enum EAnimUpdateMode { ANIM_UPDATE_MODE_1 = 1 };
class CAnimFrameInfo {
public:
    long Frame;
    long SubFrame;
    CAnimFrameInfo() : Frame(0), SubFrame(0) {}
};
class CAnimInit {
public:
    EAnimUpdateMode UpdateMode;                 // +0x00
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
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority)
    {
        Init(type, typeFlags, anim, updateMode, looping, flags, priority);
    }
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority);   // 0x0070C050 (Ego_r ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z)
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
    void PlayAnimation(long anim, long priority, bool looping);
private:
    void ClearQueuedAnims() { QueuedAnims.clear(); }
    void PlayAnimation(const CAnimInit& init, long priority);   // 0x007F8DC0 (Ego_r private AAE)
};

void CTCAnimatingObject::PlayAnimation(long anim, long priority, bool looping)
{
    if (!Animating || priority > CurrentAnimationPriority) {
        ClearQueuedAnims();
        CAnimInit init(6, 0, anim, ANIM_UPDATE_MODE_1, looping, 0, 100);
        PlayAnimation(init, priority);
    }
}
