// C3DAnimation2::RelocateData(long, void*, void*, long) -- rebase the animation's sequence/data pointers (and its shared instances') after the memory pool moved its block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4E960, length 352 bytes (`ret 10h`; CMovableResource virtual).
// Callees: C3DAnimationSequenceData::RelocateData(void*, void*) @ 0x00A4CD70 (landed as CAnimationData_00a4cd70; the
//          manifest "CParticleSimulation::RelocateData" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Shape: RelocateData overrides the CMovableResource base at +0x2c, so `this` is that subobject and every member
//          offset reads 0x2c lower than the full object (+0x1c MovementTrackData .. +0x4c InstanceList). Each pointer
//          is `p = p - old + new` under a null test; the movement track data is rebased out of line, the
//          SequenceDataBuffer records with the same code inlined; unshared Sequences own their data. Then every
//          instance in the list rebases the Sequences it shares with this animation. The list iterator lives
//          in the dead `newAddress` argument slot.
// Layout: retail C3DAnimation2 as established by the landed ctor 0x00A4EE10 (PDB minus 4: C3DAnimationStats base
//          0x2c, CMovableResource vptr +0x2c, MovementTrack +0x40, MovementTrackData +0x48, SequenceCount +0x58,
//          Sequences +0x5c, SequenceDataBufferSize +0x60, SequenceDataBuffer +0x64, BoneMask +0x68,
//          RigidBoneMask +0x6c, RigidBonePositions +0x70, InstanceList +0x78). The generated engine header
//          C3DAnimation2.h carries the PDB offsets (+0x30 shifted), so a local model is used.
class C3DVector;
class CCPPointerInfo;
class CSimpleMemoryPool;
class CMovableResourceMemoryPool;

template <class T> inline void RelocatePointer(T*& p, void* oldAddress, void* newAddress)
{
    if (p)
        p = (T*)((char*)p - (char*)oldAddress + (char*)newAddress);
}

class C3DAnimationSequenceData { public:              // 0x2c bytes
    char* BoneName;                             // +0x00
    unsigned char _04[0x10];
    void* Track14;                              // +0x14
    unsigned char _18[0x08];
    void* Track20;                              // +0x20
    void* Track24;                              // +0x24
    void* Track28;                              // +0x28
    void RelocateData(void* oldAddress, void* newAddress);   // 0x00A4CD70
    void RelocateDataInline(void* oldAddress, void* newAddress)
    {
        RelocatePointer(BoneName, oldAddress, newAddress);
        RelocatePointer(Track14, oldAddress, newAddress);
        RelocatePointer(Track20, oldAddress, newAddress);
        RelocatePointer(Track24, oldAddress, newAddress);
        RelocatePointer(Track28, oldAddress, newAddress);
    }
};

class C3DAnimationSequence { public:                  // 8 bytes (PDB)
    C3DAnimationSequenceData* Data;            // +0
    short ParentIndex;                         // +4
    bool  Shared;                              // +6
};

class CBaseObject;
struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pInfo;
    CBaseObject* GetPItem() const;             // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};

class C3DAnimation2;
struct CInstanceNode {                         // STLport list<CIntelligentPointer<C3DAnimation2> > node
    CInstanceNode* _M_next;
    CInstanceNode* _M_prev;
    CIntelligentPointer<C3DAnimation2> _M_data;
};

template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };

class C3DAnimationStats { public:                     // 0x2c-byte primary base
    virtual ~C3DAnimationStats();
    unsigned char _04[0x28];
};
class CMovableResource { public:                      // second base at +0x2c
    virtual void RelocateData(long handle, void* oldAddress, void* newAddress, long size);
};

class C3DAnimation2 : public C3DAnimationStats, public CMovableResource { public:
    CCountedPointer<CSimpleMemoryPool>        Allocator;           // +0x30
    CMovableResourceMemoryPool*               MemoryPool;          // +0x38
    long                                      MemoryHandle;        // +0x3c
    CCountedPointer<C3DAnimationSequence>     MovementTrack;       // +0x40
    CCountedPointer<C3DAnimationSequenceData> MovementTrackData;   // +0x48
    float                                     SamplesPerSecond;    // +0x50
    unsigned long                             FrameCount;          // +0x54
    unsigned long                             SequenceCount;       // +0x58
    C3DAnimationSequence*                     Sequences;           // +0x5c
    unsigned long                             SequenceDataBufferSize;  // +0x60
    C3DAnimationSequenceData*                 SequenceDataBuffer;  // +0x64
    bool*                                     BoneMask;            // +0x68
    bool*                                     RigidBoneMask;       // +0x6c
    C3DVector*                                RigidBonePositions;  // +0x70
    bool                                      LockedFromMemMemoryRelocation;   // +0x74
    CInstanceNode*                            InstanceList;        // +0x78 (STLport list: sentinel node)
    virtual void RelocateData(long handle, void* oldAddress, void* newAddress, long size);
};

void C3DAnimation2::RelocateData(long handle, void* oldAddress, void* newAddress, long size)
{
    if (MovementTrackData.Data)
        MovementTrackData.Data->RelocateData(oldAddress, newAddress);
    RelocatePointer(Sequences, oldAddress, newAddress);
    RelocatePointer(SequenceDataBuffer, oldAddress, newAddress);
    RelocatePointer(BoneMask, oldAddress, newAddress);
    RelocatePointer(RigidBoneMask, oldAddress, newAddress);
    RelocatePointer(RigidBonePositions, oldAddress, newAddress);
    for (unsigned long i = 0; i < SequenceCount; ++i) {
        C3DAnimationSequence& sequence = Sequences[i];
        if (!sequence.Shared)
            RelocatePointer(sequence.Data, oldAddress, newAddress);
    }
    for (unsigned long j = 0; j < SequenceDataBufferSize; ++j)
        SequenceDataBuffer[j].RelocateDataInline(oldAddress, newAddress);
    for (CInstanceNode* node = InstanceList->_M_next; node != InstanceList; node = node->_M_next) {
        C3DAnimation2* instance = node->_M_data.Get();
        if (instance) {
            for (unsigned long k = 0; k < instance->SequenceCount; ++k) {
                C3DAnimationSequence& sequence = instance->Sequences[k];
                if (sequence.Shared)
                    RelocatePointer(sequence.Data, oldAddress, newAddress);
            }
        }
    }
}