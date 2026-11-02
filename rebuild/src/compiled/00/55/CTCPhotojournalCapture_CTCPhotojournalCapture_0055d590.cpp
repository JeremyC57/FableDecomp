// CTCPhotojournalCapture::~CTCPhotojournalCapture() -- store the photojournal-capture vtable, leave the global TC update list, then destroy the UI component vector and the FileName / Description strings, then ~CTCBase
// Leaf: ??1CTCPhotojournalCapture@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055D590, 54 bytes, thiscall, tail `jmp ~CTCBase` 0x00686830; no callers (the slot-0
//   `??_G` 0x0055DE40 of vtable 0x0124C4B4 holds the same body inline).
// Identity (proven): the first store is vtable 0x0124C4B4 (RTTI .?AVCTCPhotojournalCapture@@ : CTCBase : CBaseClassNonCopyable :
//   CBaseClass; slot 4 OnCreate 0x0055CFA0, 11 DrawPreparePrimitivesForRendering 0x0055D5D0, 21 / 22 GetInterfaceType /
//   GetClassTypeName, 26 GlobalUpdate 0x0055D4D0, the new slot 28 HasGlobalUpdate 0x0055DE30: the FableWin.pdb overrides);
//   Ego_r.pdb / FableWin.pdb ??1CTCPhotojournalCapture@@UAE@XZ (a user dtor: the store and the RemoveFromGlobalTCs body), the body
//   ??_GCTCPhotojournalCapture 0x0055DE40 inlines (landed). The catalog label CExclusiveAssert::~CExclusiveAssert is propagated.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (?RemoveFromGlobalTCs@CTCBase@@QAEXXZ, Ego_r + FableWin), free 0x00BFEA14
//   (~_Vector_base's deallocate), CWideString::~CWideString 0x0099B510 (x2), CTCBase::~CTCBase 0x00686830 (tail).
// Layout (Ego_r.pdb members at STLport sizes, as the landed GlobalUpdate 0x0055D4D0): Description +0x0c, DiscardScreenShot +0x10,
//   FrameCounter +0x14, FirstRenderFrame +0x18, ScreenOpen +0x19, LastRenderTime +0x20, FileName +0x28, Components
//   (vector<NUISystem::CComponent*>, 0xc) +0x2c, PMannequin +0x38, WasPlayerGuiOpen +0x3c, TakeScreenshotAfterFrames +0x40; 0x48.
// Shape: own vtable store, the user body `RemoveFromGlobalTCs();`, then the member dtors in reverse order (the STLport vector frees
//   its block, the two strings), the base dtor as the tail. Base: engine/CTCBase.h retail opt-in (+ the chunk 712
//   RemoveFromGlobalTCs declaration, hdr/).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable; RemoveFromGlobalTCs 0x00686DF0

class CWideStringData;
class CWideString {
public:
    ~CWideString();                                         // 0x0099B510
    CWideStringData* PStringData;
};
namespace NUISystem { class CComponent; }

class CTCPhotojournalCapture : public CTCBase {             // vtable 0x0124C4B4, 0x48 bytes
public:
    CTCPhotojournalCapture(CThing& thing);                  // ??0CTCPhotojournalCapture@@QAE@AAVCThing@@@Z
    virtual ~CTCPhotojournalCapture();
    virtual void OnCreate();                                //  4 0x0055CFA0
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 11 0x0055D5D0
    virtual ETCInterfaceType GetInterfaceType() const;      // 21 0x0055DE00
    virtual CCharString GetClassTypeName() const;           // 22 0x0055DE10
    virtual void GlobalUpdate(bool flag);                   // 26 0x0055D4D0
    virtual bool HasGlobalUpdate();                         // 28 0x0055DE30 (new slot)
    CWideString                           Description;      // +0x0c
    bool                                  DiscardScreenShot;    // +0x10
    long                                  FrameCounter;     // +0x14
    bool                                  FirstRenderFrame; // +0x18
    bool                                  ScreenOpen;       // +0x19
    double                                LastRenderTime;   // +0x20
    CWideString                           FileName;         // +0x28
    std::vector<NUISystem::CComponent*>   Components;       // +0x2c
    CThing*                               PMannequin;       // +0x38
    bool                                  WasPlayerGuiOpen; // +0x3c
    long                                  TakeScreenshotAfterFrames;   // +0x40
};
FABLE_STATIC_ASSERT(sizeof(CTCPhotojournalCapture) == 0x48);

CTCPhotojournalCapture::~CTCPhotojournalCapture()
{
    RemoveFromGlobalTCs();
}