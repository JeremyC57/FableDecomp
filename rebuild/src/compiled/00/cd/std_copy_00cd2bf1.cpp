// std::__copy(CScriptGameResourceObjectScriptedThingBase* first, CScriptGameResourceObjectScriptedThingBase* last, CScriptGameResourceObjectScriptedThingBase* result, const random_access_iterator_tag&, int*) -- STLport counted forward assignment of CScriptGameResourceObjectScriptedThingBase elements; returns the advanced result
// Leaf: ??$__copy@PAVCScriptGameResourceObjectScriptedThingBase@@PAV1@H@std@@YIPAVCScriptGameResourceObjectScriptedThingBase@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD2BF1, 45 bytes, __fastcall (first ecx, last edx; result, the tag reference and the distance pointer on the stack), `ret 0Ch`; size-optimised TU (result advanced in place, `add dword ptr [esp+0Ch],0x10`).
// Identity (proven): std::__copy<CScriptGameResourceObjectScriptedThingBase*, CScriptGameResourceObjectScriptedThingBase*, int>; evidence: the 16-byte element's assignment 0x008ABD10 is CScriptGameResourceObjectScriptedThingBase::operator= (the 8-byte base assignment 0x0099A3B0, then the counted PImp at +8/+0xC; Ego_r.pdb names it); the live caller 0x00CD3228 is that vector's erase(first, last), reached through 0x00CD3CCB from CQ_FireHeartScript::Init / DisableFollowers, whose Ego_r.pdb FollowerHandles / seh_Prophet members are vector<CScriptGameResourceObjectScriptedThingBase> (as CV_MurderTwistFinalScript::seh_Guards). Not CScriptThing: that element is 12 bytes and its __copy is the landed 0x008AC2A0. Mutable iterators.
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ...>" is propagated (family H060: six out-of-line __copy loops, one per element).
// Callee: CScriptGameResourceObjectScriptedThingBase::operator= 0x008ABD10 (the element assignment).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, random_access_iterator_tag, Distance*)` as written (the landed CWideString
//   0x00433CEC / CArray<unsigned short> 0x00F3B8A0 bodies), `__fastcall` on the template definition.
#pragma optimize("s", on)
class CScriptGameResourceObjectScriptedThingBase {         // Ego_r.pdb, 16 bytes: the 8-byte resource-object base, then the counted PImp +8
public:
    unsigned char _body[16];
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);
};

namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CScriptGameResourceObjectScriptedThingBase* __fastcall __copy<CScriptGameResourceObjectScriptedThingBase*, CScriptGameResourceObjectScriptedThingBase*, int>(CScriptGameResourceObjectScriptedThingBase*, CScriptGameResourceObjectScriptedThingBase*, CScriptGameResourceObjectScriptedThingBase*, const random_access_iterator_tag&, int*);
}