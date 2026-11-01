// CSubtitleRenderer::~CSubtitleRenderer() -- compiler-generated destructor: the member strings, the text-info queue and the six vectors in reverse order, then the CBaseClassNonCopyable base
// Leaf: ??1CSubtitleRenderer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00443E20, 360 bytes, thiscall; ends `jmp ??1CBaseClassNonCopyable@@MAE@XZ` 0x0099A300.
// Identity: its callers are the slot-0 deleting destructor 0x004942A2 of vtable 0x012384BC, whose RTTI COL names
//   .?AVCSubtitleRenderer@@ (bases CBaseClassNonCopyable, CBaseClass; slot 0 is the only virtual), and the destructor of
//   an owner that holds a CSubtitleRenderer by value at +0x18 (0x0043B032). No vtable store inside = compiler-generated
//   (Ego_r ??1CSubtitleRenderer@@UAE@XZ is the implicit one; virtual through the base). Catalog
//   "_global::TextureCache_Destructor" is wrong.
// Layout: Ego_r.pdb CSubtitleRenderer members at retail STLport sizes, the offsets engine/CSubtitleRenderer.h gives
//   (0x120 bytes); that generated header is a flat byte layout (packed, containers as byte arrays) with no destructible
//   members, so the class is restated here with its real member types (header debt, NOTES.md).
//   TextGroupTag CWideString +0x30, TextInfoQueue queue<CSetTextInfo> +0x34 (STLport deque, 0x28), seven CWideStrings
//   +0x5C..+0x74, PrimitiveHandles / ButtonPrimitiveHandles / EmbeddedPrimitiveHandles vector<CEnginePrimitiveHandle>
//   +0x94/+0xA0/+0xAC (8-byte polymorphic elements), EmbeddedGraphics +0xB8 (trivial 16-byte elements),
//   EmbeddedHighlights +0xC4 (20-byte elements starting with a CWideString), EmbeddedHighlightActionNames
//   vector<CWideString> +0xD0, two trivially destructible CFadeInFadeOutBase at +0xE8 / +0x104.
// Callees: ~CWideString 0x0099B510, the out-of-line ~deque<CSubtitleRenderer::CSetTextInfo> 0x004427D0 (landed as
//   CUnprovenElement_004427d0), free 0x00BFEA14, ~CBaseClassNonCopyable 0x0099A300.
// Shape: no user destructor; the PDB ctor (here an empty body, not retail's) is defined only so that VC7.1 emits the
//   vtable, ??_G and the implicit ??1. STLport 4.6 vector text (_Destroy -> __destroy -> __destroy_aux on
//   __type_traits<T>::has_trivial_destructor) expands every vector inline in reverse member order: the virtual element
//   dtors as `push 0 ; call [vt]`, the trivial CEmbeddedGraphic loop vanishes. First probe.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "engine/CBaseClassNonCopyable.h"

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class T> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class I> inline void __destroy_aux(I first, I last, const __false_type&)
{
    for ( ; first != last; ++first)
        _Destroy(&*first);
}
template <class I> inline void __destroy_aux(I, I, const __true_type&) { }
template <class I, class T> inline void __destroy(I first, I last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(first, last, _Trivial_destructor());
}
template <class T> inline void _Destroy(T* first, T* last) { __destroy(first, last, (T*)0); }
template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef A allocator_type;
    explicit vector(const allocator_type& a = allocator_type()) : _Vector_base<T, A>(a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class T, class A = allocator<T> > class deque {   // STLport 4.6 deque, 0x28 bytes
public:
    unsigned char _M_body[0x28];                            // _M_start, _M_finish (16-byte iterators), _M_map, _M_map_size
    deque();
    ~deque();                                               // 0x004427D0 for CSubtitleRenderer::CSetTextInfo
};
template <class T, class Sequence = deque<T> > class queue {
public:
    Sequence c;                                             // +0x00
};
} // namespace std

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();
    ~CWideString();                                         // 0x0099B510
};
class CIVCountedPointeeBase;
class CEnginePersistentPrimitiveBase;
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
};
class CEnginePrimitiveHandle : public CBaseClass {          // 8 bytes, polymorphic
public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
    virtual ~CEnginePrimitiveHandle();
};
class C2DVector { public: float X, Y; };
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CRGBColour { public: unsigned char R, G, B, A; };
class CFadeInFadeOutBase {                                  // 0x1C
public:
    bool       Active;
    float      FadeInTime;
    float      FadeOutTime;
    bool       Closing;
    bool       Opening;
    float      OpenTimer;
    float      CloseTimer;
    CRGBColour ToColour;
};
class CFontBank;
class CGraphicDataBank;
enum EPlayerMode { };

