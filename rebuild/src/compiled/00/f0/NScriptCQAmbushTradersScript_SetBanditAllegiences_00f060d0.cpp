// CQ_AmbushTradersScript::SetBanditAllegiences(const CScriptThing& bandit) -- refill the convoy list from the "AT_ConvoyMember" things and make every living member and the bandit enemies of each other
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F060D0, length 270 bytes, `ret 4`.
// Callees: CScriptThing's virtual destructor (slot 0, flag 0; the inline pop_back loop), CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::IsAlive (virtual slot 75, +0x12c), CScriptBase::
// IsActiveThreadTerminating 0x00CB7940; through the game script interface (+0x40): GetAllThingsWithScriptName (slot 75,
// +0x12c, 0x008A8570), EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) (slot 597, +0x954,
// 0x0089C000, both directions). Literal "AT_ConvoyMember" 0x012F7CF8.
// Layout: Ego_r.pdb CQ_AmbushTradersScript - 0x14, -4 per preceding vector: ConvoyMembersVector +0xd8.
// Shape: the convoy vector is used as the member everywhere (VC keeps its address in esi; a reference local swaps
// the first [edi+edx] operand order); `while (size() != 0) pop_back();` keeps the -12 step in edi;
// the name is an implicit conversion temporary homed in the entry `push ecx` slot; the loop re-tests size() (`jb`).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C (slot 0: virtual dtor)
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

namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                     // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
}
using std::vector;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x90];
    vector<CScriptThing>  ConvoyMembersVector;       // +0xd8
    void SetBanditAllegiences(const CScriptThing& bandit);
};
} // namespace NScript
using NScript::CQ_AmbushTradersScript;

void NScript::CQ_AmbushTradersScript::SetBanditAllegiences(const CScriptThing& bandit)
{
    while (ConvoyMembersVector.size() != 0)
        ConvoyMembersVector.pop_back();
    m_pInterface->GetAllThingsWithScriptName("AT_ConvoyMember", ConvoyMembersVector);
    for (unsigned int i = 0; i < ConvoyMembersVector.size(); ++i) {
        if (ConvoyMembersVector[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            m_pInterface->EntitySetThingAsEnemyOfThing(bandit, ConvoyMembersVector[i]);
            m_pInterface->EntitySetThingAsEnemyOfThing(ConvoyMembersVector[i], bandit);
        }
    }
}