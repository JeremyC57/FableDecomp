// CTCActivationReceptorBase::OnKill() -- tell every trigger in the Triggers vector that this receptor died: std::for_each(Triggers, CInformTriggerOfReceptorDeath(this))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F7850, length 20 bytes.
// Leaf: ?OnKill@CTCActivationReceptorBase
// Callees: std::_For_each<CThing**, CInformTriggerOfReceptorDeath> @ 0x007F7B70 (`__fastcall`: the functor result by
// hidden pointer in ecx, `first` in edx, `last` and the by-value functor on the stack; manifest-named). Ego_r.pdb has
// `?OnKill@CTCActivationReceptorBase@@UAEXXZ`; the chunk's "CTCActivationReceptorCreatureGenerator::vfunc_14" is the
// inherited slot (the generator does not override OnKill). Triggers is the retail 12-byte vector at +0x10 (engine header
// not used: PDB 16-byte vector, see 0x007F7710).
// Shape: the `push ecx` frame word receives the returned functor; `this` is pushed as the 4-byte functor (a class with a
// user constructor but an implicit copy, so a plain dword); `last` is loaded before `first` (right-to-left arguments).
struct CThing;
struct CTCActivationReceptorBase;

struct CInformTriggerOfReceptorDeath {         // 4-byte functor
    CTCActivationReceptorBase* m_pReceptor;
    CInformTriggerOfReceptorDeath(CTCActivationReceptorBase* receptor) : m_pReceptor(receptor) { }
    void operator()(CThing* trigger);
};

CInformTriggerOfReceptorDeath __fastcall _For_each(CThing** first, CThing** last, CInformTriggerOfReceptorDeath func);   // 0x007F7B70
template<class It, class Fn> inline Fn for_each(It first, It last, Fn func) { return _For_each(first, last, func); }

struct CThingPtrVector {                       // retail std::vector<CThing*> (12 bytes)
    CThing** m_first;
    CThing** m_last;
    CThing** m_end;
    CThing** begin() { return m_first; }
    CThing** end() { return m_last; }
};

struct CTCActivationReceptorBase {             // retail layout (see 0x007F7710)
    virtual void v00();
    unsigned char   _body04[8];                // +0x04 CTCBase
    bool            Activated;                 // +0x0c
    CThingPtrVector Triggers;                  // +0x10
    virtual void OnKill();
};

void CTCActivationReceptorBase::OnKill()
{
    for_each(Triggers.begin(), Triggers.end(), CInformTriggerOfReceptorDeath(this));
}
