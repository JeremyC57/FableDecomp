// NCreatureAction::CActionEventCameraShake::Dispatch(const CActionEventContext&) -- starts a camera shake at the context thing's (bone) position unless the thing is gone or flagged dead
// Leaf: ?Dispatch@CActionEventCameraShake@@QAEXABUCActionEventContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00871250, length 59 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on the intelligent pointer at +8 of the
// context), CThing::GetPos @ 0x004C73D0 (Ego_r.pdb; the "CVertexBufferWin32::DoSizeof" label is propagated),
// CreateCameraShake @ 0x007F1B70 (__fastcall(position, strength, bone, flag): `ret 8`, so the bone and flag pushes are
// its two stack parameters, evaluated before the register pair; labelled "Create").
// Shape: the thing is fetched, null-tested and its one-bit flag at +0x91 tested (a branch on a
// bitfield); the byte member at +0x1c is zero-extended (`xor ecx,ecx ; mov cl`) for an unsigned char
// parameter and pushed first (last argument), the long at +0x18 second; the position result feeds
// the __fastcall creator with the strength at +0x14 in edx.
class C3DVector;

#include "engine/CThing.h"

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

struct CActionEventContext {
    unsigned char       _pad[0x8];
    CIntelligentPointer<CThing> m_pThing;                       // +0x08
};

enum EQuakeStrength { };                                  // agentNF19: CTCDRumble quake enums (values unused here)
enum EQuakeLength { };
class CTCDRumble { public:
    static CThing* __fastcall Create(const C3DVector& position, EQuakeStrength strength, EQuakeLength length, bool flag);   // 0x007F1B70 ?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@W4EQuakeStrength@@W4EQuakeLength@@_N@Z (Ego_r 0x007EB500)
};

struct CActionEventCameraShake {
    virtual void v00();
    unsigned char _pad[0x14 - 0x04];
    long          m_strength;                           // +0x14
    long          m_bone;                               // +0x18
    bool          m_boneFlag;                           // +0x1c
    void Dispatch(const CActionEventContext& context);
};

void CActionEventCameraShake::Dispatch(const CActionEventContext& context)
{
    CThing* thing = const_cast<CIntelligentPointer<CThing>&>(context.m_pThing).GetPItem();
    if (thing && !thing->IsDead())
        CTCDRumble::Create(thing->GetPos(), (EQuakeStrength)m_strength, (EQuakeLength)m_bone, m_boneFlag);
}