// CSimpleMemoryPool::New<C3DAnimationSequenceData>(unsigned long count) -- allocate count 0x2c-byte animation-sequence records from the pool (null for zero or a 32-bit size overflow) and default-construct each in place
// Grade: DIFFER(125v116) -- the 64-bit size check: retail `cmp edx,0 ; ja fail ; cmp eax,-1 ; ja fail` has no `jb` between the halves; our `(unsigned __int64)count*sizeof(T) <= 0xffffffff` adds `jb ok` (+2) and the loop gets a 7-byte `lea esp,[esp]` alignment pad
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A53440, length 116 bytes (`ret 4`). Callees: CSimpleMemoryPool::Allocate @ 0x00A5C910 (this chunk).
// Shape: placement-new loop with the per-element null test; the record ctor's stores are body assignments in retail store order,
// the IdentityFlag bit-field clear last (hoisted `and bl,7Fh`, byte store at the end). Tried `< 0x100000000ui64` (cmp edx,1) and split hi/lo tests.
// Leaf: New@
#include <new>
#include "engine/CSimpleMemoryPool.h"

struct CQuaternion;
struct C3DVectorWord;

struct C3DAnimationSequenceData {                   // 0x2c bytes (Ego_r.pdb)
    char*           BoneName;                       // +0x00
    float           SamplesPerSecond;               // +0x04
    unsigned short  FrameCount;                     // +0x08
    unsigned char   BoneType : 5;                   // +0x0a
    unsigned char   EnabledFlag : 1;
    unsigned char   AnimatedFlag : 1;
    unsigned char   IdentityFlag : 1;
    unsigned char   RotationTrackMode : 2;          // +0x0b
    unsigned char   PositionTrackMode : 2;
    unsigned char   ScalingTrackMode : 2;
    float           PositionFactor;                 // +0x0c
    float           ScalingFactor;                  // +0x10
    CQuaternion*    RotationTrack;                  // +0x14
    unsigned short  RotationTrackSize;              // +0x18
    unsigned short  PositionTrackSize;              // +0x1a
    unsigned short  PalettedRotationTrackSize;      // +0x1c
    unsigned short  PalettedPositionTrackSize;      // +0x1e
    C3DVectorWord*  PositionTrack;                  // +0x20
    unsigned char*  PalettedRotationTrack;          // +0x24
    unsigned char*  PalettedPositionTrack;          // +0x28
    C3DAnimationSequenceData()
    {
        BoneName = 0;
        RotationTrack = 0;
        RotationTrackSize = 0;
        PositionTrackSize = 0;
        PalettedRotationTrackSize = 0;
        PalettedPositionTrackSize = 0;
        PositionTrack = 0;
        PalettedRotationTrack = 0;
        PalettedPositionTrack = 0;
        FrameCount = 0;
        IdentityFlag = 0;
    }
};

struct CSimpleMemoryPoolNew : CSimpleMemoryPool {
    void* Allocate(unsigned long size);              // 0x00A5C910
    template <class T> T* New(unsigned long count)
    {
        T* p = (count != 0 && (unsigned __int64)count * sizeof(T) <= 0xffffffffu)
            ? static_cast<T*>(Allocate(count * sizeof(T))) : 0;
        T* q = p;
        for (unsigned long n = count; n > 0; --n, ++q)
            new (q) T();
        return p;
    }
};

template C3DAnimationSequenceData* CSimpleMemoryPoolNew::New<C3DAnimationSequenceData>(unsigned long);
