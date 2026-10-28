// CCreatureAction_KillCreature::PerformAction() -- if the target creature is alive and within 2 metres of this creature, kill it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008445A0, length 96 bytes. Vtable slot 3 (PerformAction).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTargetCreature at +0xa8 three times -- the landed IS_THING_ALIVE
// idiom re-fetches, then the named local -- and PCreature at +8), CThing::GetPosition @ 0x006A4D00 (`[this+0x60]+0xc`;
// manifest label propagated), CThingPhysical::IsInRange(const C3DVector&, float) @ 0x006A4DF0, CThing::Kill(bool) @ 0x004C9B80.
// Shape: `this` rides in edi, the target pointer member in esi (reused for the target itself); the creature is a named local
// (fetched before the 2.0f immediate 0x40000000 is pushed; an inline argument expression would push first). Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so
// PTargetCreature (engine-header name, PDB +0xb4) is at +0xa8 and `this` is modelled locally.
class C3DVector;

class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    const C3DVector& PeekPos() const;   // 0x006A4D00
};
class CThing : public CThingPhysical { public:                             // engine/CThing.h exists but is not used: only the dead bit and the calls are modelled
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    bool IsInRange(const C3DVector& position, float range);   // 0x006A4DF0
    void Kill(bool silent);                 // 0x004C9B80
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCreatureAction_KillCreature {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CIntelligentPointer<CThing> PTargetCreature;   // +0xa8 CIntelligentPointer<CThingCreatureBase> (PDB +0xb4)
};

void CCreatureAction_KillCreature::PerformAction()
{
    if (IS_THING_ALIVE(PTargetCreature)) {
        CThing* pTarget = PTargetCreature.GetPItem();
        CThing* pCreature = PCreature.GetPItem();   // named: fetched before the 2.0f immediate is pushed
        if (pTarget->IsInRange(pCreature->PeekPos(), 2.0f))
            pTarget->Kill(true);
    }
}