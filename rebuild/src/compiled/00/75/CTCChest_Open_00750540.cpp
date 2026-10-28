// CTCChest::Open() -- open a shut chest: mark it open, force frame updates, persist it when the def asks, spawn and attach the def's open particle effect, then stop the hero pushing it
// Leaf: Open
// Retail 0x00750540, 384 bytes, `ret`.
// Callees: CWideString() 0x0099AED0 / ~CWideString 0x0099B510 (an unused local), CDefClassBase::GetSubDef<CChestDef>
// 0x0074F7D0 (same TU: a stand-in body is given), CThing::SetForceFrameUpdate(bool) 0x004C7C60,
// CThing::SetAsGameStatePersisted(bool) 0x004C7880, CThing::GetPos 0x004C73D0, CreateEffectThing(long, const C3DVector*,
// bool) 0x006E0880 (`__fastcall`), CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/
// 0x00A01C10 (vtable 0x01238C6C), CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E) / ~ 0x0099EAE0,
// CTCDParticleEmitter::AttachToThing 0x006E0BE0 / SetAlwaysUpdate(bool) 0x006E0590 / SetEmitterOrientation 0x006E0480,
// the thing physics' slot 72 (+0x120, the orientation), CTCChest::SetPushableByHero(bool) 0x007504E0; the def's slot 1.
// Layout: Ego_r.pdb CTCChest - 4 (ChestOpener COpenCloseHelper flags +0x1C..+0x1F, ChestOpen +0x30, POpenParticle +0x31,
// packed); CChestDef PersistOnOpening +0x25, OpenParticleEffect +0x28; CThing physics +0x60, def +0x70, PTCDriver +0x5C
// (interface 0x4B), flag byte +0x92, dead flag +0x91.
// Shape: the chest def's out CDefPointer (zeroed after the thing def is read) is released by its inline dtor, register-held
// thanks to the same-TU GetSubDef stand-in, which also frees its slot for the intelligent-pointer and "" temporaries; the
// emitter is `T* p = 0; if (HasTC) p = PTCDriver;` on one GetPItem result (landed CTCResurrectionItem::OnItemUsed 0x007F04C0
// model); the opener flags are set through an inline COpenCloseHelper member (written directly they sink below the
// SetForceFrameUpdate argument push); IS_THING_ALIVE is `__forceinline`.
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefPointeeBase { public:
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
class CChestDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x25 - 0x08];
    bool          PersistOnOpening;                     // +0x25
    long          OpenParticleEffect;                   // +0x28
};
class CDefClassBase : public CDefPointeeBase { public:
    CDefPointeeBase* FindSubDef();
    bool GetSubDef(CDefPointer<CChestDef>* out);        // 0x0074F7D0
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CChestDef> (retail instantiates it in this TU).
bool CDefClassBase::GetSubDef(CDefPointer<CChestDef>* out)
{
    CDefPointeeBase* p = FindSubDef();
    out->Object = (CChestDef*)p;
    if (p)
        ++p->m_refCount;
    return p != 0;
}
#pragma auto_inline(on)
class CTCDParticleEmitter;
struct CPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual CRightHandedSet* GetOrientation();          // slot 72 (+0x120)
};
class CThing {
public:
    unsigned char        _pad00[0x20];
    unsigned long        m_presentBits[9];              // +0x20
    unsigned char        _pad44[0x5c - 0x44];
    CTCDParticleEmitter* PTCDriver;                     // +0x5c (the cached interface-0x4B component)
    CPhysics*            Physics;                       // +0x60
    unsigned char        _pad64[0x70 - 0x64];
    CDefClassBase*       PDef;                          // +0x70
    unsigned char        _pad74[0x91 - 0x74];
    unsigned char        m_flags91;                     // +0x91 bit 0: dead
    unsigned char        m_flags92;                     // +0x92 bit 4: persisted
    const C3DVector& GetPos() const;               // 0x004C73D0
    void SetForceFrameUpdate(bool force);               // 0x004C7C60
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    void SetAlwaysUpdate(bool always);                  // 0x006E0590
    void SetEmitterOrientation(CRightHandedSet* orientation);   // 0x006E0480
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct COpenCloseHelper {
    bool Opening;                                       // +0x00
    bool Closing;                                       // +0x01
    bool Opened;                                        // +0x02
    bool Closed;                                        // +0x03
    void SetOpened() { Opening = false; Closing = false; Opened = true; Closed = false; }
};
class CTCChest {
public:
    void*                       __vftable;              // +0x00
    CThing*                     ParentThing;            // +0x04
    unsigned char               _pad08[0x1c - 0x08];
    COpenCloseHelper            ChestOpener;            // +0x1c
    unsigned char               _pad20[0x30 - 0x20];
    bool                        ChestOpen;              // +0x30
    CIntelligentPointer<CThing> POpenParticle;          // +0x31
    void SetPushableByHero(bool pushable);              // 0x007504E0
    void Open();
};
#pragma pack(pop)

void CTCChest::Open()
{
    if (ChestOpen)
        return;
    CWideString unused;
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefPointer<CChestDef> def;
    if (thingDef->GetSubDef(&def)) {
        ChestOpen = true;
        ChestOpener.SetOpened();
        ParentThing->SetForceFrameUpdate(true);
        if (def->PersistOnOpening) {
            ParentThing->m_flags92 |= 0x10;
            ParentThing->SetAsGameStatePersisted(true);
        }
        if (def->OpenParticleEffect > 0) {
            POpenParticle = CIntelligentPointer<CThing>(CreateEffectThing(def->OpenParticleEffect, &ParentThing->GetPos(), false));
            if (IS_THING_ALIVE(POpenParticle)) {
                CTCDParticleEmitter* emitter = 0;
                CThing* effect = POpenParticle.GetPItem();
                if (effect->HasTC(0x4b))
                    emitter = effect->PTCDriver;
                emitter->AttachToThing(ParentThing, 1, "", 0, 0.0f);
                emitter->SetAlwaysUpdate(true);
                emitter->SetEmitterOrientation(ParentThing->Physics->GetOrientation());
            }
        }
        SetPushableByHero(false);
    }
}