class CSubtitleRenderer : public CBaseClassNonCopyable {
public:
    class CSetTextInfo {                                    // 0x1C
    public:
        CWideString   Text;
        CWideString   Button1Text;
        CWideString   Button2Text;
        CWideString   Button3Text;
        unsigned long DisplayFlags;
        float         DisplayTime;
        bool          UpdateWhilePaused;
        bool          ChangePlayerMode;
    };
    class CEmbeddedGraphic {                                // 0x10
    public:
        long      Graphic;
        long      CharIndex;
        C2DVector Offset;
    };
    class CEmbeddedHighlight {                              // 0x14
    public:
        CWideString Text;
        long        CharIndex;
        C2DVector   Offset;
        bool        Split;
    };
    CSubtitleRenderer();

    C2DBoxF            VWindow;                             // +0x04
    C2DBoxF            VTelevisionWindow;                   // +0x14
    long               TextCharactersShowing;               // +0x24
    bool               TextGroupHasEntriesRemaining;        // +0x28
    long               TextGroupIndex;                      // +0x2C
    CWideString        TextGroupTag;                        // +0x30
    std::queue<CSetTextInfo> TextInfoQueue;                 // +0x34
    CWideString        Text;                                // +0x5C
    CWideString        NameText;                            // +0x60
    CWideString        Button1Text;                         // +0x64
    CWideString        Button2Text;                         // +0x68
    CWideString        Button3Text;                         // +0x6C
    CWideString        Button1OverrideText;                 // +0x70
    CWideString        Button2OverrideText;                 // +0x74
    unsigned char      Button1Alpha;                        // +0x78
    unsigned char      Button2Alpha;                        // +0x79
    unsigned long      DisplayFlags;                        // +0x7C
    CFontBank*         PFont;                               // +0x80
    CGraphicDataBank*  Graphics;                            // +0x84
    float              MaxDisplayTime;                      // +0x88
    float              TextTimer;                           // +0x8C
    EPlayerMode        PlayerModeAdded;                     // +0x90
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;           // +0x94
    std::vector<CEnginePrimitiveHandle> ButtonPrimitiveHandles;     // +0xA0
    std::vector<CEnginePrimitiveHandle> EmbeddedPrimitiveHandles;   // +0xAC
    std::vector<CEmbeddedGraphic>       EmbeddedGraphics;           // +0xB8
    std::vector<CEmbeddedHighlight>     EmbeddedHighlights;         // +0xC4
    std::vector<CWideString>            EmbeddedHighlightActionNames; // +0xD0
    bool               PausedGame;                          // +0xDC
    bool               AllCharactersDisplayed;              // +0xDD
    bool               InCutscene;                          // +0xDE
    bool               CutsceneSkipped;                     // +0xDF
    bool               CutsceneSkippable;                   // +0xE0
    bool               CutsceneSkippableWhilePaused;        // +0xE1
    bool               CutsceneActionMode;                  // +0xE2
    bool               Button1TextOverridden;               // +0xE3
    bool               Button2TextOverridden;               // +0xE4
    bool               DisplayingText;                      // +0xE5
    bool               UpdateWhilePaused;                   // +0xE6
    bool               UpdateTimer;                         // +0xE7
    CFadeInFadeOutBase SkipCutsceneTimer;                   // +0xE8
    CFadeInFadeOutBase ButtonPulseTimer;                    // +0x104
};

CSubtitleRenderer::CSubtitleRenderer()
{
}