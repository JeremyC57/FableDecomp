// Grade: DIFFER(224v205 ours v retail) -- the `&&` with a by-value definition copy and a string temporary: retail guards both temporaries with a flag word (ebx = 1 then 3, cleared per destructor) and zeroes one frame word twice; the lane builds the addref/test, the flag word and the destructors in a different arrangement (19 bytes longer); only the plain landed UndeadReturnToEarth form was tried
// CTCLightningOrb::OnDie() -- when the orb dies, spawns its definition's LIGHTNING_ORB_HIT special effect at the orb's position (when the def's special-effects sub-def maps that name)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F2410, length 208 bytes (+ int3 padding); speed TU (no frame pointer).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) @ 0x00669580, CCharString(const char*, long)
// @ 0x0099EBF0 ("LIGHTNING_ORB_HIT" 0x01271D48) and ~CCharString @ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&,
// long&) const @ 0x00669D30 (catalog "CCRC::CalcAndLookup"), CThing::GetPosition @ 0x004C73D0, CreateEffectThing(long, const C3DVector*,
// bool) @ 0x006E0880 (`__fastcall`), the defs' virtual slot 1 on release. CThing::PDef +0x70; ParentThing +0x04.
// Shape: the landed UndeadReturnToEarth idiom with the thing's definition taken by value (CThing::GetPDef: the copy's addref and its
// release at the end of the condition, the `&&` string temporary): VC keeps the two temporaries' construction flags in ebx (1 then 3,
// the string's bit cleared at its destructor) and the condition in a frame byte; the effects sub-def is a zeroed out slot released by
// hand (null-tested) after the spawn.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> struct CDefPointerSlot {      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointerSlot() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointerSlot<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
template <class T> struct CDefPointer {          // a counted def reference held by value
    T* m_p;
    CDefPointer(const CDefPointer& other) : m_p(other.m_p)
    {
        if (m_p)
            ++m_p->m_refCount;
    }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_refCount == 0)
            m_p->Destroy();
    }
    T* operator->() const { return m_p; }
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;             // +0x70
    CDefPointer<CDefClassBase> GetPDef() const { return PDef; }
    const C3DVector& GetPosition() const;        // 0x004C73D0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CTCLightningOrb {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                   // +0x04
    void OnDie();
};
void CTCLightningOrb::OnDie()
{
    CDefPointerSlot<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    bool found = ParentThing->GetPDef()->GetSubDef(effectsSlot) && effectsSlot.m_p->GetEffectFromString("LIGHTNING_ORB_HIT", effectType);
    if (found)
        CreateEffectThing(effectType, &ParentThing->GetPosition(), false);
    ReleaseDef(effectsSlot.m_p);
}
