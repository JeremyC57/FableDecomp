// _global::CopyBackward_PairCharStringRegisteredPSCInfo_00af2d60(PairCharStringRegisteredPSCInfo*, PairCharStringRegisteredPSCInfo*, PairCharStringRegisteredPSCInfo*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, returns the moved-back result
// Leaf: CopyBackward_PairCharStringRegisteredPSCInfo_00af2d60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2D60, length 95 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0 (twice per element).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n)
// *--result = *--last;` with the signed count computed once; the moved-back result is returned.
// The manifest label is right: Ego_r.pdb CRegisteredPSCInfo (12 bytes) = { CPSCBase* (*AllocFunc)(); EPSCType
// ComponentType; CCharString GUIName }, so the 16-byte pair is key +0, AllocFunc +4, ComponentType +8, GUIName +0xC.
// The implicit operator= assigns the key string, moves the two plain words and assigns GUIName (addressed through
// the strength-reduced src+0xC/dst+0xC pointers); the count is decremented in the dead `result` argument slot.
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NParticleEngine {
class CPSCBase;
enum EPSCType { PSC_TYPE_NONE = 0 };
class CRegisteredPSCInfo { public:                      // Ego_r.pdb: 12 bytes
    CPSCBase* (__cdecl* AllocFunc)();            // +0x00
    EPSCType    ComponentType;                   // +0x04
    CCharString GUIName;                         // +0x08
};
}

struct PairCharStringRegisteredPSCInfo {         // std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, 16 bytes
    CCharString                         first;   // +0x00
    NParticleEngine::CRegisteredPSCInfo second;  // +0x04
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairCharStringRegisteredPSCInfo* __fastcall CopyBackward_PairCharStringRegisteredPSCInfo_00af2d60(PairCharStringRegisteredPSCInfo* first, PairCharStringRegisteredPSCInfo* last, PairCharStringRegisteredPSCInfo* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}