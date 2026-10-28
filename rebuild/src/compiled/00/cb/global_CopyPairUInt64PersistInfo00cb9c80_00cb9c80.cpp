// _global::Copy_PairUInt64PersistInfo_00cb9c80(PairUInt64PersistInfo*, PairUInt64PersistInfo*, PairUInt64PersistInfo*, SRandomAccessIteratorTag, long*) -- SGI __copy over pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>, returns the advanced result
// Leaf: Copy_PairUInt64PersistInfo_00cb9c80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB9C80, length 93 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0.
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed count (last - first) / 24
// is computed once and counted down in ebx. This row's manifest label is right: Ego_r.pdb CPersistInfo (16 bytes)
// = { ulong64 UID +0; CCharString PersistScript +8 } (8-byte aligned, 4 bytes of tail padding), so the pair is
// 24 bytes: key +0, UID +8, PersistScript +0x10, padding +0x14. The implicit operator= moves the key and UID
// dwords and calls the out-of-line CCharString::operator=; the padding word is never copied.
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo { public:                        // Ego_r.pdb: 16 bytes
        unsigned __int64 UID;                    // +0x00
        CCharString      PersistScript;          // +0x08
    };
};
}

struct PairUInt64PersistInfo {                   // std::pair<unsigned __int64, CPersistInfo>, 24 bytes
    unsigned __int64                                     first;    // +0x00
    NScript::CRuntimeScriptedEntityPersist::CPersistInfo second;   // +0x08
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairUInt64PersistInfo* __fastcall Copy_PairUInt64PersistInfo_00cb9c80(PairUInt64PersistInfo* first, PairUInt64PersistInfo* last, PairUInt64PersistInfo* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}