#ifdef FABLE_RETAIL_SCOPED_PTR
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
// boost::scoped_ptr (Boost 1.30 boost/scoped_ptr.hpp): one T* member; the accessors are inline (retail reads the
// pointer straight through them, e.g. CMainGameComponent::PeekWorld). Union of the members used by the rows that need it.
namespace boost {
#ifdef FABLE_RETAIL_SCOPED_PTR_RESET
// Boost 1.30 boost/checked_delete.hpp (FableWin.pdb ??$checked_delete@...@boost@@YAXPAV...@Z; inline in retail).
template<class T> inline void checked_delete(T* x) { typedef char type_must_be_complete[sizeof(T)]; delete x; }
#endif
template<class T> class scoped_ptr {
public:
    explicit scoped_ptr(T* p=0):ptr(p) {}
#ifdef FABLE_RETAIL_SCOPED_PTR_RESET
    // Nested opt-in (chunk 777, row 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest): Boost 1.30
    // scoped_ptr.hpp's inline destructor (checked_delete), reset (`this_type(p).swap(*this)`) and swap; retail expands all three
    // (0x0055F73A: the old pointer read, the new one stored, then `~LHHttp2; operator delete` on the old one). FableWin.pdb
    // ?reset@?$scoped_ptr@VLHHttp2@LHNetworkLib@@@boost@@QAEXPAVLHHttp2@LHNetworkLib@@@Z / ?swap@...@QAEXAAV12@@Z.
    ~scoped_ptr() { checked_delete(ptr); }
    void reset(T* p=0) { scoped_ptr<T>(p).swap(*this); }
    void swap(scoped_ptr& b) { T* tmp=b.ptr; b.ptr=ptr; ptr=tmp; }
#else
    ~scoped_ptr();
#endif
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
#ifdef FABLE_RETAIL_SCOPED_PTR_BOOL
    // Nested opt-in (asm round 2b, row 0x009BEF80 CDisplayManager::SetViewport): Boost 1.30 scoped_ptr.hpp's implicit
    // conversion to unspecified_bool_type (`ptr == 0 ? 0 : &this_type::get`); retail tests it as `mov edx,ecx ; neg edx ;
    // sbb edx,edx ; test edx,offset get` (0x009BF138), the out-of-line get 0x009C1220 (`mov eax,[ecx] ; ret`) being the
    // member pointer's target (FableWin.pdb ?get@?$scoped_ptr@VCRenderManager@@@boost@@QBEPAVCRenderManager@@XZ).
    typedef T* (scoped_ptr::*unspecified_bool_type)() const;
    operator unspecified_bool_type() const { return ptr == 0 ? 0 : &scoped_ptr::get; }
#endif
private:
    T* ptr;
};
}
#endif // FABLE_BOOST_SCOPED_PTR_H
#else
#ifndef FABLE_BOOST_SCOPED_PTR_H
#define FABLE_BOOST_SCOPED_PTR_H
namespace boost {
template<class T> class scoped_ptr {
public: explicit scoped_ptr(T* p=0):ptr(p) {}
 ~scoped_ptr();
private: T* ptr;
};
}
#endif

#endif // FABLE_RETAIL_SCOPED_PTR
