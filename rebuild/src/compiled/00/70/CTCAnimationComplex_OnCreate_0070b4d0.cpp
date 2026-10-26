// CTCAnimationComplex::OnCreate() -- reset the thing's animator and start the appearance's first "DEFAULT" animation as the main anim (type 6, mode 1, not looping, percent 100)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B4D0, length 216 bytes (+ int3 padding).
// Callees: the animator reset @ 0x0070EC30 (CAnimateThing, name unproven; manifest "Navigator_UpdateBuildingTreeUI" is
// propagated), CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (byte-twin label GetSubDef<CAbilityDef>),
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CAnimationSet::GetAnimation(const CCharString&,
// long, const CAnimFilterBase&) @ 0x005DC340, CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim(const CAnimInit&, long,
// long) @ 0x0070D580, the defs' virtual Destroy (slot 1).
// Identity: the filter temporary is CAnimFilterNoGroup (vtable 0x0122FFDC), destroyed through the CAnimFilterBase store 0x0122FFCC.
// Layout: Ego_r.pdb CTCAnimationComplex PAnimation +0x0C; retail CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the thing's base def is fetched through a by-value CDefPointer temporary read through .Object (the add-ref and the
// release share one null test, both count stores kept) and the raw pointer then asks for the appearance sub-def; the
// "DEFAULT" lookup is the 0x007F8F30 PlayDefaultAnimation block (def read after the string is built, kept in esi for the
// release at the end).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
class CAnimationEntry { public:
    long AnimIndex;                             // +0x00
};
class CAnimFilterBase { public:                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
class CAnimationSet { public:
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};

class CAnimInit;
class CAnimateThing { public:
    void ResetAnimations_0070ec30();            // 0x0070EC30 (descriptive)
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
class CAnimInit { public:                              // 0x2c bytes
    long          Mode;                         // +0x00
    unsigned char AnimType;                     // +0x04
    bool          Flag05;                       // +0x05
    long          AnimIndex;                    // +0x08
    float         Scales[3];                    // +0x0c
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;                        // +0x20
    long          Arg24;                        // +0x24
    long          Percent;                      // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CCountedDefPointer {  // the by-value CDefPointer<T> copy: add-ref on copy, release on destruction
    T* Object;
    CCountedDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CCountedDefPointer() { if (Object) Object->Release(); }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
class CThing { public:
    unsigned char _pad00[0x70];
    CThingBaseDef* PBaseDef;                    // +0x70
    CCountedDefPointer<CThingBaseDef> GetBaseDef() const { return CCountedDefPointer<CThingBaseDef>(PBaseDef); }
};

class CTCAnimationComplex {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CAnimateThing* PAnimation;                  // +0x0c
    void OnCreate();
};

void CTCAnimationComplex::OnCreate()
{
    CAnimateThing* animation = PAnimation;
    animation->ResetAnimations_0070ec30();
    const CThingBaseDef* baseDef = ParentThing->GetBaseDef().Object;
    CDefPointer<CAppearanceDef> out;
    baseDef->GetSubDef(&out);
    CAppearanceDef* def;
    const CAnimationEntry* entry;
    {
        CCharString name("DEFAULT");
        def = out.Object;
        entry = def->Animation.GetAnimation(name, 0, CAnimFilterNoGroup());
    }
    CAnimInit init(6, 0, entry->AnimIndex, 1, false, 0, 100);
    animation->SetMainAnim(init, 1, 0);
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}