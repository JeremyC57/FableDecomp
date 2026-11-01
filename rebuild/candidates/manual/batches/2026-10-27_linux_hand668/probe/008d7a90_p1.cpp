// NCreatureMode::CModeGuildmasterPowerUpShield::OnDeactivate() -- kill the beam effects, resume AI, stand-mode OnDeactivate
// Leaf: ?OnDeactivate@CModeGuildmasterPowerUpShield@NCreatureMode@@UAEXXZ
class CBaseObject;
class CClass { public:
    virtual void v00();
    unsigned long GetType() const { return Type; }
    unsigned char _base_0x04[0xc];
    unsigned char Type;                         // +0x10
};
class CThing : public CClass { public:
    unsigned char _pad11[0x91 - 0x11];
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
}
