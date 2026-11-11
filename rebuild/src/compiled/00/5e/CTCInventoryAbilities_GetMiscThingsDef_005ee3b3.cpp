// CTCInventoryAbilities::GetMiscThingsDef() -- look up the "UI_MISC_THINGS_DEF" definition's index and return a counted pointer to that def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE3B3, length 101 bytes, `ret 4` (hidden result pointer; `this` in ecx
//   unused).
// Identity: called with ecx = the component and a frame result slot from CTCInventoryAbilities code (0x005EEEB5,
//   0x005F1B69, ... ; 0x005EE418 is CTCInventoryAbilities::FrameUpdate), so it is a member of that TU's class; the
//   catalog label "CCreatureAction_CastStrengthMultiStrikeProper" is propagated. Ego_r.pdb names it
//   `?GetMiscThingsDef@CTCInventoryAbilities@@ABE?BV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@XZ`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0 (returns
//          the global manager), CDefinitionManager::GetDefGlobalIndex(const CCharString&, bool) @ 0x009AD390 (manifest
//          label "ActualToUniqueIndex" is propagated), CCharString::~CCharString @ 0x0099EAE0,
//          CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) @ 0x0042B312.
// Shape: a size-optimised frame-pointer TU (`optimize("y",off)` + `("s",on)`: `push ebp ; mov ebp,esp ... leave`,
//   `and dword ptr [ebp-4],0`); the name string lives in its own block and its frame word is reused by the def
//   pointer local (zeroed by its inline constructor); `return def;` copy-constructs the hidden result (add-ref) and the
//   local's destructor releases -- one shared null test with both `inc`/`dec [ecx+4]` kept, slot 1 on zero.
// Leaf: GetMiscThingsDef
// Fix (retry 10): the def local's release stays register-held once GetDef<T> has its same-TU operator=-shaped body
//   (defreal), and CGameDefinitionManager::Get is the `__fastcall` static returning a reference (a plain static Get()
//   hoisted `lea eax,[ebp-4]` above `push 1`).
#include "fable_compiler.h"
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefBase {                           // counted game definition: vtable + reference count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_refCount;                        // +4
};

// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T>
class CDefPointer { public:
    typename CDefNonConst<T>::Type* m_pDef;
    FABLE_FORCEINLINE CDefPointer() : m_pDef(0) { }
    FABLE_FORCEINLINE CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    FABLE_FORCEINLINE ~CDefPointer() { if (m_pDef) { if (--m_pDef->m_refCount == 0) m_pDef->Destroy(); } }
};

namespace NUISystem { class CUIMiscThingsDef : public CDefBase { }; }

class CDefinitionManager { public:
    long GetDefGlobalIndex(const CCharString& name, bool mustExist);          // 0x009AD390
    template <class T> bool GetDef(long index, CDefPointer<const T>* result) const;   // 0x0042B312 GetDef<NUISystem::CUIMiscThingsDef> (Ego_r.pdb)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <> __declspec(noinline) bool CDefinitionManager::GetDef<NUISystem::CUIMiscThingsDef>(long p0, CDefPointer<const NUISystem::CUIMiscThingsDef>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

class CTCInventoryAbilities { private:
    const CDefPointer<const NUISystem::CUIMiscThingsDef> GetMiscThingsDef() const;   // Ego_r.pdb: private, const return
};

const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long index;
    {
        CCharString name("UI_MISC_THINGS_DEF", -1);
        index = CGameDefinitionManager::Get().GetDefGlobalIndex(name, true);
    }
    {
        CDefPointer<const NUISystem::CUIMiscThingsDef> def;      // sibling block: shares the string's frame word
        CGameDefinitionManager::Get().GetDef(index, &def);
        return def;
    }
}