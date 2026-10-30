// CM_BanditLeader::Main() -- bandit-leader spawner: when the "BanditLeader" thing is dead (or gone), spawn a new one here (creature def from this marker's data string, scripted "BanditLeader") and attach it to the "BanditTollVillage"
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4A740, length 335 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("BanditLeader" @ 0x012E4C14,
//   "BanditTollVillage" @ 0x012E4FB0), IsDone @ 0x00F35B30, ~CScriptThing @ 0x004AA840 (inlined at the first temporary,
//   out of line at the other two), CBaseClass::~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC (the count block);
//   CScriptThing slots 3 GetDataString, 6 GetPos, 76 IsDead (virtual on the returned temporary); CGameScriptInterface
//   (vtable 0x01260F0C) slots 72 GetThingWithScriptName, 91 CreateCreature, 492 EntityAttachToVillage.
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8}; CScriptThing {vptr, CCountedPointer PImp {object,
//   count block}} over CBase as the landed CScriptThing scalar deleting destructor 0x004AB170.
// Shape: the dead test is the `if` condition on the returned temporary (destroyed before the name string; VC7.1 inlines
//   the destructor at this first site only, given an inline `virtual ~CScriptThing() { }`); the new leader is scoped
//   inside the branch so it shares the frame words of the first temporary (function scope moves it, DIFFER 335v335).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    void*        m_pObject;                        // +0
    SCountBlock* m_pCount;                         // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                     // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing() { }                    // slot 0; complete destructor 0x004AA840
    virtual void v001(); virtual void v002();
    virtual CCharString GetDataString() const;                           // slot 3 (+0xc)
    virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;                             // slot 6 (+0x18)
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046();
    virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual bool IsDead() const;                                         // slot 76 (+0x130)
    CCountedPointer<CScriptThing> PImp;                          // +0x04
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CM_BanditLeader : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    void Main();
};

void CM_BanditLeader::Main()
{
    if (Game->GetThingWithScriptName("BanditLeader").IsDead()) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing leader = Game->CreateCreature(Me.GetDataString(), Me.GetPos(), "BanditLeader", false);
        Game->EntityAttachToVillage(leader, Game->GetThingWithScriptName("BanditTollVillage"));
    }
}