// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (CScriptGameResourceObjectScriptedThingBase*, CScriptGameResourceObjectScriptedThingBase*, int) STLport random-access backward copy: n = last - first assignments `*--result = *--last` through the element's out-of-line operator=, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAVCScriptGameResourceObjectScriptedThingBase@@PAV1@H@std@@YIPAVCScriptGameResourceObjectScriptedThingBase@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD2C7C, 47 bytes (size-optimised TU: unaligned entry), __fastcall (/Gr: first ecx,
//   last edx; result, the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy_backward<CScriptGameResourceObjectScriptedThingBase*, CScriptGameResourceObjectScriptedThingBase*, int>; evidence: the landed vector<CScriptGameResourceObjectScriptedThingBase>::_M_fill_insert 0x00CD378D declares and calls this __copy_backward (16-byte elements, vtable 0x0127094C RTTI .?AVCScriptGameResourceObjectScriptedThingBase@@); the other caller is the forwarder 0x00CD3119.
// Label: the manifest's "_Copy_backward<CArray<unsigned_short>*,CArray<unsigned_short>*>" is a propagated Dinkumware spelling (family J078: five backward copies, 8- or
//   16-byte strides; retail has no identical-code folding).
// Callees: CScriptGameResourceObjectScriptedThingBase::operator= 0x008ABD10 (binding ??4CScriptGameResourceObjectScriptedThingBase@@QAEAAV0@ABV0@@Z) (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim; the count is a shift (0x10-byte elements) and the
//   result pointer is decremented in its own stack slot (the J070 __copy model, backwards).
#pragma optimize("s", on)
class CBaseClass {                                       // RTTI root
public:
    virtual ~CBaseClass();
};
struct CBaseObjectPointer;
class CBaseObject : public CBaseClass {                  // Ego_r.pdb 8 bytes
public:
    CBaseObjectPointer* IntelligentPointer;              // +0x04
};
class CScriptGameResourceObjectBase : public CBaseObject { };   // Ego_r.pdb 8 bytes
class CCPPointerInfo;
template <class T> class CCountedPointer {               // 8 bytes
public:
    T*              Data;                                // +0x0
    CCPPointerInfo* Info;                                // +0x4
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {   // Ego_r.pdb 0x10, vtable 0x0127094C
public:
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
};
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
typedef CScriptGameResourceObjectScriptedThingBase TElement_00cd2c7c;
template TElement_00cd2c7c* __copy_backward<TElement_00cd2c7c*, TElement_00cd2c7c*, int>(TElement_00cd2c7c*, TElement_00cd2c7c*, TElement_00cd2c7c*,
                                                                         const random_access_iterator_tag&, int*);
}