// Grade: DIFFER(337v337) -- frame-slot packing only: retail keeps the LIGHTNING_HAND_EFFECT string alone at L+4 and overlays the effect type, the 8-byte CIntelligentPointer temp, the GetInterface key and the bone-name string at L+8; ours overlays the temps on the first string (L+4) and gives the effect type L+0xc (25 forms swept; same packing as 0x008E7A00)
// NCreatureMode::CModeThunderLightningStorm::CreateHandGlow() -- spawn Thunder's LIGHTNING_HAND_EFFECT at his position, keep it in the mode's hand-glow pointer and attach its particle emitter to his WEAPON_FOCUS_01 bone
// Leaf: CreateHandGlow
// Callees: GetSubDef 0x00669580 (same-TU stand-in body), CCharString 0x0099EBF0/~ 0x0099EAE0, GetEffectFromString 0x00669D30, GetPosition 0x004C73D0, CreateEffectThing 0x006E0880, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10, LowerBound 0x0040F020, CTCDParticleEmitter::AttachToThing 0x006E0BE0.
struct CTCBase;
struct CBaseObject;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CSpecialEffectsDef : CDefPointeeBase {
    bool GetEffectFromString(const CCharString& name, long* type);   // 0x00669D30
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x45];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
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
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CThing* GetPItem() const;              // 0x00A01B50
    void SetPItem(CThing* item);           // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
};
struct CModeStandBase : CModeBase {
    unsigned char _body[0x4c];
};
namespace NCreatureMode {
struct CModeThunderLightningStorm : CModeStandBase {
    unsigned char _pad68[0x34];
    CIntelligentPointerThing PHandGlow;         // +0x9c
    CDefClassBase* PDef;                        // +0xa4
    void CreateHandGlow();
};
void CModeThunderLightningStorm::CreateHandGlow()
{
    CDefPointer<CSpecialEffectsDef> out;
    long effectType = 0;
    PDef->GetSubDef(&out);
    CDefHolder<CSpecialEffectsDef> effects(out.Object);
    if (effects->GetEffectFromString("LIGHTNING_HAND_EFFECT", &effectType)) {
        C3DVector position = ParentThing->GetPosition();
        CThing* glow = CreateEffectThing(effectType, &position, false);
        if (glow && !glow->IsDead()) {
            PHandGlow = CIntelligentPointerThing(glow);
            CTCDParticleEmitter* emitter;
            if (glow->GetInterface(&emitter, 0x4b) && emitter)
                emitter->AttachToThing(ParentThing, 10, "WEAPON_FOCUS_01", 0, 0.0f);
        }
    }
}
}
