// NCreatureMode::CModeGuildmasterPowerUpSpeed::OnDeactivate() -- kill every live lightning-beam effect, resume the guildmaster's AI (undo OnActivate's SuspendAI) and run the stand-mode OnDeactivate
// Leaf: ?OnDeactivate@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UAEXXZ
// Layout: engine/CClass.h is a generated flat record (pack(1), vptr as data, sizeof 0x11): the polymorphic CClass is local.
// Retail: Fable.exe (TLC, VC7.1) -- Shield @ 0x008D7A90 and Speed @ 0x008D7B20, 135 bytes each, thiscall, byte-identical but for
//   the rel32 call displacements; tail `jmp ?OnDeactivate@CModeStandBase@@UAEXXZ` (0x00861D00).
// Identity: each row is the only reference from slot 6 of its own vtable -- 0x008D7A90 at 0x01268608 = vtable 0x012685F0 + 0x18
//   (RTTI NCreatureMode::CModeGuildmasterPowerUpShield), 0x008D7B20 at 0x0126863C = vtable 0x01268624 + 0x18 (RTTI
//   NCreatureMode::CModeGuildmasterPowerUpSpeed). Slot 6 is OnDeactivate in the mode family (the landed sibling
//   CModeGuildmasterPowerUpHeal::OnDeactivate 0x008D69A0 is slot 6 of 0x012685BC and ends in the same SuspendAI(false) + base
//   tail; slot 5 of the Shield vtable is the landed OnActivate 0x008E0060). Ego_r.pdb ?OnDeactivate@CModeGuildmasterPowerUpShield@
//   NCreatureMode@@UAEXXZ (donor 0x00818E40) and ..PowerUpSpeed.. (donor 0x00818ED0): 0x90 apart in the donor as in retail.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50 (Ego_r / FableWin), ?Kill@CThing@@QAEX_N@Z
//   0x004C9B80, ?SuspendAI@CThingAICreature@@QAEX_NPAX@Z 0x00833A10, ?OnDeactivate@CModeStandBase@@UAEXXZ 0x00861D00.
// Layout: Ego_r.pdb CModeGuildmasterPowerUpShield / Speed (0x98): Filter +0x68, Timer +0x74, LightningBeamParticleEmitters
//   (vector<CIntelligentPointer<CThing> >) +0x78, Guards (vector<CIntelligentPointer<CThingAICreature> >) +0x88 donor = +0x84
//   retail (STLport vector 12 bytes); CModeBase::ParentThing +0x04; CClass::Type +0x10; the CThing dead bit +0x91 bit 0 (as the
//   landed OnActivate 0x008E0060). engine/CModeStandBase.h / CClass.h are flattened (vptr as data), so the chain is local.
// Shape: `long count = LightningBeamParticleEmitters.size(); for (long i = 0; i < count; ++i) if (IS_THING_ALIVE(v[i]))
//   v[i]->Kill(true);` -- signed size (`sar ebp,3` then `jle`/`jl`), i lives in the `push ecx` slot, each element access
//   re-reads _M_start (`*(begin() + n)`), IS_THING_ALIVE (Ego_r ??$IS_THING_ALIVE@V?$CIntelligentPointer@..@@@@YI_NABV..@Z,
//   a __fastcall template) is the inline pointer test + dead-bit test; then the Heal sibling's SuspendAI(false, this) under
//   `ParentThing->GetType() == 1` and the qualified base call as a tail jump.
// TU: retail compiled both modes in one file (adjacent rows); this source holds both bodies, the leaf picks the row.
class CBaseObject;
class CClass { public:
    virtual void v00();
    unsigned long GetType() const { return Type; }
    unsigned char _base_0x04[0xc];
    unsigned char Type;                         // +0x10
};
class CThing : public CClass { public:
    unsigned char _pad14[0x91 - 0x14];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool b);                          // 0x004C9B80
};
class CThingAICreature : public CThing { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10
};
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();
    void* PData;                                // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* operator->() const { return (T*)GetPItem(); }
    operator T*() const { return (T*)GetPItem(); }
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !(thing->m_flags91 & 1);
}
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp& reference;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
};
}
class CAnimFilterOfGroup { public: unsigned char _body[0xc]; };
class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual ~CModeBase();
        virtual void v01();
        virtual void v02();
        virtual void v03();
        virtual void v04();
        virtual void OnActivate();
        virtual void OnDeactivate();
        CThing* ParentThing;                    // +0x04
        unsigned char _pad08[0x14];
    };
};
class CModeStandBase : public CTCCreatureModeManager::CModeBase { public:
    unsigned char _pad1c[0x68 - 0x1c];
    virtual void OnActivate();                  // 0x008621A0
    virtual void OnDeactivate();                // 0x00861D00
};
namespace NCreatureMode {
class CModeGuildmasterPowerUpShield : public CModeStandBase { public:
    CAnimFilterOfGroup Filter;                  // +0x68
    long Timer;                                 // +0x74
    std::vector<CIntelligentPointer<CThing> > LightningBeamParticleEmitters;   // +0x78
    std::vector<CIntelligentPointer<CThingAICreature> > Guards;                // +0x84
    virtual void OnActivate();
    virtual void OnDeactivate();
};
void CModeGuildmasterPowerUpShield::OnDeactivate()
{
    long count = LightningBeamParticleEmitters.size();
    for (long i = 0; i < count; ++i) {
        if (IS_THING_ALIVE(LightningBeamParticleEmitters[i]))
            LightningBeamParticleEmitters[i]->Kill(true);
    }
    if (ParentThing->GetType() == 1)
        static_cast<CThingAICreature*>(ParentThing)->SuspendAI(false, this);
    CModeStandBase::OnDeactivate();
}
class CModeGuildmasterPowerUpSpeed : public CModeStandBase { public:
    CAnimFilterOfGroup Filter;                  // +0x68
    long Timer;                                 // +0x74
    std::vector<CIntelligentPointer<CThing> > LightningBeamParticleEmitters;   // +0x78
    std::vector<CIntelligentPointer<CThingAICreature> > Guards;                // +0x84
    virtual void OnActivate();
    virtual void OnDeactivate();
};
void CModeGuildmasterPowerUpSpeed::OnDeactivate()
{
    long count = LightningBeamParticleEmitters.size();
    for (long i = 0; i < count; ++i) {
        if (IS_THING_ALIVE(LightningBeamParticleEmitters[i]))
            LightningBeamParticleEmitters[i]->Kill(true);
    }
    if (ParentThing->GetType() == 1)
        static_cast<CThingAICreature*>(ParentThing)->SuspendAI(false, this);
    CModeStandBase::OnDeactivate();
}
}