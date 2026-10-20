// CTCSoundPlayer::OnInitialActivate() -- copy the anim-criteria clip distance from the thing def's sound-player sub-def
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066B040, length 61 bytes; vtable 0x0125B1C4 (CTCSoundPlayer) slot 6,
// the CTCBase OnInitialActivate slot.
// Callees: CDefClassBase::GetSubDef<CSoundPlayerDef>(CDefPointer<T>*) const @ 0x0065F604 (manifest
// "GetSubDef<CAbilityDef>" names a byte-identical instantiation); the sub-def's virtual Destroy (slot 1) on release.
// Layout: the thing keeps its counted def pointer at +0x70 (as CTCAISummon::OnInitialActivate 0x007FDE70);
// CTCSoundPlayer::AnimCriteriaClipDistance (short) +0x1c; the sub-def's clip distance is a short at +0x50.
// Shape: a null-initialised local CDefPointer filled by GetSubDef, read when it succeeds, released at the end
// (one null test, `dec [ecx+4]` in place).
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CSoundPlayerDef;

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long m_refCount;                                        // +0x04
};

template <class T>
struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            if (DecRefToZero(Object->m_refCount))
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

struct CSoundPlayerDef : CDefPointee {
    unsigned char _pad[0x50 - 8];
    short         AnimCriteriaClipDistance;                 // +0x50
};

struct CThingDef : CDefPointee {
    bool GetSubDef(CDefPointer<CSoundPlayerDef>* out) const;   // 0x0065F604
};
void* FindDef_CThingDef_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointer<CSoundPlayerDef>* out) const
{
    void* def = FindDef_CThingDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char          _pad[0x70];
    CDefPointer<CThingDef> PDef;                            // +0x70
};

struct CTCSoundPlayer {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    bool          ToBeRemoved;                              // +0x08
    unsigned char _pad0c[0x1c - 0x09];
    short         AnimCriteriaClipDistance;                 // +0x1c
    long          LastMaterialIndex;                        // +0x20
    void OnInitialActivate();
};

void CTCSoundPlayer::OnInitialActivate()
{
    CDefPointer<CSoundPlayerDef> def;
    if (ParentThing->PDef->GetSubDef(&def))
        AnimCriteriaClipDistance = def->AnimCriteriaClipDistance;
}