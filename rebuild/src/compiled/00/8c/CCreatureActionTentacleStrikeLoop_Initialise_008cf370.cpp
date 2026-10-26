// CCreatureAction_TentacleStrikeLoop::Initialise() -- names the animation "TENTACLE_SMASH_LOOP", stamps the start frame at +0x114 (if still -1) from the world frame counter, remembers the creature's dword at +0xb4 in +0x11c, then runs the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF370, length 96 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation @ 0x008574D0 (manifest label propagated),
// CCharString::~CCharString @ 0x0099EAE0, GetWorldFrame @ 0x0049D870 (returns the global @ 0x013B89BC; the manifest's "GFGetPreMainMemoryUsed"
// label is propagated -- landed naming of 0x006929E0), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CCombatActionBase::Initialise
// @ 0x00857520 (non-virtual call).
// Shape: the landed "scoped name + base Initialise" family (0x008CC760) with two member stores in between.
#include "fable_string.h"

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};              // 0x0049D870 (returns the global @ 0x013B89BC)

class CThingCreatureBase { public:                 // only the dword at +0xb4 is modelled
    unsigned char _pad[0xb4];
    long m_targetId;                        // +0xb4
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +8
    unsigned char _pad10[0x114 - 0x10];
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_TentacleStrikeLoop : public CCombatActionBase { public:
    long m_startFrame;                      // +0x114 (-1 until the first Initialise)
    long m_118;                             // +0x118 untouched here
    long m_targetId;                        // +0x11c
    virtual void Initialise();
};

void CCreatureAction_TentacleStrikeLoop::Initialise()
{
    {
        CCharString name("TENTACLE_SMASH_LOOP", -1);
        SetCombatAnimation(name);
    }
    if (m_startFrame == -1)
        m_startFrame = CWorld::GetFrame();
    m_targetId = PCreature.GetPItem()->m_targetId;
    CCombatActionBase::Initialise();
}