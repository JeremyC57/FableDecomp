// NPlayerGui::CDrawBase::ClearEffects(std::vector<CIntelligentPointer<CParticleEmitter> >& effects) -- kill every live particle emitter in the list and reset its pointer
// Leaf: ?ClearEffects@CDrawBase@NPlayerGui@@SIXAAV?$vector@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@V?$allocator@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@@std@@@std@@@Z
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- STLport operator[] as `*(begin() + n)` (and size() as end() - begin()) gives retail's `mov eax,[esi] ; lea ecx,[edi+eax]` second element fetch; a raw `_M_start + n` form is one byte shorter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006445BA, 109 bytes, plain `ret` (static __fastcall: effects in ecx).
// Size-optimised NPlayerGui TU (frame pointer, `leave`).
// Identity: Ego_r.pdb `?ClearEffects@CDrawBase@NPlayerGui@@SIXAAV?$vector@V?$CIntelligentPointer@VCParticleEmitter@
// NParticleEngine@@@@...@std@@@Z`; eleven callers across the NPlayerGui draw classes (0x00644647 .. 0x00652DB0).
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50 (twice per live entry), NParticleEngine::CParticleEmitter::Kill()
// 0x00AEE8C0, CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) 0x00655318, CIntelligentPointer::operator=
// 0x0065533C, ~CBaseIntelligentPointer 0x00A01C10 (as the landed CStatBar::DrawStatBox 0x00645CE9 model).
// Shape: an unsigned index loop against size() (8-byte elements, `sar 3`, re-read every pass, `jb`), the element
// re-fetched from _M_start for each use; the reset goes through a named null CIntelligentPointer local and operator=.
#pragma optimize("s", on)
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;                                      // 0x00A01B50
    ~CBaseIntelligentPointer();                                         // 0x00A01C10
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                                       // 0x00655318
    CIntelligentPointer& operator=(const CIntelligentPointer& other);  // 0x0065533C
    T* operator->() const { return (T*)GetPItem(); }
};

namespace NParticleEngine {
class CParticleEmitter { public:
    void Kill();                                                        // 0x00AEE8C0
};
}

namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

namespace NPlayerGui {

class CDrawBase {
public:
    static void __fastcall ClearEffects(std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >& effects);
};

void __fastcall CDrawBase::ClearEffects(std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >& effects)
{
    for (unsigned int i = 0; i < effects.size(); ++i) {
        if (effects[i].GetPItem()) {
            effects[i]->Kill();
            CIntelligentPointer<NParticleEngine::CParticleEmitter> none(0);
            effects[i] = none;
        }
    }
}

}