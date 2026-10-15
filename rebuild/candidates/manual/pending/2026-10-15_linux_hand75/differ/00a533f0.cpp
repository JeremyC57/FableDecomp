// Grade: DIFFER(82v80) -- the lane keeps a dead `jb` after `test edx,edx ; ja` in the unsigned 64-bit size guard (retail drops it: test clears CF); `<=`, inverted `>`, `< 2^32`, a named 64-bit local, optimize("s"), /Oa and /GS tried; the rest of the body matches
// CSimpleMemoryPool::New<SSequenceKey>(unsigned long count) -- allocates count 8-byte keys from the pool (nothing for a zero or overflowing count) and default-constructs each one (value 0, index -1, flag clear)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A533F0, length 80 bytes.
// Leaf: ??$New@
// Callees: CSimpleMemoryPool::Allocate(unsigned long) @ 0x00A5C910 (`__thiscall`: `this` passes through ecx untouched).
// The template argument of the manifest label (C3DAnimationSequence) is propagated; the element here is an 8-byte
// record {long 0, short -1, byte 0}, named descriptively. Layout of the pool: engine/CSimpleMemoryPool.h.
// Shape: the 64-bit size guard (`mul` by 8, `test edx,edx ; ja`, `cmp eax,-1 ; ja`) with the allocation as the taken
// arm and a null result as the fall-through, then a counted placement-new loop (null-checked) on a separate cursor.
#include <new>
#include "engine/CSimpleMemoryPool.h"

struct SSequenceKey {                           // 8 bytes
    long          m_value;                      // +0
    short         m_index;                      // +4
    unsigned char m_flag;                       // +6
    SSequenceKey() : m_value(0), m_index(-1), m_flag(0) { }
};

struct CSimpleMemoryPool_Methods : CSimpleMemoryPool {
    void* Allocate(unsigned long size);         // 0x00A5C910
    template <class T> T* New(unsigned long count)
    {
        T* data;
        if (count != 0 && (unsigned __int64)count * sizeof(T) <= 0xFFFFFFFFu)
            data = static_cast<T*>(Allocate(count * sizeof(T)));
        else
            data = 0;
        for (unsigned long i = 0; i < count; ++i)
            new (data + i) T;
        return data;
    }
};

template SSequenceKey* CSimpleMemoryPool_Methods::New<SSequenceKey>(unsigned long count);
