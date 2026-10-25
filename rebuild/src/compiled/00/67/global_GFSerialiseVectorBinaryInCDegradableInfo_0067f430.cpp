// GFSerialiseVectorBinaryIn<CDegradableInfo>(CMemoryDataInputStream& stream, vector<CDegradableInfo>& out) -- read the element count, resize the vector to it (new stages default-constructed: a zeroed engine graphic), then read every degradable stage in place through CPersistTraits<CDegradableInfo>
// Leaf: ??$GFSerialiseVectorBinaryIn@VCDegradableInfo@@
// Grade: RELOCATION_MATCH (agent902). Fix: the traits carries Ego_r.pdb's uninitialised-by-use `T DefaultVal` (GOTCHAS
//   1236) -- with the noinline TransferBinaryIn body in the TU VC7.1 deletes the dead DefaultVal stores and packs the
//   28-byte traits over the dead resize(n, T()) temporary, so the traits `this` is retail's [esp+1Ch]. The empty-traits
//   model packed it over the dead count word instead. Catalog label <CBrainUpdateZone> is propagated (element CDegradableInfo).
//
// Retail 0x0067F430, 173 bytes (+ int3 padding to 176), __fastcall (stream in ecx, out in edx), plain `ret`; speed TU
//   (no frame pointer, `sub esp, 20h`: the count word and one 28-byte CDegradableInfo).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path),
//          vector<CDegradableInfo>::resize(size_type, const CDegradableInfo&) @ 0x0067F220,
//          CPersistTraits<CDegradableInfo>::TransferBinaryIn @ 0x0067DF60 (catalog label).
// Identity: the load arm next to GFSerialiseVectorBinaryOut<CDegradableInfo> 0x0067E720 in the persist-context transfer
//   at 0x0067E34D (`sub eax, 2 ; je` picks 0x0067F430, else 0x0067E720).  The catalog's "<CBrainUpdateZone>" is a
//   propagated label (the landed CBrainUpdateZone instance is 0x00466BEB).
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC, chunkLength +0x14); Ego_r.pdb CDegradableInfo
//   (28 bytes) = { float HealthPercentage; CEngineGraphic EngineGraphic (+4, 14 packed bytes); long SmashParticleEmitter
//   (+0x14); bool BlocksNavigation (+0x18); } -- only the engine graphic has a constructor (all five members zeroed).
// Shape: the landed 0x00466BEB / 0x00672530 template: the inline 4-byte Read of the count, STLport's one-argument
//   resize forwarding `resize(n, T())` (the temporary's implicit ctor zeroes EngineGraphic only), then
//   traits.TransferBinaryIn(&stream, &out[i]) with the index strength-reduced to a byte offset and a down-counter.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

#pragma pack(push, 1)
class CEngineGraphic {                                      // 14 bytes (Ego_r.pdb)
public:
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) {}
};
#pragma pack(pop)

class CDegradableInfo {                                     // 28 bytes
public:
    float          HealthPercentage;                        // +0x00
    CEngineGraphic EngineGraphic;                           // +0x04
    long           SmashParticleEmitter;                    // +0x14
    bool           BlocksNavigation;                        // +0x18
};

namespace _STL {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(size_type __new_size, const _Tp& __x);                  // 0x0067F220 for CDegradableInfo
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CDegradableInfo> {
    CDegradableInfo DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, CDegradableInfo* value);  // 0x0067DF60
};
// Same-TU stand-in for 0x0067DF60 (GOTCHAS 1046: the callee that receives the empty traits gets a real body,
// noinline): read the stage's members in place.
__declspec(noinline) void CPersistTraits<CDegradableInfo>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, CDegradableInfo* value)
{
    stream->Read(&value->HealthPercentage, 4);
    stream->Read(&value->EngineGraphic, 14);
    stream->Read(&value->SmashParticleEmitter, 4);
    stream->Read(&value->BlocksNavigation, 1);
}

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, _STL::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(&stream, &out[i]);
}

template void __fastcall GFSerialiseVectorBinaryIn<CDegradableInfo>(CMemoryDataInputStreamReader& stream, _STL::vector<CDegradableInfo>& out);