// CAudioPlatform::~CAudioPlatform() -- EH-framed destructor of the libpk platform layer: its own vtable (0x012B5B80), Shutdown on `this` (0x00C4D1F0, called directly, EH state 4), the four 0x14-byte node pools at +0x40, +0x2c, +0x18 and +4 destroyed out of line in reverse order (CNodePool::~CNodePool 0x00C3CAA0, states 3..0), then the CPKObject root vtable (0x012B5868) from its inlined empty destructor
// Leaf: ??1CAudioPlatform@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3E2A0, 122 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 440 "0xC3E2A0 "CTCFishingSpot" dtor = CAudioPlatform dtor"):
//   - the vtable it stores, 0x012B5B80, is agent440's CAudioPlatform vtable: slot 3 is the landed CAudioPlatform::Shutdown
//     0x00C4D1F0, the function this destructor calls directly (a virtual called on `this` in a destructor is bound
//     statically), and slot 0 is the scalar deleting destructor 0x00C3E380 (`call 0x00C3E2A0 ; test [esp+8],1 ;
//     call 0x00C3C370`, CPKObject::operator delete);
//   - the four 0x14-byte members are the node pools Shutdown clears (CNodePool::Clear 0x00C3C930 on +4, +0x18, +0x2c,
//     +0x40; AcquireVoiceA 0x00C4D520 uses +0x2c and AcquireVoiceB 0x00C4D5F0 +0x40). Their destructor 0x00C3CAA0 stores
//     the pool vtable 0x012B586C, calls Clear 0x00C3C930 when the pool is still in use, and restores 0x012B5868;
//   - 0x012B5868 is the library root CPKObject (landed CEaxContext rows: virtual destructor, operator new 0x00C3C360 /
//     delete 0x00C3C370).
//   The landed CTCFishingSpot_00c3e2a0 / catalog CVtable_012b5868 names are propagated labels (no PDB/RTTI covers libpk);
//   descriptive names, agent440's class name, the pool named as in the CNodePool renames (agent440: CVoicePool).
// Shape: four separate members (an array would go through `__ehvec_dtor`), so each has its own EH state; the root's
//   destructor is inline and empty but its vtable store survives.
class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CNodePool : public CPKObject {                // vtable 0x012B586C, 0x14 bytes: an active chain and a free chain
public:
    ~CNodePool();                                   // 0x00C3CAA0
    void* m_pActiveHead;                            // +0x04
    void* m_pFreeHead;                              // +0x08
    long  m_freeCount;                              // +0x0c
    long  m_activeCount;                            // +0x10
};

class CAudioPlatform : public CPKObject {           // vtable 0x012B5B80
public:
    ~CAudioPlatform();                              // slot 0 (scalar deleting destructor 0x00C3E380)
    virtual void v01(long);                         // slot 1: 0x00C3E320 (`ret 4`)
    virtual void v02();                             // slot 2: 0x00C3E330 (jmp 0x00C4D440)
    virtual void Shutdown();                        // slot 3: 0x00C4D1F0
    virtual long v04();                             // slot 4: 0x00C3E340 (return 0)
    virtual long v05();                             // slot 5: 0x00C3E350 (return 0)
    virtual long v06();                             // slot 6: 0x00C3E360 (return 0)
    virtual long v07();                             // slot 7: 0x00C3E370 (return 0)
    CNodePool m_pool04;                             // +0x04
    CNodePool m_pool18;                             // +0x18
    CNodePool m_poolA;                              // +0x2c (AcquireVoiceA)
    CNodePool m_poolB;                              // +0x40 (AcquireVoiceB)
};

CAudioPlatform::~CAudioPlatform()
{
    Shutdown();
}
