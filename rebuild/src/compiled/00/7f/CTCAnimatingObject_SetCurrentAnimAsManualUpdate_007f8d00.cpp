// CTCAnimatingObject::SetCurrentAnimAsManualUpdate() -- the thing's animation component's current main anim set gets update mode 0 (manual)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8D00, length 88 bytes.
// Leaf: SetCurrentAnimAsManualUpdate
// Callees: CVectorMap::LowerBound @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (`return *PAnimation`; the
// manifest's "CWorld::DrawGetEnvironment" is propagated). Interface 0x5a (CTCAnimationComplex) = word 2, bit 26 of
// the presence bitset (`test dword ptr [eax+28h],4000000h`). Ego_r.pdb CAnimSet: UpdateMode +0, CurrentPercentageComplete
// +0x24 / LastPercentageComplete +0x28 (retail +0x1c / +0x20 behind two 12-byte vectors); CTCAnimatingObject: Animating
// +0x0c, CurrentAnimationPriority +0x10.
// Shape: the landed out-parameter GetInterface template (0x007504E0 / 0x0069F750 family): the presence bitset word
// test, LowerBound on the +0x44 map with the key in the `push ecx` frame word, end-or-larger-key -> the end entry, the
// component left in eax. The out pointer is NOT initialised: on the absent path retail reads the uninitialised local
// (`mov eax,[esp+N]`) and still makes the call, which the same source reproduces.
struct CTCBaseInterface;
struct CInterfaceMap {                         // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];            // +0x20 std::bitset<274> UsingComponents
    CInterfaceMap m_interfaces;                // +0x44 Components
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

class CAnimInit;

class CAnimSet { public:                              // CAnimateThing::CurrentAnim.MainAnimSet (retail layout)
    long  UpdateMode;                          // +0x00 EAnimUpdateMode
    unsigned char _vectors[0x18];              // +0x04 Anims, InternalAlphas (12-byte retail vectors)
    float CurrentPercentageComplete;           // +0x1c
    float LastPercentageComplete;              // +0x20
};

class CAnimateThing { public:
    CAnimSet CurrentAnim;                      // +0x00 (the CCompleteAnimation's MainAnimSet)
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};

class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();             // 0x0070B460 (`mov eax,[ecx+0Ch]`, PAnimation)
};

class CTCAnimatingObject : public CTCBase { public:
    bool Animating;                            // +0x0c
    long CurrentAnimationPriority;             // +0x10
    void SetCurrentAnimAsManualUpdate();
    void SetFractionComplete(float fraction);
    void PlayAnimation(const CAnimInit& init, long priority);
};

void CTCAnimatingObject::SetCurrentAnimAsManualUpdate()
{
    CTCAnimationComplex* animation;
    ParentThing->GetInterface(&animation, 0x5a);
    animation->GetAnimation().CurrentAnim.UpdateMode = 0;
}