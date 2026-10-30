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

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const;                      // slot 75 (+0x12c), 0x004AB130
};

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