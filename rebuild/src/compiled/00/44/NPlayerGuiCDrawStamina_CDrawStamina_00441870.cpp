// NPlayerGui::CDrawStamina::~CDrawStamina() -- implicit destructor: destroy the IncreaseEffects and Effects emitter-pointer vectors, then CDrawBase's primitive-handle vector (virtual element destructors, then free the storage)
// Leaf: ??1CDrawStamina
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00441870, length 147 bytes, plain `ret` (thiscall).
// Callees: free @ 0x00BFEA14 (STLport allocator deallocate); the element destructors run through vtable slot 0 (`push 0 ;
// call [vptr]`, the virtual ~CIntelligentPointer / ~CEnginePrimitiveHandle).
// Label: the catalog "List_DestroyElements" is behavioural. Callers: the `scalar deleting destructor' 0x00442980 and the
// CCountedPointer<NPlayerGui::CDrawStamina> deleter 0x00443520, whose Reset (0x004440C0) the CPlayerGui constructor uses for
// the member it fills with `new (0x64) CDrawStamina` (ctor 0x0064E24F, landed) -- the PDB's PDrawStamina.
// Layout (the landed CDrawStamina ctor): CDrawBase::PrimitiveHandles +0x1C (vector<CEnginePrimitiveHandle>), Effects +0x28 and
// IncreaseEffects +0x34 (vector<CIntelligentPointer<CParticleEmitter>>, 8-byte polymorphic elements), PreviousStamina +0x40,
// PreviousMaxStamina +0x44, StaminaIncreaseFade +0x48 (trivial destructor).
// Model: the class declares no destructor: VC7.1 emits the compiler-generated one out of line for the retail-style
// `delete p` deleter below and stores no vtable in it. STLport vector teardown: `_Destroy(start, finish)` (explicit
// virtual element destructor per element) then `~_Vector_base` (`if (start) free(start)`). Speed TU (aligned loop heads).
#include <stddef.h>
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
void __cdecl operator delete(void* p);             // 0x00BFE9BC
struct CPlayerGui;

namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) free(_M_start); }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

struct CEnginePrimitiveHandle {                    // vptr + counted primitive pointer
    virtual ~CEnginePrimitiveHandle();
    void* Data;
};
struct CParticleEmitter;
template <class T> struct CIntelligentPointer {    // vptr + item
    virtual ~CIntelligentPointer();
    void* m_pItem;
};

namespace NPlayerGui {
struct CFadeInFadeOutBase {
    unsigned char _body[0x1c];
};

struct CDrawBase {
    virtual void v00();
    unsigned char                        _body[0x1c - 4];   // +0x04 .. +0x1c
    std::vector<CEnginePrimitiveHandle>  PrimitiveHandles;  // +0x1c
};

struct CDrawStamina : CDrawBase {                  // vtable 0x0125A618
    virtual void v00();
    std::vector<CIntelligentPointer<CParticleEmitter> > Effects;           // +0x28
    std::vector<CIntelligentPointer<CParticleEmitter> > IncreaseEffects;   // +0x34
    long                                                PreviousStamina;   // +0x40
    long                                                PreviousMaxStamina;// +0x44
    CFadeInFadeOutBase                                  StaminaIncreaseFade; // +0x48
};
}

void __fastcall DestroyDrawStamina(NPlayerGui::CDrawStamina* p)   // the CCountedPointer deleter shape (0x00443520)
{
    delete p;
}