// _global::CopyBackward_PairUInt64PersistInfo_00cb94d0(PairUInt64PersistInfo*, PairUInt64PersistInfo*, PairUInt64PersistInfo*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>, returns the moved-back result
// Leaf: CopyBackward_PairUInt64PersistInfo_00cb94d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB94D0, length 94 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n)
// *--result = *--last;` with the signed count computed once; the moved-back result is returned.
// The manifest label is right: Ego_r.pdb CPersistInfo (16 bytes) = { ulong64 UID +0; CCharString PersistScript
// +8 } (8-byte aligned), so the pair is 24 bytes: key +0, UID +8, PersistScript +0x10, tail padding +0x14 (never
// copied). The implicit operator= moves the four key/UID dwords and calls CCharString::operator=; the count is held
// in ebx. Backward twin of 0x00CB9C80.
class CCharString { public:
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

PairUInt64PersistInfo* __fastcall CopyBackward_PairUInt64PersistInfo_00cb94d0(PairUInt64PersistInfo* first, PairUInt64PersistInfo* last, PairUInt64PersistInfo* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}