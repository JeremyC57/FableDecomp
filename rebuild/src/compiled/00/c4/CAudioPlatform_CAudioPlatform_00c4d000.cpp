// CAudioPlatform::CAudioPlatform() -- EH-framed constructor of the libpk platform layer: its vtable (0x012B5B80) and the four 0x14-byte node pools at +4, +0x18, +0x2c, +0x40, each built with CNodePool::CNodePool (0x00C3C8A0; EH states 0..3)
// Leaf: ??0CAudioPlatform@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4D000, 108 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 440 "0xC3E2A0 = CAudioPlatform dtor"): it stores the
//   CAudioPlatform vtable 0x012B5B80 (slot 3 Shutdown 0x00C4D1F0, slot 0 the deleting destructor 0x00C3E380 around the
//   destructor 0x00C3E2A0) and builds the same four pool members the destructor tears down (+4/+0x18/+0x2c/+0x40) with
//   the pool constructor 0x00C3C8A0 (stores the CNodePool vtable 0x012B586C, zeroes the chains and counts, free count
//   -1). The landed TeleportData_00c4d000 name is a propagated manifest label; descriptive names (no PDB/RTTI covers libpk).
// Shape: EH state 0 is entered before the first member exists (the inlined root constructor's vtable store of
//   0x012B5868 is dropped by VC7.1 because the derived store overwrites it; the root's inline empty destructor is what
//   the destructor 0x00C3E2A0 ends with), then one state per constructed pool.
class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CNodePool : public CPKObject {                // vtable 0x012B586C, 0x14 bytes
public:
    CNodePool();                                    // 0x00C3C8A0
    ~CNodePool();                                   // 0x00C3CAA0
    void* m_pActiveHead;                            // +0x04
    void* m_pFreeHead;                              // +0x08
    long  m_freeCount;                              // +0x0c
    long  m_activeCount;                            // +0x10
};

class CAudioPlatform : public CPKObject {           // vtable 0x012B5B80
public:
    CAudioPlatform();
    ~CAudioPlatform();                              // 0x00C3E2A0 (slot 0: deleting destructor 0x00C3E380)
    virtual void v01(long);                         // slot 1: 0x00C3E320
    virtual void v02();                             // slot 2: 0x00C3E330
    virtual void Shutdown();                        // slot 3: 0x00C4D1F0
    virtual long v04();                             // slot 4: 0x00C3E340
    virtual long v05();                             // slot 5: 0x00C3E350
    virtual long v06();                             // slot 6: 0x00C3E360
    virtual long v07();                             // slot 7: 0x00C3E370
    CNodePool m_pool04;                             // +0x04
    CNodePool m_pool18;                             // +0x18
    CNodePool m_poolA;                              // +0x2c (AcquireVoiceA)
    CNodePool m_poolB;                              // +0x40 (AcquireVoiceB)
};

CAudioPlatform::CAudioPlatform()
{
}
