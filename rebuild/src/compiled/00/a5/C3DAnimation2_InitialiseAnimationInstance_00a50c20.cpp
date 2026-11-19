// C3DAnimation2::InitialiseAnimationInstance(C3DAnimation2& shared_anim, const C3DMesh2& mesh, bool fit_to_mesh, float* out_match) -- make this animation an instance of the shared one: copy its frame count, rate and stats, register in its instance list, build the sequences in the scratch buffer, then move them into a movable-resource block (or the allocator's own memory); out_match gets the matched-sequence ratio; false when the resource allocation fails
// Leaf: ?InitialiseAnimationInstance@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A50C20, 611 bytes, thiscall `ret 10h`; lib_3d_animation_2.obj (Ego_r index 57).
// Callees: _Rb_tree<CCharString,pair<const CCharString,float>,...>::operator= 0x00A54A90 (TimingEvents), CBaseIntelligentPointer ctor
//   0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<C3DAnimation2> vtable 0x0129D50C),
//   malloc 0x00BFEA0E (list node), CScratchBuffer::Lock 0x009F4980 / Unlock 0x009F49A0 on g_ScratchBuffer 0x013CB498 (GetByteSize
//   inline: NoBytes +0x10), CSimpleMemoryPool SetMemoryPool 0x00A5C7E0 / CalculateMemoryAllocatedByClient 0x00A5C850 / Reset
//   0x00A5C790 / SetCapacity 0x00A5C5C0 / Allocate 0x00A5C910, InitialiseAnimationSequences 0x00A50510,
//   CMovableResourceMemoryPool::AllocateResource 0x00A62E40, CHandleBasedMemoryPool::GetPointerFromHandle 0x00A026B0,
//   RelocateData (virtual, CMovableResource slot 1), CCountedPointer<C3DAnimationSequence>::operator= (inline).
// Shape: Ego_d.pdb's record (parameters shared_anim / mesh / fit_to_mesh / out_match; locals temp_buffer, buffer_size, matches,
//   temp_size, the CScopedLockFlagBool `lock` on LockedFromMemMemoryRelocation, and dest_buffer in each arm) in FableWin.pdb's /Od
//   statement order (0x032257D0): the base stats copied member by member (LoopFlag, Duration, MovementFlag, MovementVector,
//   TimingEvents, AnimType -- no operator= call, retail 0x00A50C3C..0x00A50C74), the instance registered through a temporary
//   CIntelligentPointer (retail's out-of-line base ctor + SetPItem, and the node built with the user copy constructor,
//   FABLE_RETAIL_CINTELLIGENTPOINTER_COPY), the lock's destructor clearing the flag on both exits (0x00A50D86 / 0x00A50E75).
// Inferred: the scratch-buffer part sits in its own block after the instance registration: retail's frame is 8 bytes, `matches`
//   sharing the CIntelligentPointer temporary's frame word (0x00A50D0C lea edx,[esp+10h] = 0x00A50C77's slot); with the locals at
//   function scope (Ego_d's newer revision) the frame is 0xC (611v611, every other byte equal).
// Headers: hand4b overlay opt-ins C3DANIMATIONSTATS_TIMINGEVENTS (typed, protected stats members), C3DANIMATION2_INSTANCELIST (typed
//   instance list), CSIMPLEMEMORYPOOL_RESET (Reset / CalculateMemoryAllocatedByClient declarations); hdr/README.md.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DANIMATIONSTATS_MOVEMENTVECTOR
#define FABLE_RETAIL_C3DANIMATIONSTATS_TIMINGEVENTS
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_C3DANIMATION2_INSTANCELIST
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#define FABLE_RETAIL_C3DANIMATION2_INITSEQUENCES
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_RESET
#include "fable_compiler.h"
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DAnimationStats.h"
#include "engine/C3DAnimation2.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/CScopedLockFlagBool.h"
class CScratchBuffer {
public:
    virtual ~CScratchBuffer();
    bool Lock(void** buffer);                                // 0x009F4980
    void Unlock();                                           // 0x009F49A0
    long GetByteSize() const { return NoBytes; }
private:
    long           _pad04;
    unsigned char* Buffer;                                   // +0x08
    bool           Locked;                                   // +0x0c
    long           NoBytes;                                  // +0x10
};
extern CScratchBuffer g_ScratchBuffer;                       // 0x013CB498
bool C3DAnimation2::InitialiseAnimationInstance(C3DAnimation2& shared_anim, const C3DMesh2& mesh, bool fit_to_mesh, float* out_match)
{
    CScopedLockFlagBool lock(LockedFromMemMemoryRelocation);
    FrameCount = shared_anim.FrameCount;
    SamplesPerSecond = shared_anim.SamplesPerSecond;
    LoopFlag = shared_anim.LoopFlag;
    Duration = shared_anim.Duration;
    MovementFlag = shared_anim.MovementFlag;
    MovementVector = shared_anim.MovementVector;
    TimingEvents = shared_anim.TimingEvents;
    AnimType = shared_anim.AnimType;
    shared_anim.InstanceList.push_back(CIntelligentPointer<C3DAnimation2>(this));
    {
        void* temp_buffer = 0;
        g_ScratchBuffer.Lock(&temp_buffer);
        unsigned long buffer_size = g_ScratchBuffer.GetByteSize();
        Allocator->SetMemoryPool(temp_buffer, buffer_size);
        unsigned long matches = 0;
        InitialiseAnimationSequences(shared_anim, mesh, fit_to_mesh, matches);
        MovementTrack = shared_anim.MovementTrack;
        unsigned long temp_size = Allocator->CalculateMemoryAllocatedByClient();
        if (MemoryPool) {
            if (!MemoryPool->AllocateResource(&MemoryHandle, this, temp_size, 0)) {
                g_ScratchBuffer.Unlock();
                return false;
            }
            void* dest_buffer = MemoryPool->GetPointerFromHandle(MemoryHandle);
            memcpy(dest_buffer, temp_buffer, temp_size);
            RelocateData(MemoryHandle, temp_buffer, dest_buffer, temp_size);
            Allocator->SetMemoryPool(dest_buffer, temp_size);
        } else {
            Allocator->Reset();
            Allocator->SetCapacity(temp_size);
            void* dest_buffer = Allocator->Allocate(temp_size);
            memcpy(dest_buffer, temp_buffer, temp_size);
            RelocateData(0, temp_buffer, dest_buffer, temp_size);
        }
        g_ScratchBuffer.Unlock();
        if (out_match)
            *out_match = (float)matches / (float)std::max(SequenceCount, 1UL);
        return true;
    }
}