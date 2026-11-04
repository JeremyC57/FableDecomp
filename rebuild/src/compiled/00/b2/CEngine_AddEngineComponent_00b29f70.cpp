// CEngine::AddEngineComponent(T* component, T*& globalPointer, unsigned char flag) -- (T = CEngineSubPrimitiveRenderer) report load progress ("Engine: Add Engine Component"), publish the component through its global pointer, register that pointer with its CEngineComponent base (+0x4), append the base to the engine's component list and set up its console enable variable
// Leaf: ??$AddEngineComponent@VCEngineSubPrimitiveRenderer@@@CEngine@@AAEXPAVCEngineSubPrimitiveRenderer@@AAPAV1@E@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B29F70, 160 bytes, thiscall, `ret 0Ch`.
// Identity (proven): CEngine::AddEngineComponent<CEngineSubPrimitiveRenderer>(CEngineSubPrimitiveRenderer*, CEngineSubPrimitiveRenderer*&, unsigned char); evidence: the single caller 0x00B26360 (call at 0x00B266F4) passes `new` 0x190 bytes built by the constructor 0x00B84470, which stores vtables 0x012A3A28 / 0x012A3A60 (RTTI .?AVCEngineSubPrimitiveRenderer@@, base array CEngineSubPrimitiveRenderer <- CBaseClassNonCopyable <- CBaseClass <- CEngineComponent, CEngineComponent at mdisp 0x4 = the body's `lea edi, [esi + 0x4]`), and the global pointer 0x01436E74; the PDB name sets hold exactly this instantiation (Ego_r.pdb sizeof CEngineSubPrimitiveRenderer = 0x1D0)
// Label: the manifest's "AddEngineComponent<CEngineLightingManager>" is propagated (family J026: six AddEngineComponent<T> bodies whose T has its
//   CEngineComponent base at a non-zero offset; retail has no identical-code folding, so each address is one T).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z), NProgressDisplay::DisplayProgress
//   0x009E9F40 (?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z, `__fastcall`: text in ecx, the first bool in dl,
//   the float and the second bool on the stack), CCharString::~CCharString 0x0099EAE0, CEngineComponent::SetGlobalComponentPointer
//   0x00B59750 (?SetGlobalComponentPointer@CEngineComponent@@QAEXPAPAX@Z), STLport vector<CEngineComponent*>::_M_insert_overflow
//   0x00B292B0 (same-TU body, kept out of line), CEngineComponent::SetupEnableConsoleVariable 0x00B59760
//   (?SetupEnableConsoleVariable@CEngineComponent@@QAEXXZ).
// Layout: CEngine::EngineComponents (CArray<CEngineComponent*>, a 12-byte STLport vector) at +0x168 in retail (engine/CEngine.h
//   has +0x178 with 16-byte Ego_r CArrays), so `this` is a local model (the landed 0x00B29E30 model); CEngineComponent {vptr,
//   GlobalPointerAddress +4, RenderEnabled +8, KeepAlive +9} as in Ego_r; CEngineSubPrimitiveRenderer: primary base CBaseClassNonCopyable (0x4 bytes), CEngineComponent at +0x4 (RTTI); its own members are not used here.
// Shape: as the landed 0x00B29E30, plus the T* -> CEngineComponent* adjustment: the member calls go through the unchecked
//   base address (`lea edi, [esi + 0x4]`), the push_back argument is the null-checked conversion (`neg ; sbb ; and`)
//   homed in the dead `component` slot.
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

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};

class CEngineSubPrimitiveRenderer : public CBaseClassNonCopyable, public CEngineComponent {   // RTTI .?AVCEngineSubPrimitiveRenderer@@, CEngineComponent at +0x4
};

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

template void CEngine::AddEngineComponent<CEngineSubPrimitiveRenderer >(CEngineSubPrimitiveRenderer*, CEngineSubPrimitiveRenderer*&, unsigned char);