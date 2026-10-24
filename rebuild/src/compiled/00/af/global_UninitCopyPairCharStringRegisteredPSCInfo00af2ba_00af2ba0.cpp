// _global::UninitCopy_PairCharStringRegisteredPSCInfo_00af2ba0(const PairCharStringRegisteredPSCInfo* first, const PairCharStringRegisteredPSCInfo* last, PairCharStringRegisteredPSCInfo* result, const __false_type&) -- STLport __uninitialized_copy over pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, returns the advanced result
// Leaf: UninitCopy_PairCharStringRegisteredPSCInfo_00af2ba0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2BA0, length 94 bytes, `ret 8`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice per element).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx,
// last in edx -- spilled to the `push ecx` frame word and reloaded after the calls -- result + the tag reference on
// the stack). Each element is placement-new copy-constructed (null test on the destination) through the pair's
// implicit copy constructor: the key string (out of line), then the nested CRegisteredPSCInfo's implicit copy
// constructor -- AllocFunc +4 and ComponentType +8 as plain words, GUIName +0xC out of line -- addressed through the
// strength-reduced src+0xC/dst+0xC pointer pair (a flat 16-byte record schedules differently: the nesting matters).
// Element = Ego_r.pdb CRegisteredPSCInfo (12 bytes: AllocFunc, ComponentType, GUIName) behind a CCharString key,
// the same pair the TU neighbour 0x00AF2D60 copies backward. Label correction: the catalog's
// `_Move<pair<pair<long,long>, CEngineTextureDecal>*>` is propagated (that pair is 32 bytes; this is a `ret 8`
// construct-copy).
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);       // 0x0099EC30
};

namespace NParticleEngine {
class CPSCBase;
enum EPSCType { PSC_TYPE_NONE = 0 };
struct CRegisteredPSCInfo {                      // Ego_r.pdb: 12 bytes
    CPSCBase* (__cdecl* AllocFunc)();            // +0x00
    EPSCType    ComponentType;                   // +0x04
    CCharString GUIName;                         // +0x08
};
}

struct PairCharStringRegisteredPSCInfo {         // std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, 16 bytes
    CCharString                         first;   // +0x00
    NParticleEngine::CRegisteredPSCInfo second;  // +0x04
};

struct __false_type { __false_type() { } };

PairCharStringRegisteredPSCInfo* __fastcall UninitCopy_PairCharStringRegisteredPSCInfo_00af2ba0(const PairCharStringRegisteredPSCInfo* first, const PairCharStringRegisteredPSCInfo* last, PairCharStringRegisteredPSCInfo* result, const __false_type&)
{
    PairCharStringRegisteredPSCInfo* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) PairCharStringRegisteredPSCInfo(*first);
    return cur;
}