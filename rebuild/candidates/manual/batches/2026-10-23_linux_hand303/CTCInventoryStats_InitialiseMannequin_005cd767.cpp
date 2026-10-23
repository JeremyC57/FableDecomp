// CTCInventoryStats::InitialiseMannequin() -- creates the stats-screen mannequin from the hero def's mannequin def at the hero's position, copies the hero's appearance components onto it, adds an animation component and starts its DEFAULT animation, then gives it an editable frame update
// Leaf: InitialiseMannequin@CTCInventoryStats
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CD767, 745 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CThing::GetPosition 0x004C73D0, the thing-create helper 0x00835A20 (`__fastcall` def index / position, stack bool),
//   CThing::AddAndCopyTC 0x004C9F60, CThing::AddTC(const CCharString&, bool, bool) 0x004C9D60, bitset test 0x00410DE0,
//   LowerBound 0x0040F020 (non-const), CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, animation-set lookup 0x005DC340
//   (CAnimFilterNoGroup temporary, vtables 0x0122FFDC / 0x0122FFCC), CAnimationEntry::GetAnimateThingFlags 0x005DC390,
//   CAnimInit::Set 0x0070C050, CTCAnimationComplex::GetAnimation 0x0070B460, CAnimateThing::SetMainAnim 0x0070D580,
//   component 0x5E EditFrameUpdate 0x00707930, CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: the hero def at +0x68 is copied twice into def-pointer temporaries whose destructor skips the null test; the parent
//   thing is read into a named local before the create call (retail loads it ahead of the def copy); the 0x5C component is
//   added unconditionally (read uninitialised when absent, as retail); all def-pointer functions are __forceinline.
// Layout: local views -- CTCInventoryStats ParentThing +0x04, hero def +0x68, Mannequin +0x140; CThing presence bits +0x20,
//   interface map +0x44, component +0x64, base def +0x70; hero def mannequin def index +0x2E0; appearance animation set +0x34.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct C3DVector;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CAnimationEntry {
    long AnimIndex;                                     // +0x00
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
};
struct CAnimFilterBase {                                // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {           // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
};
struct CAnimationSet {
    CAnimationEntry* GetAnimation(const CCharString& name, long n, const CAnimFilterBase& filter) const;   // 0x005DC340
};
struct CDefPointeeBase {
    virtual void v000();
    virtual void DestroyDef();                          // slot 1 (+4)
    long m_refCount;                                    // +4
};
template<class T> struct CDefPointer {
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    __forceinline CDefPointer(T* p) : m_pObject(p) { if (p) ++p->m_refCount; }
    __forceinline ~CDefPointer() { if (m_pObject) { if (--m_pObject->m_refCount == 0) m_pObject->DestroyDef(); } }
    T* operator->() const { return m_pObject; }
};
template<class T> struct CDefRef {
    T* m_pObject;
    __forceinline CDefRef(T* p) : m_pObject(p) { if (p) ++p->m_refCount; }
    __forceinline ~CDefRef() { if (--m_pObject->m_refCount == 0) m_pObject->DestroyDef(); }
    T* operator->() const { return m_pObject; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x2c];
    CAnimationSet Animation;                            // +0x34
};
struct CDefClassBase : CDefPointeeBase {
    CAppearanceDef* FindAppearanceDef();
    bool GetSubDef(CDefPointer<CAppearanceDef>* out);   // 0x0042B0A2
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>* out)
{
    CAppearanceDef* def = FindAppearanceDef();
    if (!def)
        return false;
    out->m_pObject = def;
    return true;
}
#pragma auto_inline(on)
struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
struct CAnimInit {                                      // 0x2c bytes
    long          Mode;
    unsigned char AnimType;
    bool          Flag05;
    long          AnimIndex;
    float         Scales[3];
    CBlendPair    Blend;                                // +0x18
    unsigned long Flags;
    long          Arg24;
    long          Percent;
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
struct CAnimateThing {
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
struct CTCAnimationComplex {
    CAnimateThing& GetAnimation();                      // 0x0070B460
};
struct CTCEditable {
    void EditFrameUpdate();                             // 0x00707930
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CPresenceBits { bool test(long kind) const; };   // bitset<>::test 0x00410DE0
struct CThing {
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                           // +0x20
    unsigned char  _pad21[0x44 - 0x21];
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCBase*       Clothing;                            // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* PBaseDef;                            // +0x70
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void AddAndCopyTC(CTCBase* tc, bool b);             // 0x004C9F60
    void AddTC(const CCharString& name, bool a, bool b);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (m_present.test(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, bool b);   // 0x00835A20
struct CHeroDef : CDefPointeeBase {
    unsigned char _pad08[0x2e0 - 0x08];
    long MannequinDef;                                  // +0x2e0
};
struct CTCInventoryStats {
    void*     __vftable;
    CThing*   ParentThing;                              // +0x04
    unsigned char _pad08[0x68 - 0x08];
    CHeroDef* HeroDef;                                  // +0x68
    unsigned char _pad6c[0x140 - 0x6c];
    CThing*   Mannequin;                                // +0x140
    void InitialiseMannequin();
};
void CTCInventoryStats::InitialiseMannequin()
{
    bool noMannequin = CDefRef<CHeroDef>(HeroDef)->MannequinDef == 0;
    if (noMannequin) {
        Mannequin = 0;
        return;
    }
    CThing* parent = ParentThing;
    Mannequin = CreateThing(CDefRef<CHeroDef>(HeroDef)->MannequinDef, parent->GetPosition(), true);
    Mannequin->AddAndCopyTC(ParentThing->Clothing, false);
    CTCBase* tc;
    if (ParentThing->GetInterface(&tc, 0x5e))
        Mannequin->AddAndCopyTC(tc, false);
    if (ParentThing->GetInterface(&tc, 0x78))
        Mannequin->AddAndCopyTC(tc, false);
    if (ParentThing->GetInterface(&tc, 3))
        Mannequin->AddAndCopyTC(tc, false);
    {
        CTCBase* tc5c;
        ParentThing->GetInterface(&tc5c, 0x5c);
        Mannequin->AddAndCopyTC(tc5c, false);
    }
    Mannequin->AddTC("CTCAnimationComplex", false, false);
    CTCAnimationComplex* complex;
    Mannequin->GetInterface(&complex, 0x5a);
    CDefPointer<CAppearanceDef> appearance;
    CDefPointer<CDefClassBase>(Mannequin->PBaseDef)->GetSubDef(&appearance);
    CAnimationEntry* entry = appearance->Animation.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
    CAnimInit init(6, 0, entry->AnimIndex, 1, true, entry->GetAnimateThingFlags(), 100);
    complex->GetAnimation().SetMainAnim(init, 1, 0);
    CTCEditable* editable;
    if (Mannequin->GetInterface(&editable, 0x5e))
        editable->EditFrameUpdate();
}
