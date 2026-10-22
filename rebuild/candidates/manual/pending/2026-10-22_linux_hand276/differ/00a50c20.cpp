// C3DAnimation2::InitialiseAnimationInstance(C3DAnimation2&, const C3DMesh2&, bool, float*) -- make this animation an instance of `source`: copy its stats, register in its instance list, build the sequences in the scratch buffer, then move them into the movable memory pool (or the private allocator) and report the instance's sequence ratio
// Grade: DIFFER(611v611) -- scheduling only: in the allocation-failure exit retail emits `xor al,al ; mov [ebp+74h],al` before `pop edi ; pop esi`, ours interleaves the pops; everything else is relocation-equal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A50C20, length 611 bytes (`ret 10h`).
// Callees: multimap operator= 0x00A54A90 (manifest "LTextTreeWalkThrough_CopyConstructor" propagated), CBaseIntelligentPointer
//   ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<C3DAnimation2> vtable
//   0x0129D50C), malloc 0x00BFEA0E (list node), scratch buffer @ 0x013CB498 Lock 0x009F4980 / Unlock 0x009F49A0 (manifest
//   "CNavSwitchableLeafNode::GetRegion" / "CXMVPlaybackInfo::CXMVPlaybackInfo" propagated; size at +0x10),
//   CSimpleMemoryPool SetMemoryPool 0x00A5C7E0 / CalculateMemoryAllocatedByClient 0x00A5C850 / Reset 0x00A5C790 /
//   SetCapacity 0x00A5C5C0 / Allocate 0x00A5C910, InitialiseAnimationSequences 0x00A50510, CMovableResourceMemoryPool::
//   AllocateResource 0x00A62E40, CHandleBasedMemoryPool::GetPointerFromHandle 0x00A026B0, RelocateData (CMovableResource slot 1).
// Shape: the instance pointer is a named local in its own block and the rest of the body sits in a second block, so the
//   sequence count shares the pointer's frame word (retail's 8-byte frame); the STLport list insert reads pos->_M_prev
//   once before the links; the ratio is `count / max(SequenceCount, 1)` through a const-ref max.
// Layout: retail C3DAnimation2 as the landed ctor 0x00A4EE10 / RelocateData 0x00A4E960 (C3DAnimationStats base 0x2c, packed
//   to 2; CMovableResource +0x2c, Allocator +0x30, MemoryPool +0x38, MemoryHandle +0x3c, MovementTrack +0x40, FrameCount
//   +0x54, SequenceCount +0x58, LockedFromMemMemoryRelocation +0x74, InstanceList +0x78).
// Leaf: InitialiseAnimationInstance
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0129D50C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

struct CSimpleMemoryPool {
    void SetMemoryPool(void* buffer, unsigned long size);   // 0x00A5C7E0
    unsigned long CalculateMemoryAllocatedByClient();       // 0x00A5C850
    void Reset();                                           // 0x00A5C790
    void SetCapacity(unsigned long size);                   // 0x00A5C5C0
    void* Allocate(unsigned long size);                     // 0x00A5C910
};
struct CMovableResource;
struct CMovableResourceMemoryPool {
    bool AllocateResource(long* handle, CMovableResource* resource, unsigned long size, bool flag);   // 0x00A62E40
    void* GetPointerFromHandle(long handle);                // 0x00A026B0
};
struct CScratchBuffer {                                     // global @ 0x013CB498
    unsigned char _pad00[0x10];
    unsigned long Size;                                     // +0x10
    void Lock(void*& buffer);                               // 0x009F4980
    void Unlock();                                          // 0x009F49A0
};
extern CScratchBuffer g_AnimationScratchBuffer;

struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template <class T> struct CCountedPointer {
    T* Data;
    SCountBlock* Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* c = other.Info; T* p = other.Data;
        if (Info != c) {
            if (Info) { --Info->references; if (!Info->references) { Info->destroy(Info->object); delete Info; } }
            Data = p; Info = c; if (c) ++c->references;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};

struct C3DVector { float X, Y, Z; };
struct CTimingEventMultimap {
    void* _M_header; unsigned long _M_node_count; long _pad;
    CTimingEventMultimap& operator=(const CTimingEventMultimap& other);   // 0x00A54A90
};
#pragma pack(push, 2)
struct C3DAnimationStats {
    virtual void v00();
    long _04;
    long AnimType;                                      // +0x08
    float Duration;                                     // +0x0c
    bool LoopFlag;                                      // +0x10
    bool MovementFlag;                                  // +0x11
    C3DVector MovementVector;                           // +0x12
    unsigned short _pad1e;                              // +0x1e
    CTimingEventMultimap TimingEvents;                  // +0x20
};
#pragma pack(pop)
struct CMovableResource {
    virtual void v00();
    virtual void RelocateData(long handle, void* oldAddress, void* newAddress, unsigned long size);   // slot 1
};
struct CIntelligentPointerNode {
    CIntelligentPointerNode* _M_next;
    CIntelligentPointerNode* _M_prev;
    CIntelligentPointer<struct C3DAnimation2> _M_data;
};
struct CInstanceList {
    CIntelligentPointerNode* _M_node;
    void push_back(const CIntelligentPointer<struct C3DAnimation2>& x)
    {
        CIntelligentPointerNode* pos = _M_node;
        CIntelligentPointerNode* tmp = (CIntelligentPointerNode*)malloc(sizeof(CIntelligentPointerNode));
        new (&tmp->_M_data) CIntelligentPointer<struct C3DAnimation2>(x);
        CIntelligentPointerNode* prev = pos->_M_prev;
        tmp->_M_next = pos;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        pos->_M_prev = tmp;
    }
};
struct C3DAnimationSequence;
struct C3DAnimationSequenceData;
class C3DMesh2;
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

struct C3DAnimation2 : C3DAnimationStats, CMovableResource {
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
    bool InitialiseAnimationSequences(C3DAnimation2& source, const C3DMesh2& mesh, bool flag, unsigned long& count);   // 0x00A50510
    bool InitialiseAnimationInstance(C3DAnimation2& source, const C3DMesh2& mesh, bool flag, float* ratio);
};

bool C3DAnimation2::InitialiseAnimationInstance(C3DAnimation2& source, const C3DMesh2& mesh, bool flag, float* ratio)
{
    LockedFromMemMemoryRelocation = true;
    FrameCount = source.FrameCount;
    SamplesPerSecond = source.SamplesPerSecond;
    LoopFlag = source.LoopFlag;
    Duration = source.Duration;
    MovementFlag = source.MovementFlag;
    MovementVector = source.MovementVector;
    TimingEvents = source.TimingEvents;
    AnimType = source.AnimType;
    {
        CIntelligentPointer<C3DAnimation2> self(this);
        source.InstanceList.push_back(self);
    }
    {
        void* buffer = 0;
        g_AnimationScratchBuffer.Lock(buffer);
        Allocator->SetMemoryPool(buffer, g_AnimationScratchBuffer.Size);
        unsigned long count = 0;
        InitialiseAnimationSequences(source, mesh, flag, count);
        MovementTrack = source.MovementTrack;
        unsigned long size = Allocator->CalculateMemoryAllocatedByClient();
        if (MemoryPool) {
            if (!MemoryPool->AllocateResource(&MemoryHandle, this, size, false)) {
                g_AnimationScratchBuffer.Unlock();
                LockedFromMemMemoryRelocation = false;
                return false;
            }
            void* data = MemoryPool->GetPointerFromHandle(MemoryHandle);
            memcpy(data, buffer, size);
            RelocateData(MemoryHandle, buffer, data, size);
            Allocator->SetMemoryPool(data, size);
        } else {
            Allocator->Reset();
            Allocator->SetCapacity(size);
            void* data = Allocator->Allocate(size);
            memcpy(data, buffer, size);
            RelocateData(0, buffer, data, size);
        }
        g_AnimationScratchBuffer.Unlock();
        if (ratio)
            *ratio = (float)count / (float)Max(SequenceCount, 1UL);
        LockedFromMemMemoryRelocation = false;
        return true;
    }
}
