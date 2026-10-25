// CNodePool::~CNodePool() -- EH-framed destructor of the libpk 20-byte node pool: its own vtable (0x012B586C), Clear (0x00C3C930) when the pool still has active nodes or recycling is enabled, then the CPKObject root vtable (0x012B5868) from its inlined empty destructor
// Leaf: ??1CNodePool@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CAA0, length 86 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 440 "0xC3E2A0 = CAudioPlatform dtor" and the chunk 438 node
//   pool): this is the destructor of the four 0x14-byte pools of CAudioPlatform (its destructor 0x00C3E2A0 calls this on
//   +0x40/+0x2c/+0x18/+4; CAudioPlatform::Shutdown 0x00C4D1F0 calls CNodePool::Clear 0x00C3C930 on the same members, and
//   rebuild/oracles/reloc_bindings.tsv binds ?Clear@CVoicePool@@QAEXXZ / ?Clear@CSlot_00c3cb30@@QAEXXZ to 0x00C3C930) and of
//   the four pools of the 0x00C3CB30 owner. The pool constructor 0x00C3C8A0 stores the same 0x012B586C vtable and sets
//   the free count to -1 (Clear resets head/free head/active count to 0 and the free count to -1: "not recycling").
//   The landed CSlot_00c3cb30 name and the manifest's "CTCFishingSpot::Destructor_UnwindHelper" are propagated labels;
//   descriptive name, as in the CNodePool renames (GetFree 0x00C3C9A0, PushActive 0x00C3C9B0, Clear 0x00C3C930, ...).
// Layout: +0 is the vptr (the CNodePool rows' "m_pOwner" word), active head +4, free head +8, free count +0xc (negative:
//   recycling disabled), active count +0x10.
// Shape: `mov [esp+4],esi` saves `this` for the unwind; EH state 0 (the root sub-object) is stored between the count
//   load and its test; `m_activeCount != 0 || m_freeCount >= 0` as two branches to the call.
class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CPooledNode;

class CNodePool : public CPKObject {                // vtable 0x012B586C, 0x14 bytes: an active chain and a free chain
public:
    ~CNodePool();
    bool IsRecycling() const { return m_freeCount >= 0; }
    void Clear();                                   // 0x00C3C930
    CPooledNode* m_pActiveHead;                     // +0x04
    CPooledNode* m_pFreeHead;                       // +0x08
    long         m_freeCount;                       // +0x0c (negative: recycling disabled)
    long         m_activeCount;                     // +0x10
};

CNodePool::~CNodePool()
{
    if (m_activeCount != 0 || IsRecycling())
        Clear();
}
