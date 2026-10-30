// CQ_HangingTreeGoodScript::SetGuardsHealth() -- re-apply the saved guard health values: for every living guard with a non-zero saved value, change its health by (saved - current), then clear the saved list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D766C0, length 291 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptThing::IsAlive (virtual slot 75, +0x12c),
// __ftol2 0x00BFEA70 (the float health truncated to a long), vector<float>::erase(float*, float*) 0x00440550 (out of
// line, memmove form; propagated label "Buffer_MoveData"); through the game script interface (+0x40): GetHealth(const
// CScriptThing&) (slot 264, +0x420, 0x00890400), ModifyThingHealth(const CScriptThing&, float, bool) (slot 265, +0x424,
// 0x00890430). Float 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CQ_HangingTreeGoodScript - 0x14 (retail 12-byte vectors): CurrentGuardVector +0xac,
// GuardsHealthVector +0xd0.
// Shape: the health vector is a reference local (ebp); `size()` is recomputed by the loop test (unsigned `jb`); the
// truncated health is a named long homed in the entry `push ecx` slot; clear() is the inline `erase(begin(), end())`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

template <class T> struct vector {                     // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                       // vector<float>: 0x00440550
    void clear() { erase(begin(), end()); }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_HangingTreeGoodScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x64];
    vector<CScriptThing>  CurrentGuardVector;        // +0xac
    unsigned char         _padb8[0x18];              // RegionsWhereSpawned, AreasSpawnedIn
    vector<float>         GuardsHealthVector;        // +0xd0
    void SetGuardsHealth();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

void NScript::CQ_HangingTreeGoodScript::SetGuardsHealth()
{
    vector<float>& health = GuardsHealthVector;
    if (health.size() == 0)
        return;
    for (unsigned int i = 0; i < CurrentGuardVector.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        if (CurrentGuardVector[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            long current = m_pInterface->GetHealth(CurrentGuardVector[i]);
            if (health[i] != 0.0f)
                m_pInterface->ModifyThingHealth(CurrentGuardVector[i], health[i] - current, false);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    health.clear();
}