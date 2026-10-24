// std::vector<CIntelligentPointer<CParticleEmitter> >::resize(unsigned int newSize) -- one-argument resize: resize(newSize, value_type()) with an empty intelligent pointer built and destroyed in the frame
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657037, length 50 bytes, `leave ; ret 4`. Callers: the NPlayerGui CDraw*
// constructors (0x0064E239 in the landed CDrawHealth ctor 0x0064E1E4, which names it "vector<CIntelligentPointer
// <CParticleEmitter>>::Resize @ 0x00657037" on its Effects member; 0x0064E29A, 0x00651AF7, 0x00652F89, 0x0065428F,
// 0x00654386). The two-argument resize 0x00656F6D (manifest "CArray<pair<long,...>>::push_back", propagated) strides 8
// bytes (`sar edi,3`). The manifest label (_Dest_val<allocator<pair<CCharString,CScriptInfo>>>) is propagated and wrong.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, resize(unsigned int, const value_type&)
// @ 0x00656F6D, CBaseIntelligentPointer::~CBaseIntelligentPointer() @ 0x00A01C10; vtable 0x0125A26C = CIntelligentPointer
// (rtti-vtables.tsv).
// Opt: the CDraw region's frame-pointer size TU (GOTCHAS: `#pragma optimize("y", off)` + `("s", on)`).
// Shape: the landed resize(n) form; the temporary's inline ctor (out-of-line base ctor, own vtable store scheduled after
// the argument pushes) and its compiler-generated dtor (no vtable store, out-of-line base dtor).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CBaseIntelligentPointer {
    virtual void v000();
    void* PItem;                                             // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0125A26C
    CIntelligentPointer() {}
    virtual void v000();
};

class CParticleEmitter;

namespace std {
template<class T> struct vector {                            // three words: first/last/end
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    void resize(unsigned int newSize, const T& value);       // 0x00656F6D for T = CIntelligentPointer<CParticleEmitter>
    void resize(unsigned int newSize) { resize(newSize, T()); }
};
}

template struct std::vector<CIntelligentPointer<CParticleEmitter> >;