// Grade: DIFFER(272v280) -- lane gap: retail pushes ebx/esi/edi only after the early-out tests (the failure tail `*handle = -1 ; ret 0Ch` has no pops); the lane pushes ebx in the prologue, so every stack offset and the register pick shift (group in ebx v edx->ebx copy), the query is homed in the `pass` slot rather than the `handle` slot, and the push_back tail differs. The allocation helper (index local, zeroing record constructor, `inc [count]`) matches.
// CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest(CEngineInternalPrimitiveBase& primitive, long pass, long* handle) -- when occlusion testing is on and this is the test pass, allocate an occlusion query for the primitive's group in the current occlusion layer and record it in the group's per-frame query list (cleared on a new frame); the handle is -1 when no query is made
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B905A0, length 278 bytes (`ret 0Ch`).
// Callees: memmove @ 0x00BFEAE6 (the inline erase's tail copy, never taken), vector<long>::_M_insert_overflow @ 0x00464ACE.
// Globals: the occlusion settings @ 0x01436E78 (enabled byte +0xbd, current layer +0x78), the occlusion query pool @ 0x01436E5C
// (frame number +0x4010, query records (8 bytes) +0x4014, used count +0x401c, capacity 0x2000) -- descriptive names.
// Layout (descriptive): the primitive's per-layer occlusion groups at +0x48; a group holds the query vector (STLport, 12 bytes) at
// +0x0c and the frame it was filled in at +0x1c.
// Shape: the pool allocation is an inline helper (`count < 0x2000` stores two zero words and post-increments the count, else -1);
// the handle is written out first and then homed in the dead `handle` argument slot for push_back's const reference; a new frame
// clears the vector through the inline STLport erase(begin(), end()); push_back with the __true_type tag.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int size);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };

struct CLongVector {                                    // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    void _M_insert_overflow(long* position, const long& x, const __true_type&, unsigned int fill, bool atEnd);   // 0x00464ACE
    static __forceinline long* copy(long* first, long* last, long* result)
    {
        if (first != last) {
            unsigned int bytes = (char*)last - (char*)first;
            memmove(result, first, bytes);
            return (long*)((char*)result + bytes);
        }
        return result;
    }
    __forceinline long* erase(long* first, long* last)
    {
        long* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    __forceinline void clear() { erase(begin(), end()); }
    __forceinline void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) long(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct COcclusionGroup {
    unsigned char _pad00[0x0c];
    CLongVector   Queries;                              // +0x0c
    unsigned char _pad18[4];
    unsigned long FrameFilled;                          // +0x1c
};

struct COcclusionQuery {                               // 8-byte query record
    long Query;
    long Result;
    COcclusionQuery() : Query(0), Result(0) { }
};
struct COcclusionQueryPool {
    unsigned char    _pad[0x4010];
    unsigned long    FrameNumber;                       // +0x4010
    COcclusionQuery* Queries;                           // +0x4014
    unsigned char    _pad4018[4];
    unsigned long    Count;                             // +0x401c
    __forceinline long Allocate()
    {
        if (Count < 0x2000) {
            unsigned long index = Count;
            Queries[index] = COcclusionQuery();
            ++Count;
            return index;
        }
        return -1;
    }
};
extern COcclusionQueryPool* g_pOcclusionQueryPool;      // 0x01436E5C

struct COcclusionSettings {
    unsigned char _pad00[0x78];
    long          CurrentLayer;                         // +0x78
    unsigned char _pad7c[0x41];
    bool          Enabled;                              // +0xbd
};
extern COcclusionSettings* g_pOcclusionSettings;        // 0x01436E78

struct CEngineInternalPrimitiveBase {
    unsigned char    _pad00[0x48];
    COcclusionGroup* OcclusionGroups[1];                // +0x48 (per layer)
};

struct CEnginePrimitiveManagerOcclusionBase {
    void PrepareOcclusionTest(CEngineInternalPrimitiveBase& primitive, long pass, long* handle);
};

void CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest(CEngineInternalPrimitiveBase& primitive, long pass, long* handle)
{
    if (g_pOcclusionSettings->Enabled && pass == 2) {
        long layer = g_pOcclusionSettings->CurrentLayer;
        if (layer != -1) {
            COcclusionGroup* group = primitive.OcclusionGroups[layer];
            if (group) {
                long query = g_pOcclusionQueryPool->Allocate();
                *handle = query;
                if (query != -1) {
                    if (g_pOcclusionQueryPool->FrameNumber != group->FrameFilled)
                        group->Queries.clear();
                    group->Queries.push_back(query);
                    group->FrameFilled = g_pOcclusionQueryPool->FrameNumber;
                }
                return;
            }
        }
    }
    *handle = -1;
}
