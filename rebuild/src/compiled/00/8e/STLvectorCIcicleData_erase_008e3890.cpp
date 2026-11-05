// std::vector<CIcicleData>::erase(CIcicleData* position) -- remove one ice-troll icicle record: shift the tail down one slot (when the erased record is not the last), destroy the vacated last record, return the position
// Leaf: ?erase@?$vector@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3890, 64 bytes, `__fastcall` (this -> esi, position on the stack -> edi),
//   `ret 4`, returns the position.
// Identity: the ice-troll icicle TU (landed vector<CIcicleData>::_M_insert_overflow 0x008E3AA0, the C3DVector swaps
//   0x008E2DB0/0x008E3150); the 0x24-byte element is Ego_r.pdb CIcicleData (CIntelligentPointer<CThing> PEffect +0,
//   PTrap +8, long EffectTimer +0x10, TrapTimer +0x14, C3DVector Position +0x18) -- the layout the callee 0x008E34C0
//   copies (two pointer assignments, two words, a three-word block). Caller: 0x008DEC59. The catalog's
//   "std_list_remove<CIntelligentPointer<CThing>, NStealth::CDangerStatus>" is propagated.
// Callees: STLport __copy<CIcicleData*, CIcicleData*, int>(first, last, result, const random_access_iterator_tag&, int*)
//   0x008E34C0 (/Gr `__fastcall`), ~CBaseIntelligentPointer 0x00A01C10 (twice: PTrap, then PEffect).
// Shape: STLport 4.6 `if (__position + 1 != end()) __copy_ptrs(__position + 1, _M_finish, __position, __false_type());
//   --_M_finish; _Destroy(_M_finish); return __position;` -- __copy_ptrs forwards to the out-of-line random-access
//   __copy with an empty tag homed in the dead position slot (`lea eax,[esp+10h]`) and a null distance pointer;
//   CIcicleData's generated destructor runs the base pointer destructors directly (no vtable reset).
class CBaseObject;
class CThing;

#include "engine/C3DVector.h"

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CIcicleData {                                     // Ego_r.pdb, 0x24 bytes
public:
    CIntelligentPointer<CThing> PEffect;                // +0x00
    CIntelligentPointer<CThing> PTrap;                  // +0x08
    long                        EffectTimer;            // +0x10
    long                        TrapTimer;              // +0x14
    C3DVector                   Position;               // +0x18
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x008E34C0

template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> inline void __fastcall _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* end() { return _M_finish; }
    _Tp* erase(_Tp* __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, __false_type());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};

template class vector<CIcicleData>;
}