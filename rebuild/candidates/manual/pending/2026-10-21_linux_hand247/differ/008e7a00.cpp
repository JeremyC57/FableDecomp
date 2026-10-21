// Grade: DIFFER(391v391) -- frame-slot packing only: retail overlays the 8-byte CIntelligentPointer temp on the GetInterface key / effect-type slot (L+0xc), ours on the CCharString temp (L+8), and our effect-type zero store issues before the def load; registers match once the position is read through an inline GetParent() accessor (36 forms swept)
// NCreatureMode::CModeBriarRoseTeleportInvulnerable::OnActivate() -- Briar Rose charges a teleport: set the 1.25 s frame count, suspend a player creature's animation, advance the boss phase when ready, and spawn the TELEPORT_CHARGE effect at her position, keeping it in the mode's effect pointer
// Leaf: OnActivate
// Callees: CModeStandBase::OnActivate 0x008621A0, __ftol2 0x00BFEA70, SetSuspendAnimation 0x00833A10, LowerBound 0x0040F020, CTCBoss::IsReadyForPhaseChange 0x007AEE90 / IncrementBossPhase 0x007AED20, GetSubDef 0x00669580, CCharString 0x0099EBF0/~ 0x0099EAE0, GetEffectFromString 0x00669D30, GetPosition 0x004C73D0, CreateEffectThing 0x006E0880, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CTCBase;
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
template <class T> struct CDefCopy {
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefCopy()
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
struct CModeBase;
struct CThing {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                      // +0x10
    unsigned char  _pad11[0x0f];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x24];
    CDefClassBase* PDef;                        // +0x70
    unsigned char  _pad74[0x1d];
    unsigned char  m_flags91;                   // +0x91
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
    void SetSuspendAnimation(bool suspend, CModeBase* mode);   // 0x00833A10
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CTCBoss {
    bool IsReadyForPhaseChange();               // 0x007AEE90
    void IncrementBossPhase();                  // 0x007AED20
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
extern long g_GameFramesPerSecond;              // 0x01375550
struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
};
struct CModeStandBase : CModeBase {
    CThing* GetParent() const { return ParentThing; }
    unsigned char _body[0x4c];
    void OnActivate();                          // 0x008621A0
};
namespace NCreatureMode {
struct CModeBriarRoseTeleportInvulnerable : CModeStandBase {
    unsigned char _pad68[0x0c];
    long          FrameCount;                   // +0x74
    unsigned char _pad78[0x08];
    CIntelligentPointerThing PEffect;           // +0x80
    void OnActivate();
};
void CModeBriarRoseTeleportInvulnerable::OnActivate()
{
    CModeStandBase::OnActivate();
    FrameCount = (long)(g_GameFramesPerSecond * 1.25f);
    if (ParentThing->m_type == 1)
        ParentThing->SetSuspendAnimation(true, this);
    {
    CTCBoss* boss;
    if (ParentThing->GetInterface(&boss, 0xad) && boss->IsReadyForPhaseChange())
        boss->IncrementBossPhase();
    }
    {
    long effectType = 0;
    CDefPointer<CSpecialEffectsDef> out;
    bool found = CDefCopy<CDefClassBase>(ParentThing->PDef)->GetSubDef(&out)
        && out.Object->GetEffectFromString("TELEPORT_CHARGE", &effectType);
    CDefHolder<CSpecialEffectsDef> effects(out.Object);
    if (found) {
        CThing* effect = CreateEffectThing(effectType, &GetParent()->GetPosition(), false);
        if (effect && !(effect->m_flags91 & 1))
            PEffect = CIntelligentPointerThing(effect);
    }
    }
}
}
