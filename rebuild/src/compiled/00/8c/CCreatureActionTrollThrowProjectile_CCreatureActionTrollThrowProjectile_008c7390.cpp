// CCreatureAction_TrollThrowProjectile::CCreatureAction_TrollThrowProjectile(CThingCreatureBase*, CThing*, const C3DVector&) -- base CCombatActionBase(owner, thing, 150, zeroed CSoundPair), own vtable, an empty projectile pointer, then the target position with the position flag set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C7390, length 112 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10 (the member at +0x114, then the inline CIntelligentPointer vtable 0x01238C6C). Own vtable 0x01281694 (rtti-vtables.tsv).
// Shape: `ret 0Ch`; the sound pair zeroed from eax in the 8-byte frame; TargetPos and the flag are BODY assignments: the whole-vector assignment
// re-bases the destination (`lea eax,[esi+11Ch]`) and the flag byte is stored last (an init-list copy would go through C3DVector's memberwise
// copy constructor at fixed offsets, see the copy constructor 0x008C7D80).
// Leaf: ??0CCreatureAction_TrollThrowProjectile@@QAE@PAVCThingCreatureBase@@PAVCThing@@ABVC3DVector@@@Z
// The PDB header (CCreatureAction_TrollThrowProjectile.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
class CThingCreatureBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }                          // leaves the words unset
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise copy constructor (fixed-offset moves);
};                                                                                 // assignment stays implicit (re-based through `lea`)
class CThing { public:
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {         // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);  // 0x00857F30
    CCombatActionBase(const CCombatActionBase& other);                                                    // 0x0062F3B0
};
class CCreatureAction_TrollThrowProjectile : public CCombatActionBase { public:   // vtable 0x01281694; retail sizeof 0x130
    CIntelligentPointer<CThing> PProjectile;        // +0x114 CIntelligentPointer<CThing>
    C3DVector TargetPos;                    // +0x11c
    bool TargetIsPosition;                  // +0x128
    long ThrowFrame;                        // +0x12c
    virtual void v00();
    CCreatureAction_TrollThrowProjectile(CThingCreatureBase* owner, CThing* thing, const C3DVector& target);
};

CCreatureAction_TrollThrowProjectile::CCreatureAction_TrollThrowProjectile(CThingCreatureBase* owner, CThing* thing, const C3DVector& target)
    : CCombatActionBase(owner, thing, 150, CSoundPair())
{
    TargetPos = target;
    TargetIsPosition = true;
}