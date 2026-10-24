// _STL::__uninitialized_copy(pair<EPSCType, CComponentInstance>* first, pair<EPSCType, CComponentInstance>* last, pair<EPSCType, CComponentInstance>* result, const __false_type&) -- copy-construct the particle-system component map entries of [first, last) into raw storage at result and return the end of the copies
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF6920, 94 bytes, __fastcall (first ecx, last edx; result and the empty tag on the
//   stack, `ret 8`).
// Callers: the vector<pair<EPSCType, CComponentInstance> > grow/insert paths at 0x00AF74BA, 0x00AF7528, 0x00AF7611 and
//   0x00AF7668 (the Components map of NParticleEngine::CParticleSystem; the fill twin is landed 0x00AF6980).
// Element: _STL::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> (0x18 bytes): first +0;
//   CComponentInstance (vtable 0x0129F478) +4 = {vptr, CCountedPointer<CPSCBase> Component +4 (Data, Block -> RefCount +0),
//   bool Enabled +0xc, unsigned long InstanceName +0x10}.
// Label: catalog "CopyElements_6DwordStruct" is descriptive.
// Shape: STLport 4.6 __uninitialized_copy verbatim, the placement new keeping its null test; the element copy is the
//   implicit pair / CComponentInstance copy constructors (one vtable store, hoisted into edx) over the counted pointer's
//   user copy constructor (`if (Block) ++Block->RefCount`).
inline void* operator new(unsigned int, void* p) { return p; }

struct CCountBlock {
    unsigned long RefCount;                                 // +0x0
};

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    CCountBlock* Block;                                     // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Block(other.Block)
    {
        if (Block)
            ++Block->RefCount;
    }
};

namespace NParticleEngine {
enum EPSCType { PSC_TYPE_NONE = 0 };
class CPSCBase;

class CComponentInstance {                                  // vtable 0x0129F478
public:
    virtual ~CComponentInstance();
    CCountedPointer<CPSCBase> Component;                    // +0x04
    bool                      Enabled;                      // +0x0c
    unsigned long             InstanceName;                 // +0x10
};
}

namespace _STL {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                    const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

typedef _STL::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> CComponentPair;

namespace _STL {
template CComponentPair* __fastcall __uninitialized_copy(CComponentPair*, CComponentPair*, CComponentPair*, const __false_type&);
}