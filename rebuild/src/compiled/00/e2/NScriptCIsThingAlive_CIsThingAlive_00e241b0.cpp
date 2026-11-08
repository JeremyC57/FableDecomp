// NScript::CIsThingAlive::~CIsThingAlive() -- this class's vtable (0x012C32F0), the CScriptThing member at +4 destroyed inline (its vtable 0x01238C8C, the counted block at +0xc released: destroy and delete at zero, both words cleared, CBaseClass::~CBaseClass @ 0x0099A2E0), then the functor base's vtable (0x012C2FF4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E241B0, length 96 bytes.
// Leaf: ??1CIsThingAlive@NScript@@UAE@XZ
// Identity (naming wave 26, c693 queue): Ego_r.pdb + FableWin.pdb ??1CIsThingAlive@NScript@@UAE@XZ (labels_naming_wave9 / labels_chunk571);
//   engine/CIsThingAlive.h (vtable 0x012C32F0: operator() 0x00CDF120, Clone 0x00CDF130, scalar deleting dtor 0x00CDF160 -> this body); the
//   final store is NScript::CPredicateBase's vtable 0x012C2FF4 (engine/CPredicateBase.h, inline destructor). Was
//   CScriptThingFunctor_00e241b0 (non-virtual ??1CScriptThingFunctor_00e241b0@@QAE@XZ) over a CThingFunctorBase_012c2ff4 stand-in base. The
//   CScriptThing member keeps the row's local model (its destructor expanded inline here, as retail; engine/CScriptThing.h declares it
//   out of line).
// Shape: `this` in edi, the member in esi.
#include "fable_compiler.h"
void __cdecl operator delete(void* block);         // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CPredicateBase.h"                    // NScript::CPredicateBase, vtable 0x012C2FF4
struct SCountBlock_00e241b0 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
class __declspec(novtable) CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0 (the base vtable store is elided: novtable)
};
struct CCountedPointer_00e241b0 {                      // the thing and its count block (a member object: its destructor runs after the vtable store)
    void*               m_pThing;                  // +0x00
    SCountBlock_00e241b0*   m_pCount;                  // +0x04
    FABLE_FORCEINLINE ~CCountedPointer_00e241b0()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pThing = 0;
        m_pCount = 0;
    }
};
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
    CCountedPointer_00e241b0 m_thing;                  // +0x04
    virtual ~CScriptThing() {}
};
namespace NScript {
class CIsThingAlive : public CPredicateBase {           // vtable 0x012C32F0 (engine/CIsThingAlive.h; CScriptThing local, see above)
public:
    virtual bool operator()() const;                    // slot 0  0x00CDF120
    virtual CPredicateBase* Clone() const;              // slot 1  0x00CDF130
    virtual ~CIsThingAlive();                           // slot 2  0x00CDF160 (scalar deleting)
    CScriptThing Thing;                                 // +0x04
};
}

NScript::CIsThingAlive::~CIsThingAlive()
{
}
