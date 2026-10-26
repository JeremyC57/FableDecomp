// CCreatureAction_JackFloatUp::Initialise() -- names the animation "RISE_TO_FLOAT", runs the base Initialise, clears Moving and FloatSpeed and records the creature's current height as StartHeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0730, length 96 bytes. Vtable slot 12.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PCreature at +8), CThing::GetPosition @ 0x004C73D0 (`[this+0x60] + 0xc`: the physics component's position
// vector; manifest label "CVertexBufferWin32::DoSizeof" is propagated nonsense).
// Shape: the landed Initialise family, then two zero stores through the shared zero register (Moving first, so FloatSpeed
// is written as an integer zero: a float literal would be an immediate) and StartHeight copied from the vector's Z (+8)
// with a register move. Member names from the engine header (PDB +0x12c..; retail CCombatActionBase is 0x114 bytes).
struct CPosition {                          // the physics component's position vector (X, Y, Z)
    long X;
    long Y;
    long Z;                                 // +0x08 (float in the engine; copied as a dword)
};

class CThingCreatureBase_Calls { public:           // engine/CThingCreatureBase.h exists but is not used: only this call is modelled
    const CPosition& GetPosition() const;   // 0x004C73D0
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

#include "fable_string.h"

class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThingCreatureBase_Calls> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_JackFloatUp : public CCombatActionBase { public:   // vtable 0x01280214
    long          FloatHeight;              // +0x114 (PDB +0x12c, float; untouched here)
    long          FloatSpeed;               // +0x118 (PDB +0x130, float; cleared through the zero register)
    unsigned char Moving;                   // +0x11c (PDB +0x134, bool)
    long          StartHeight;              // +0x120 (PDB +0x138, float; copied as a dword)
    virtual void Initialise();
};

void CCreatureAction_JackFloatUp::Initialise()
{
    {
        CCharString name("RISE_TO_FLOAT", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    Moving = 0;
    FloatSpeed = 0;
    StartHeight = PCreature.GetPItem()->GetPosition().Z;
}