// std::for_each(NAnimationEvents::CEvent* first, NAnimationEvents::CEvent* last, CCollectEventNames f) -- applies an 8-byte functor (out-of-line operator() 0x006FCC80) to every 8-byte animation event and returns it
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FCE00, 54 bytes, `ret 0Ch`. Caller: 0x006F9B9C in the animation-event manager (0x006F9B50),
// which walks an event array found through a CVectorMap. Callee 0x006FCC80 (manifest "ParticleMorphs_BuildEntry" is wrong) masks
// the event id with 0x3FFFFFFF and appends a name to the functor's vector; element and functor names are descriptive (the catalog's
// CTargetPoint/CCollectEventNames label is propagated across byte-identical bodies).
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: `push esi ; lea ecx,[esp+18h] ; call` -- element by reference, functor in its argument slot; 8-byte copy-out.
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
namespace NAnimationEvents {
class CEvent {                                    // 8 bytes: id (top two bits are flags) + time
public:
    unsigned long m_idAndFlags;                   // +0x00
    float m_time;                                 // +0x04
};
}
class CCollectEventNames {
public:
    void* m_pNames;                               // +0x00 the output vector
    void* m_pManager;                             // +0x04
    CCollectEventNames(void* names, void* manager) : m_pNames(names), m_pManager(manager) {}
    void operator()(const NAnimationEvents::CEvent& p);   // 0x006FCC80
};
template CCollectEventNames __fastcall std::for_each(NAnimationEvents::CEvent*, NAnimationEvents::CEvent*, CCollectEventNames);