// CPersistTraits<CComboMultiplierData>::TransferBinaryIn(CMemoryDataInputStream* stream, CComboMultiplierData* value) -- binary read of the combo multiplier data: the two experience floats, then the entry count and every (damage attribute, multiplier) pair inserted into the ComboMultiplier map
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCComboMultiplierData@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DBE00, 480 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCComboMultiplierData@@@@QAEXAAVCMemoryDataInputStream@@AAVCComboMultiplierData@@@Z;
// CComboMultiplierData = { map<EDamageAttribute,float> ComboMultiplier +0, float
// ExperienceMultiplierNumSecondsForAttenuation, float ExperienceMultiplierParamDecayDeltaPerSecond } (retail STLport map is
// 12 bytes, so the floats sit at +0xC / +0x10 instead of the PDB's +0x10 / +0x14).
// Callees: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out,
// 0x004619BA map<EDamageAttribute,float>::insert(const value_type&) (pair<iterator,bool> through a hidden pointer;
// landed as CRetailTree_longkey_ins00461a40::insert, also called by CHitParameters::AddDamageWithAttribute 0x00868CA0).
// Caller: 0x006DD0AA (one site).
// Shape: CMemoryDataInputStream::Read inlined (speed TU, `position += size`), the scalars through inline
// CPersistTraits<T>::TransferBinaryIn(stream, &x); the count is a `!= 0` down-counter (`test edi,edi ; je` then `dec edi ; jne`;
// `i < count` gives `jbe`), the per-entry key and multiplier locals take the two dead argument slots, and the pair temporary is
// built from them right before each insert. No frame pointer.
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

enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0, DAMAGE_ATTRIBUTE_FORCE_DWORD = 0x7fffffff };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T> struct less {};
struct _Rb_tree_node_base;
struct _Rb_tree_iterator { _Rb_tree_node_base* _M_node; };
template <class K, class V, class C = less<K> > class map {
public:
    typedef pair<const K, V> value_type;
    _Rb_tree_node_base* _M_header;                     // +0x00
    unsigned long       _M_node_count;                 // +0x04
    C                   _M_key_compare;                // +0x08 (padded)
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x004619BA for <EDamageAttribute,float>
};
}

class CComboMultiplierData {
public:
    std::map<EDamageAttribute, float> ComboMultiplier; // +0x00
    float ExperienceMultiplierNumSecondsForAttenuation;  // +0x0C
    float ExperienceMultiplierParamDecayDeltaPerSecond;  // +0x10
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
template <> struct CPersistTraits<EDamageAttribute> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, EDamageAttribute* value)
    {
        EDamageAttribute number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<CComboMultiplierData> {
    void TransferBinaryIn(CMemoryDataInputStream* stream, CComboMultiplierData* value);
};

void CPersistTraits<CComboMultiplierData>::TransferBinaryIn(CMemoryDataInputStream* stream, CComboMultiplierData* value)
{
    CPersistTraits<float> floatTraits;
    floatTraits.TransferBinaryIn(stream, &value->ExperienceMultiplierNumSecondsForAttenuation);
    floatTraits.TransferBinaryIn(stream, &value->ExperienceMultiplierParamDecayDeltaPerSecond);
    unsigned long count;
    CPersistTraits<unsigned long> countTraits;
    countTraits.TransferBinaryIn(stream, &count);
    for (; count != 0; --count) {
        EDamageAttribute attribute;
        CPersistTraits<EDamageAttribute> keyTraits;
        keyTraits.TransferBinaryIn(stream, &attribute);
        float multiplier;
        floatTraits.TransferBinaryIn(stream, &multiplier);
        value->ComboMultiplier.insert(std::pair<const EDamageAttribute, float>(attribute, multiplier));
    }
}