// CTCInventoryClothing::InitialiseMannequin() -- create the inventory mannequin from the inventory def at the owner, copy the owner's appearance components onto it and start its DEFAULT animation
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CAnimationEntry {
    long AnimIndex;                                            // +0x00
    unsigned long GetAnimateThingFlags() const;                // 0x005DC390
    long GetTransitionInTime() const;                          // 0x005DC1D0
};
struct CAnimFilterBase {                                       // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {                  // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
};
struct CAnimationSet {
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                                    // slot 1
    long RefCount;                                             // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                                   // +0x34
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef_0();
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CAnimFrameInfo {
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
    CAnimFrameInfo(const CAnimFrameInfo& o) : Current(o.Current), Last(o.Last) {}
};
struct CAnimInit {                                             // 0x2c bytes
    long          UpdateMode;
    unsigned char Type;
    bool          Looping;
    long          Anim;
    float         Speed;
    float         Alpha;
    float         LastAlpha;
    CAnimFrameInfo Frame;                                      // +0x18
    unsigned long Flags;
    unsigned long TypeFlags;
    long          Priority;
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags,
              long priority)
    {
        Set(type, typeFlags, anim, updateMode, looping, flags, priority);
    }
    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags,
             long priority);                                   // 0x0070C050
};
struct CAnimFrameUpdate {
    float Time;
    float Reserved0;
    float Reserved1;
    CAnimFrameUpdate(float time) { Time = time; Reserved0 = 0; Reserved1 = 0; }
};
struct CAnimRef { short Index; };
struct CAnimSet {
    long          UpdateMode;                                  // +0x00
    CAnimRef*     Anims;                                       // +0x04 vector _M_start
    unsigned char _pad08[0x1c - 0x08];
    CAnimFrameInfo Frame;                                      // +0x1c
};
struct CAnimateThing {
    CAnimSet MainAnimSet;
    CAnimFrameInfo GetFrameInfo() const { return MainAnimSet.Frame; }
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
    void UpdateFrameLogic(const CAnimFrameUpdate& update);     // 0x0070E8B0
};
struct CTCAnimationComplex {
    CAnimateThing& GetAnimation();                             // 0x0070B460
};
struct CTCBase;
struct CTCHeroAttachableAppearanceModifiers {                  // interface 0x5E
    void EditFrameUpdate();                                    // 0x00707930
};
struct CPresenceBits { bool test(long bit); };                 // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                                  // +0x20
    unsigned char  _pad21[0x23];
    CInterfaceMap  m_interfaces;                               // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCBase*       PTCBase64;                                  // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CThingBaseDef* PBaseDef;                                   // +0x70
    const C3DVector& GetPosition() const;                      // 0x004C73D0
    void AddAndCopyTC(CTCBase* tc, bool activate);             // 0x004C9F60
    CTCBase* AddTC(const CCharString& name, bool activate, void* params);   // 0x004C9D60
    bool HasTC(long type) { return m_present.test(type); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceFound(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it != map->last && !(wanted < it->type)) ? it : map->last)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTCFound(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it != map->last && !(wanted < it->type)) ? it : map->last)->component;
    }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, bool flag);   // 0x00835A20

struct CInventoryDef : CDefPointeeBase {
    unsigned char _pad08[0xd4 - 0x08];
    long MannequinDef;                                         // +0xd4
};
template <class T> struct CDefPointerCopy {
    T* Object;
    __forceinline CDefPointerCopy(T* p) : Object(p) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointerCopy() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
// Layout: retail CTCInventoryClothing (12-byte containers): ParentThing +0x04, the inventory def +0x68, PMannequin +0x158.
struct CTCInventoryClothing {
    void*   __vftable;
    CThing* ParentThing;                                       // +0x04
    unsigned char _pad08[0x68 - 0x08];
    CInventoryDef* PInventoryDef;                              // +0x68 (CDefPointer)
    unsigned char _pad6c[0x158 - 0x6c];
    CThing* PMannequin;                                        // +0x158
    __forceinline CDefPointerCopy<CInventoryDef> GetInventoryDef() const { return CDefPointerCopy<CInventoryDef>(PInventoryDef); }
    __forceinline CThing* GetParentThing() const { return ParentThing; }
    void InitialiseMannequin();
};
void CTCInventoryClothing::InitialiseMannequin()
{
    if (GetInventoryDef()->MannequinDef == 0) {
        PMannequin = 0;
        return;
    }
    PMannequin = CreateThing(GetInventoryDef()->MannequinDef, GetParentThing()->GetPosition(), true);
    if (PMannequin) {
        PMannequin->AddAndCopyTC(ParentThing->PTCBase64, false);
        CTCBase* tc;
        if (ParentThing->GetInterface(&tc, 3))
            PMannequin->AddAndCopyTC(tc, false);
        if (ParentThing->GetInterface(&tc, 0x5e))
            PMannequin->AddAndCopyTC(tc, false);
        if (ParentThing->GetInterfaceFound(&tc, 0x78))
            PMannequin->AddAndCopyTC(tc, false);
        CTCBase* tc5c = 0;
        ParentThing->GetInterface(&tc5c, 0x5c);
        PMannequin->AddAndCopyTC(tc5c, false);
        PMannequin->AddTC("CTCAnimationComplex", false, 0);
        CTCAnimationComplex* complex = 0;
        PMannequin->GetInterface(&complex, 0x5a);
        CDefPointer<CAppearanceDef> appearance;
        CDefPointer<CThingBaseDef>(PMannequin->PBaseDef)->GetSubDef(&appearance);
        const CAnimationEntry* entry = appearance->Animation.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
        CAnimInit init(6, 0, entry->AnimIndex, 1, true, entry->GetAnimateThingFlags(), 100);
        complex->GetAnimation().SetMainAnim(init, 1, 0);
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (PMannequin->GetInterface(&modifiers, 0x5e))
            modifiers->EditFrameUpdate();
    }
}
