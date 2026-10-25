// CNodePool::CNodePool() -- construct an idle libpk node pool: its vtable (0x012B586C), empty active and free chains, free count -1 (recycling disabled until Init), active count 0
// Leaf: ??0CNodePool@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C8A0, length 27 bytes (+ int3 padding), thiscall, no callees.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 440 "0xC3E2A0 = CAudioPlatform dtor"): CAudioPlatform::
//   CAudioPlatform 0x00C4D000 builds its four 0x14-byte pools (+4/+0x18/+0x2c/+0x40) with this, and its destructor
//   0x00C3E2A0 destroys the same members with the pool destructor 0x00C3CAA0, which stores the same 0x012B586C vtable
//   and calls CNodePool::Clear 0x00C3C930 (which restores exactly this state: chains and active count 0, free count -1).
//   The other caller is the 0x00C3D7D0 owner's constructor (four pools at +0x1c..+0x58). The landed
//   SVtableRecord_00c3c8a0 name and the catalog's "CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo" are propagated;
//   descriptive name, as in the CNodePool renames.
// Shape: `xor ecx,ecx` before the vtable store, then the members in order with -1 as an immediate: a plain initialiser
//   list. The library root CPKObject (vtable 0x012B5868) has an inline empty virtual destructor (its destructors store
//   0x012B5868, e.g. CNodePool::~CNodePool 0x00C3CAA0); VC7.1 drops the inlined root constructor's vtable store, which
//   the derived store overwrites, so no store of 0x012B5868 appears here.
class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CPooledNode;

class CNodePool : public CPKObject {                // vtable 0x012B586C, 0x14 bytes: an active chain and a free chain
public:
    CNodePool();
    ~CNodePool();                                   // 0x00C3CAA0
    CPooledNode* m_pActiveHead;                     // +0x04
    CPooledNode* m_pFreeHead;                       // +0x08
    long         m_freeCount;                       // +0x0c (negative: recycling disabled)
    long         m_activeCount;                     // +0x10
};

CNodePool::CNodePool() : m_pActiveHead(0), m_pFreeHead(0), m_freeCount(-1), m_activeCount(0)
{
}
