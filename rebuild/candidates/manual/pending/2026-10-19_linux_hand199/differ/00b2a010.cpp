// CEngine::AddEngineComponent<CEngineLightingManager>(CEngineLightingManager* component, CEngineLightingManager** globalPointer, bool) -- report "Engine: Add Engine Component" on the loading display, publish the new component through its global pointer, register it in the engine's component list and run its post-add hook
// Grade: DIFFER(163v163) -- only the frame slot of the up-cast push_back temporary: retail homes it in the dead `component` slot [esp+14h], ours in the `globalPointer` slot [esp+18h]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2A010, length 176 bytes (ret 0Ch; called once from the engine start-up @ 0x00B2671E).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; __fastcall DisplayProgress(const CCharString&,
// float, bool, bool) 0x009E9F40; CEngineComponent::SetGlobalPointerAddress(void**) 0x00B59750 (stores +4); the component-list
// insert-and-grow 0x00B292B0 (STLport _M_insert_overflow); CEngineComponent::OnAddedToEngine 0x00B59760 (calls its own virtual
// slot 9 for the CCharString name and drops it -- the class of that hook is the component base, its purpose is a guess).
// Layout: the CEngineComponent base (PDB 0x0C: vptr, GlobalPointerAddress +4, RenderEnabled +8, KeepAlive +9) sits at +0x268
// of CEngineLightingManager (the PDB header lists no bases); the engine's component vector is the retail 12-byte STLport
// vector at +0x168 (PDB CArray EngineComponents +0x178: every earlier container is 4 bytes smaller in retail).
// Shape: the string is a block-scoped named local destroyed right after the progress call; the up-cast to the component base
// is the null-guarded `neg/sbb/and` adjust, homed in the dead first-argument slot because push_back takes a const reference.
// The third argument is unused by every instantiation.
#include <new>
#include "fable_string.h"

void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40

struct CEngineComponent {                       // PDB 0x0C
    virtual void v00();
    void** GlobalPointerAddress;                // +0x04
    bool   RenderEnabled;                       // +0x08
    unsigned char KeepAlive;                    // +0x09
    void SetGlobalPointerAddress(void** address);   // 0x00B59750
    void OnAddedToEngine();                     // 0x00B59760
};

struct CEngineLightingManagerBody {             // the polymorphic primary part before the component base (opaque here)
    virtual void v00();
    unsigned char _body[0x264];
};

struct CEngineLightingManager : CEngineLightingManagerBody, CEngineComponent {
};

struct SScalarTag { SScalarTag() { } };

struct CEngineComponentVector {                 // STLport vector<CEngineComponent*>
    CEngineComponent** _M_start;
    CEngineComponent** _M_finish;
    CEngineComponent** _M_end_of_storage;
    void _M_insert_overflow(CEngineComponent** position, CEngineComponent* const& value, const SScalarTag& tag,
                            unsigned long count, bool atEnd);   // 0x00B292B0
    void push_back(CEngineComponent* const& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CEngineComponent*(value);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, SScalarTag(), 1, true);
    }
};

struct CEngine {
    unsigned char          _pad000[0x168];
    CEngineComponentVector EngineComponents;    // +0x168
    template <class T> void AddEngineComponent(T* component, T** globalPointer, bool unused);
};

template <class T> void CEngine::AddEngineComponent(T* component, T** globalPointer, bool)
{
    {
        CCharString text("Engine: Add Engine Component", -1);
        DisplayProgress(text, -1.0f, false, false);
    }
    *globalPointer = component;
    component->SetGlobalPointerAddress((void**)globalPointer);
    EngineComponents.push_back(component);
    component->OnAddedToEngine();
}

template void CEngine::AddEngineComponent<CEngineLightingManager>(CEngineLightingManager*, CEngineLightingManager**, bool);
