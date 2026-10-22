// CCreatureAction_ShowTrophy::PerformAction() -- while the shown trophy is alive: kill a live emitter, respawn the
// "SHOW_TROPHY_HELD_ALOFT" effect at the creature's WEAPON_FOCUS_01 dummy as PEmitter (attached to the right hand), then tell the
// trophy's CTCTrophy (interface 0x39) that the creature showed it
// (the effect lookup is the Initialise 0x00845350 shape)
// Leaf: ?PerformAction@CCreatureAction_ShowTrophy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008455C0, length 641 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10,
//   CVectorMap::LowerBound 0x0040F020, CTCTrophy::OnShownByCreature 0x006DE8C0 (descriptive name), CThing::Kill(bool) 0x004C9B80,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30,
//   CThing::GetPosition 0x004C73D0, CTCGraphicAppearance::GetDummyObject 0x004C08C0, CreateEffectThing 0x006E0880,
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, g_RightHandAttachmentName 0x013BAC30,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Layout: Ego_r.pdb PTrophyBeingShown +0xb4, PEmitter +0xc1 (packed); retail CCreatureActionBase is 0xa8, so the members
//   sit 12 bytes lower (PEmitter at the odd offset +0xb5; the landed FinishAction 0x00845850).
// Shape: GetSubDef / GetEffectFromString get same-TU stand-in bodies; operator= names the item before SetPItem (this+0xb5 loads late); the && chain carries the by-value def copy and the
//   name temporary (conditional destruction flags 1/2); the sub def pointer lives to the end of the function.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern CCharString g_RightHandAttachmentName;          // 0x013BAC30
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Find_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CSpecialEffectsDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CThing;
struct CTCBase;
struct CTCTrophy {
    void OnShownByCreature(CThing* creature);   // 0x006DE8C0
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* GraphicAppearance;    // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool silent);                     // 0x004C9B80
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
    bool GetDummyObject(const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
    {
        return GraphicAppearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
    }
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
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentThingPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentThingPointer() : CBaseIntelligentPointer() { }
    __forceinline CIntelligentThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentThingPointer& operator=(const CIntelligentThingPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CCreatureActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_ShowTrophy : public CCreatureActionBase {
public:
    CIntelligentThingPointer PTrophyBeingShown; // +0xa8 (PDB +0xb4)
    unsigned long            ShowingTrophyDeedHandle;   // +0xb0
    bool                     PostedDeed;        // +0xb4
    CIntelligentThingPointer PEmitter;          // +0xb5 (packed)
    virtual void PerformAction();
    __forceinline CTCTrophy* GetTrophy() const
    {
        CTCTrophy* trophy;
        PTrophyBeingShown.GetPItem()->GetInterface(&trophy, 0x39);
        return trophy;
    }
};
#pragma pack(pop)

void CCreatureAction_ShowTrophy::PerformAction()
{
    if (PTrophyBeingShown.GetPItem() && !(PTrophyBeingShown.GetPItem()->m_flags91 & 1)) {
        if (PEmitter.GetPItem() && !(PEmitter.GetPItem()->m_flags91 & 1))
            PEmitter.GetPItem()->Kill(true);
        CDefPointer<CSpecialEffectsDef> effects;
        long effectType = 0;
        if (GetCreature() && GetCreature()->GetDef()->GetSubDef(&effects)
            && effects->GetEffectFromString("SHOW_TROPHY_HELD_ALOFT", effectType)) {
            C3DVector position = GetCreature()->GetPosition();
            CRightHandedSet orientation;
            GetCreature()->GetDummyObject("WEAPON_FOCUS_01", &position, &orientation);
            CThing* effect = CreateEffectThing(effectType, &position, false);
            if (effect) {
                PEmitter = CIntelligentThingPointer(effect);
                CTCDParticleEmitter* emitter;
                if (PEmitter.GetPItem()->GetInterface(&emitter, 0x4b))
                    emitter->AttachToThing(GetCreature(), 10, g_RightHandAttachmentName, 0, 0.0f);
            }
        }
        GetTrophy()->OnShownByCreature(GetCreature());
    }
}
