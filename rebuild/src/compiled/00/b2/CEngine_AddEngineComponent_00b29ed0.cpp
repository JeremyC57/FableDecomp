// CEngine::AddEngineComponent(T* component, T*& globalPointer, unsigned char flag) -- (T = CEngineScreenEffectDisplacementRenderer) report load progress ("Engine: Add Engine Component"), publish the component through its global pointer, tell it where that pointer lives, append it to the engine's component list and let it set up its console enable variable
// Leaf: ??$AddEngineComponent@VCEngineScreenEffectDisplacementRenderer@@@CEngine@@AAEXPAVCEngineScreenEffectDisplacementRenderer@@AAPAV1@E@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B29ED0, 148 bytes (+ int3 padding), thiscall, `ret 0Ch`.
// Identity: the single caller is the CEngine constructor (call at 0x00B266CA), which passes `new` 0x1AC bytes built by
//   0x00B86C00 -- that constructor stores vtable 0x012A3A94, RTTI `.?AVCEngineScreenEffectDisplacementRenderer@@` -- and the global pointer 0x01436E38; the
//   Ego_r name set holds exactly this instantiation (private: component, reference to the global pointer, an unsigned
//   char flag this body never reads).
//   Label correction: the manifest calls this AddEngineComponent<CEngineShadowRenderer> (propagated).
//   Family F009 (20 byte-identical bodies, one per engine component the CEngine constructor adds).
// Callees: CCharString(const char*, long = -1) 0x0099EBF0, NProgressDisplay::DisplayProgress 0x009E9F40 (`__fastcall`:
//   text in ecx, the first bool in dl, the float and the second bool on the stack), ~CCharString 0x0099EAE0,
//   CEngineComponent::SetGlobalComponentPointer(void**) 0x00B59750, STLport vector<CEngineComponent*>::_M_insert_overflow
//   0x00B292B0 (push_back growth), CEngineComponent::SetupEnableConsoleVariable() 0x00B59760 (both Ego_r names, the
//   only two non-virtual CEngineComponent members, in that order).
// Layout: CEngine::EngineComponents (CArray<CEngineComponent*>, a 12-byte STLport vector) at +0x168 in retail
//   (engine/CEngine.h puts it at +0x178: the Ego_r CArrays are 16 bytes), so `this` is a local model; CEngineComponent
//   {vptr, GlobalPointerAddress +4, RenderEnabled +8, KeepAlive +9} as in Ego_r.
// Shape: the progress text is a named CCharString in its own block (built before the arguments are pushed); push_back is
//   STLport's inline form (placement-new null test) with the T* -> CEngineComponent* temporary homed in the dead
//   `component` slot and the __true_type tag in the dead `flag` slot.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E (in the same-TU overflow body)
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

#include "engine/CArray.h"


class CEngineComponent {
public:
    virtual ~CEngineComponent();
    void** GlobalPointerAddress;                            // +0x04
    bool RenderEnabled;                                     // +0x08
    unsigned char KeepAlive;                                // +0x09
    void SetGlobalComponentPointer(void** address);         // 0x00B59750
    void SetupEnableConsoleVariable();                      // 0x00B59760
};

class CEngineScreenEffectDisplacementRenderer : public CEngineComponent { };   // CEngineComponent subobject at +0 (no `this` adjustment in the body)

class CEngine {
public:
    unsigned char _pad000[0x168];
    CArray<CEngineComponent*> EngineComponents;             // +0x168 (retail; Ego_r +0x178 with 16-byte CArrays)
private:
    template <class T> void AddEngineComponent(T* component, T*& globalPointer, unsigned char flag);
};

template <class T> void CEngine::AddEngineComponent(T* component, T*& globalPointer, unsigned char flag)
{
    {
        CCharString text("Engine: Add Engine Component");
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }
    globalPointer = component;
    component->SetGlobalComponentPointer((void**)&globalPointer);
    EngineComponents.push_back(component);
    component->SetupEnableConsoleVariable();
}

template void CEngine::AddEngineComponent<CEngineScreenEffectDisplacementRenderer >(CEngineScreenEffectDisplacementRenderer*, CEngineScreenEffectDisplacementRenderer*&, unsigned char);