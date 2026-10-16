// CTCFadeOutAndIn::CTCFadeOutAndIn(CThing&) -- CTCBase(thing), not fading, fully opaque, no frames left, an empty fading-things vector, then the parent itself pushed as the first fading thing
// Leaf: ??0CTCFadeOutAndIn
//
// Retail 0x006C3C30, 112 bytes (`ret 4`). Ego_r.pdb `??0CTCFadeOutAndIn@@QAE@AAVCThing@@@Z`. Callees:
// CTCBase::CTCBase(CThing&) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10
// (then the CIntelligentPointer<CThing> vtable 0x01238C6C), ::SetPItem @ 0x00A01B90,
// vector<CIntelligentPointer<CThing>>::push_back(const value_type&) @ 0x004AD520 (manifest
// "Vector_PushBack_IntelligentPointer"), ::~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x0125DA0C =
// CTCFadeOutAndIn.
// Layout = Ego_r.pdb CTCFadeOutAndIn (0x28, no retail shift): FadingOut +0xc, FadingIn +0xd, Alpha
// +0x10, FramesLeftForFade +0x14, FadingThings +0x18 (12-byte STLport vector).
// Shape: plain initialisers with one zero register; the vector is the STLport `_Vector_base` +
// `_STLP_alloc_proxy` model addressed once in edi; the body pushes a CIntelligentPointer temporary
// built in the frame (base ctor, vtable store, SetPItem) and destroys it out of line.
struct CBaseObject;
struct CThing;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

template <class T> struct SRetailAllocProxy {               // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void push_back(const T& value);                         // 0x004AD520
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing& thing);                                 // 0x00686800
};

struct CTCFadeOutAndIn : CTCBase {                          // vtable 0x0125DA0C
    bool  FadingOut;                                        // +0x0c
    bool  FadingIn;                                         // +0x0d
    float Alpha;                                            // +0x10
    long  FramesLeftForFade;                                // +0x14
    CRetailVector<CIntelligentPointer<CThing> > FadingThings;   // +0x18
    virtual void v00();
    CTCFadeOutAndIn(CThing& thing);
};

CTCFadeOutAndIn::CTCFadeOutAndIn(CThing& thing)
    : CTCBase(thing)
    , FadingOut(false)
    , FadingIn(false)
    , Alpha(1.0f)
    , FramesLeftForFade(0)
    , FadingThings()
{
    FadingThings.push_back(CIntelligentPointer<CThing>(&thing));
}