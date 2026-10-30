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

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

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