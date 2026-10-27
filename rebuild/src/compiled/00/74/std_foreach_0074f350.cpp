// std::for_each(CThingObject* const* first, CThingObject* const* last, CAddObjectNotSolidTo f) -- make every object in the range not solid to the functor's CTCSolidToThing
// Leaf: ??$for_each@PBQAVCThingObject@@VCAddObjectNotSolidTo@@@std@@YI?AVCAddObjectNotSolidTo@@PBQAVCThingObject@@0V1@@Z
// Retail: Fable.exe @ 0x0074F350, 64 bytes, __fastcall (ecx = the hidden functor result, edx = first), `ret 8` (last and
//   the 4-byte functor on the stack).
// Identity (proven): FableWin.pdb `??$for_each@...VCAddObjectNotSolidTo@@@std@@` over vector<CThingObject*>::const_iterator
//   (STLport's is `CThingObject* const*`) with `??0CAddObjectNotSolidTo@@QAE@PAVCTCSolidToThing@@@Z` and
//   `??RCAddObjectNotSolidTo@@QAEXPAVCThingObject@@@Z`; Ego_r lays the functor out as one `CTCSolidToThing*
//   TCSolidToThing` (+0). The body is STLport `for (; first != last; ++first) f(*first); return f;` with the inline
//   operator() calling CTCSolidToThing::AddThingNotSolidTo(const CThing*) 0x00770FA0 (landed). The caller 0x0074FA80..
//   builds the functor from the thing's interface 0x4D (CTCSolidToThing) and passes a vector's first/last.
class CThing { public:
};
class CThingObject : public CThing { public:
};
class CTCSolidToThing { public:
    void AddThingNotSolidTo(const CThing* thing);       // 0x00770FA0
};
class CAddObjectNotSolidTo { public:
    CTCSolidToThing* TCSolidToThing;                    // +0x00
    CAddObjectNotSolidTo(CTCSolidToThing* tc) : TCSolidToThing(tc) {}
    void operator()(CThingObject* object) { TCSolidToThing->AddThingNotSolidTo(object); }
};

namespace std {
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for ( ; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
template CAddObjectNotSolidTo __fastcall for_each(CThingObject* const*, CThingObject* const*, CAddObjectNotSolidTo);
}