// CTCAnimationComplex::~CTCAnimationComplex() -- delete the owned animate-thing, then the CTCBase base destructor
// Leaf: ??1CTCAnimationComplex@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B5B0, 42 bytes, thiscall, tail `jmp ~CTCBase`.
// Identity (proven): stores vtable 0x012650A4 (RTTI .?AVCTCAnimationComplex@@, rtti-vtables.tsv) first; Ego_r.pdb
//   ??1CTCAnimationComplex@@UAE@XZ sits at 0x0057A580 between OnCreate 0x0057A4A0 (0xE0 bytes) and GetClassTypeName
//   0x0057A5B0, and retail's OnCreate is vtable slot 4 = 0x0070B4D0 (+0xE0 = this row) with slot 22 at 0x0070B5E0 (+0x30, the
//   donor's dtor size). No callers: the slot-0 `??_G` 0x0070BB80 inlines this body; the out-of-line `??1` is the virtual
//   destructor's own COMDAT. The catalog label "~CAInputProcess" is propagated (the real one is 0x00A0D3D0).
// Callees: CAnimateThing::~CAnimateThing 0x0070BA90 (non-polymorphic, so `delete` is dtor + operator delete),
//   operator delete 0x00BFE9BC, CTCBase::~CTCBase 0x00686830.
// Shape: `if (PAnimation) delete PAnimation;` -- the pointer is loaded before the vtable store (scheduling); no zero store.
// Layout: CTCBase (vptr +0, ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9), PAnimation +0x0C (Ego_r.pdb, sizeof 0x10).
//   Local CTCBase model: engine/CTCBase.h flattens the vptr into a data member, so it cannot carry the virtual destructor.
void __cdecl operator delete(void* block);     // 0x00BFE9BC

class CThing;

class CAnimateThing {
public:
    ~CAnimateThing();                           // 0x0070BA90
};

class CTCBase {
public:
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};

class CTCAnimationComplex : public CTCBase {   // vtable 0x012650A4
public:
    CAnimateThing* PAnimation;                  // +0x0C
    virtual ~CTCAnimationComplex();
};

CTCAnimationComplex::~CTCAnimationComplex()
{
    if (PAnimation)
        delete PAnimation;
}