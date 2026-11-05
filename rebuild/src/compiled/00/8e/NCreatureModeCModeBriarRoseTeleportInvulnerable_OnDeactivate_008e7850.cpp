// NCreatureMode::CModeBriarRoseTeleportInvulnerable::OnDeactivate() -- base deactivate, resume the owner's animation, drop TC 0x93, restore full alpha, kill the summoned helper if still alive
// Leaf: ?OnDeactivate@CModeBriarRoseTeleportInvulnerable@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E7850, 112 bytes (padding included); vtable 0x01280228 slot 6.
// Shape: CModeStandBase::OnDeactivate (0x00861D00) first; when the owner's state byte (+0x10) is 1
// it un-suspends its animation for this mode (CThingAICreature::SuspendAI(false, this),
// 0x00833A10); RemoveTC(0x93) (0x004C9840); the owner's graphic appearance (+0x64) gets
// SetAlpha(255) (0x004BFAC0); finally the CIntelligentPointer at +0x80 is tested with the
// IS_THING_ALIVE idiom (GetPItem 0x00A01B50 re-called for each use: non-null, dead bit 0 of +0x91
// clear) and the thing is killed (CThing::Kill(true), 0x004C9B80).
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAlpha(unsigned char alpha);     // 0x004BFAC0
};
}
using NGraphicAppearance::CTCGraphicAppearance;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
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

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CModeStandBase {
    virtual void v00();
    CThing* ParentThing;                    // +4
    unsigned char _body[0x60];
    virtual void OnDeactivate();            // 0x00861D00
};

namespace NCreatureMode {

struct CModeBriarRoseTeleportInvulnerable : CModeStandBase {
    unsigned char _pad68[0x18];             // +0x68 filter etc.
    CIntelligentPointer<CThing> m_pHelper;      // +0x80
    virtual void OnDeactivate();
};

void CModeBriarRoseTeleportInvulnerable::OnDeactivate()
{
    CModeStandBase::OnDeactivate();
    if (ParentThing->GetType() == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(false, this);
    ParentThing->RemoveTC((ETCInterfaceType)(0x93));
    ParentThing->GetTCGraphicAppearance()->SetAlpha(0xff);
    if (IS_THING_ALIVE(m_pHelper))
        m_pHelper.GetPItem()->Kill(true);
}

}