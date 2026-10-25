// CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn(CMemoryDataInputStream* stream, CAttackHistoryCombo* value) -- binary read of one attack-history combo: the entry count, every (attack history type, damage attribute) pair pushed onto AttackCombo, then the Multiplier
// Grade: DIFFER(437v447) -- retail keeps `value` in ebp and the two element reads register-cached (edi/ebx, pre-loop loads from their arg-slot homes, reload after the overflow call); ours keeps value in ebx and re-reads the elements from their slots
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCAttackHistoryCombo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DC6A0, 447 bytes, `ret 8` (traits `this` unused).
// Identity: Ego_r.pdb name set ?TransferBinaryIn@?$CPersistTraits@VCAttackHistoryCombo@@@@QAEXAAVCMemoryDataInputStream@@
//   AAVCAttackHistoryCombo@@@Z. The manifest's "CPersistTraits<CHeroStatIncreaseData>" is propagated (that reader is the
//   landed map reader 0x006DC070): the only caller is the vector loop 0x006DD580 (stride 0x10, call at 0x006DD5F2), which
//   0x006D91B9 runs on the def's last vector member right after CHeroExperienceDef::ReadFromStream 0x006DD1C0 read
//   StatIncreasePerHitType -- the PDB's next member is vector<CAttackHistoryCombo> AttackHistoryCombos.
// CAttackHistoryCombo = { vector<pair<EAttackHistoryType,EDamageAttribute> > AttackCombo +0, float Multiplier +0xC }
// (retail STLport vectors are 12 bytes, so the PDB's +0x10 Multiplier is +0xC here).
// Callees: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out,
// 0x006DB8C0 vector<pair<EAttackHistoryType,EDamageAttribute> >::_M_insert_overflow(pos, x, __true_type, 1, true).
// Shape: CMemoryDataInputStream::Read inlined (speed TU, `position += size`), the scalars through inline
// CPersistTraits<T>::TransferBinaryIn(stream, &x); a `!= 0` down-counter loop (the counter spilled to the frame), the
// two element locals in the dead argument slots, the pair temporary built from them, then STLport's inline push_back
// (null-checked placement copy, `add [ebp+4],8`, else the out-of-line overflow insert with the __true_type homed in the
// stream slot). No frame pointer.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
#include "engine/CMemoryDataInputStream.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader is CDataInputStream's.
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
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

enum EAttackHistoryType { ATTACK_HISTORY_TYPE_NONE = 0, ATTACK_HISTORY_TYPE_FORCE_DWORD = 0x7fffffff };
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0, DAMAGE_ATTRIBUTE_FORCE_DWORD = 0x7fffffff };

struct __true_type { __true_type() {} };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};

template <class T> struct _STLP_alloc_proxy { T _M_data; };

template <class T> class vector {                      // STLport vector: start / finish / end of storage
public:
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x006DB8C0
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*> _M_end_of_storage;
};

// _M_insert_overflow @ 0x006DB8C0: a same-TU stand-in with the STLport reallocation shape (the landed C3DFrustrum::AddPoint
// 0x0084E390 rule: the callee that receives the empty __true_type gets a body, never inlined, the tag unnamed) -- VC7.1 then
// sees that the pair temporary does not escape and copies the element straight from the registers that read it.
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    unsigned int old_size = _M_finish - _M_start;
    unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != pos; ++p, ++new_finish)
        new (new_finish) T(*p);
    for (unsigned int i = 0; i < fill_len; ++i, ++new_finish)
        new (new_finish) T(x);
    if (!atend)
        for (T* p = pos; p != _M_finish; ++p, ++new_finish)
            new (new_finish) T(*p);
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage._M_data = new_start + len;
}
#pragma auto_inline(on)
}

class CAttackHistoryCombo {                            // 0x10 bytes in retail
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x0
    float Multiplier;                                  // +0xC
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<unsigned long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, unsigned long* value)
    {
        unsigned long number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<float> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, float* value)
    {
        float number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<EAttackHistoryType> {
    static __forceinline EAttackHistoryType Read(CMemoryDataInputStream* stream)
    {
        EAttackHistoryType number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        return number;
    }
};
template <> struct CPersistTraits<EDamageAttribute> {
    static __forceinline EDamageAttribute Read(CMemoryDataInputStream* stream)
    {
        EDamageAttribute number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        return number;
    }
};
template <> struct CPersistTraits<CAttackHistoryCombo> {
    void TransferBinaryIn(CMemoryDataInputStream* stream, CAttackHistoryCombo* value);
};

void CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn(CMemoryDataInputStream* stream, CAttackHistoryCombo* value)
{
    unsigned long count;
    CPersistTraits<unsigned long> countTraits;
    countTraits.TransferBinaryIn(stream, &count);
    for (; count != 0; --count) {
        EAttackHistoryType type = CPersistTraits<EAttackHistoryType>::Read(stream);
        EDamageAttribute attribute = CPersistTraits<EDamageAttribute>::Read(stream);
        value->AttackCombo.push_back(std::pair<EAttackHistoryType, EDamageAttribute>(type, attribute));
    }
    CPersistTraits<float> floatTraits;
    floatTraits.TransferBinaryIn(stream, &value->Multiplier);
}
