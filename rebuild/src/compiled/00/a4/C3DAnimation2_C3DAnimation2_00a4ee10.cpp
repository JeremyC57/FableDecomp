// C3DAnimation2::C3DAnimation2() -- zero the animation state, create the instance list sentinel and a 256-entry x 4096 CSimpleMemoryPool allocator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4EE10, length 176 bytes.
// Identity: stores the C3DAnimation2 vtables 0x0129D548 (+0) and 0x0129D534 (the CMovableResource subobject at +0x2c,
// after that base's inline 0x0129B860 store); Ego_r.pdb ??0C3DAnimation2@@QAE@XZ.
// Callees: C3DAnimationStats::C3DAnimationStats @ 0x00A4EB00 (stores vtable 0x0129D514; catalog "CRegionDef" is
// propagated), the STLport node allocator @ 0x00BFEA0E (0x10-byte list node), operator new @ 0x00BFEA1A,
// CSimpleMemoryPool::CSimpleMemoryPool(256, 4096) @ 0x00A5C570, CCountedPointer<CSimpleMemoryPool>::Reset(T*) @ 0x00A52BE0
// (catalog "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset" is a byte twin).
// Layout: retail offsets are the PDB's minus 4 (C3DAnimationStats base 0x2c): Allocator +0x30, MemoryPool +0x38,
// MemoryHandle +0x3c, MovementTrack +0x40, MovementTrackData +0x48, SamplesPerSecond +0x50, FrameCount +0x54,
// SequenceCount +0x58 .. RigidBonePositions +0x70, LockedFromMemMemoryRelocation +0x74, InstanceList +0x78.
// Shape: init-list zeroes in member order (one zero register), FrameCount / SamplesPerSecond / the base word +8
// assigned in the body, then `Allocator.Reset(new CSimpleMemoryPool(256, 4096))`.
// Leaf: ??0C3DAnimation2
class CCPPointerInfo;
template<class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Reset(T* object);                          // 0x00A52BE0
};

class CSimpleMemoryPool { public:
    CSimpleMemoryPool(unsigned long count, unsigned long size);   // 0x00A5C570
    unsigned char _body[0x1c];
};

class C3DAnimationSequence;
class C3DAnimationSequenceData;
class C3DVector;
class CMovableResourceMemoryPool;
struct CIntelligentPointerNode {                    // list<CIntelligentPointer<C3DAnimation2> > node (0x10)
    CIntelligentPointerNode* _M_next;
    CIntelligentPointerNode* _M_prev;
    unsigned char _M_data[8];
};
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (MSVCR71)
struct CInstanceList {                              // STLport list: the sentinel node pointer
    CIntelligentPointerNode* _M_node;
    CInstanceList() : _M_node(0)
    {
        CIntelligentPointerNode* node = (CIntelligentPointerNode*)malloc(sizeof(CIntelligentPointerNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

class C3DAnimationStats { public:
    C3DAnimationStats();                            // 0x00A4EB00
    virtual ~C3DAnimationStats();
    long _04;
    long m_08;                                      // +0x08
    unsigned char _0c[0x20];
};
class CMovableResource { public:
    CMovableResource() {}
    virtual void RelocateData();
};

class C3DAnimation2 : public C3DAnimationStats, public CMovableResource { public:
    CCountedPointer<CSimpleMemoryPool> Allocator;               // +0x30
    CMovableResourceMemoryPool* MemoryPool;                     // +0x38
    long MemoryHandle;                                          // +0x3c
    CCountedPointer<C3DAnimationSequence> MovementTrack;        // +0x40
    CCountedPointer<C3DAnimationSequenceData> MovementTrackData;// +0x48
    float SamplesPerSecond;                                     // +0x50
    unsigned long FrameCount;                                   // +0x54
    unsigned long SequenceCount;                                // +0x58
    C3DAnimationSequence* Sequences;                            // +0x5c
    unsigned long SequenceDataBufferSize;                       // +0x60
    C3DAnimationSequenceData* SequenceDataBuffer;               // +0x64
    bool* BoneMask;                                             // +0x68
    bool* RigidBoneMask;                                        // +0x6c
    C3DVector* RigidBonePositions;                              // +0x70
    bool LockedFromMemMemoryRelocation;                         // +0x74
    CInstanceList InstanceList;                                 // +0x78
    C3DAnimation2();
    virtual ~C3DAnimation2();
    virtual void RelocateData();
};

C3DAnimation2::C3DAnimation2()
    : MemoryPool(0), MemoryHandle(-1), SequenceCount(0), Sequences(0), SequenceDataBufferSize(0),
      SequenceDataBuffer(0), BoneMask(0), RigidBoneMask(0), RigidBonePositions(0), LockedFromMemMemoryRelocation(false)
{
    FrameCount = 0;
    SamplesPerSecond = 0;
    m_08 = 0;
    Allocator.Reset(new CSimpleMemoryPool(256, 4096));
}