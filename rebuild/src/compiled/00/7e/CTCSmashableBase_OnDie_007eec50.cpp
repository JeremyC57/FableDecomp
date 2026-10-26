// CTCSmashableBase::OnDie() -- when the smashable def says the thing is smashable and it has not been smashed yet, smash it (virtual Smash)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EEC50, 53 bytes. Vtable slot 13 of CTCSmashableBarrier 0x0124017C (shared by
// CTCSmashableWindow 0x01240BA4 and the base vtable 0x01271860: the inherited base method).
// Callees: the def's virtual Destroy (slot 1) on the last release; this->Smash() (slot 28, +0x70) as a sibling-call `jmp`.
// Layout: Ego_r.pdb CTCSmashableBase (0x18 bytes, as the landed OnInitialActivate 0x007EECF0): Smashed +0x10, Definition +0x14;
// CSmashableDef (engine/CSmashableDef.h): Smashable +0x25. Local model: the def pointer needs its add-ref/release shape.
// Shape: the landed OnInitialActivate shape -- the flag read into a named bool (bl) before the by-value def temporary is released.
class C3DVector { public: float X, Y, Z; };

class CThing {
public:
    const C3DVector& GetPosition() const;                // 0x004C73D0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

class CSmashableDef { public:
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long          m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
    unsigned char _pad08[0x25 - 0x08];
    bool          Smashable;                             // +0x25
    unsigned char _pad26[2];
    long          ReplacementObject;                     // +0x28 CDefIndex
    long          SmashParticleEmitter;                  // +0x2c
};

struct CDefPointer {
    CSmashableDef* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    ~CDefPointer() { m_pDef->Release(); }
    CSmashableDef* operator->() const { return m_pDef; }
};

class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual void Smash();                                // slot 28 (+0x70)
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
};

class CTCSmashableBase : public CTCBase {
public:
    long        LastWorldFrameHit;                       // +0x0c
    bool        Smashed;                                 // +0x10
    CDefPointer Definition;                              // +0x14
    CDefPointer GetDef() const { return Definition; }
    void OnDie();
};

void CTCSmashableBase::OnDie()
{
    bool smashable = GetDef()->Smashable;
    if (smashable && !Smashed)
        Smash();
}