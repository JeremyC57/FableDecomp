// CTCInventoryClothing::InitialiseMannequin() -- create the inventory mannequin from the inventory def at the owner, copy the owner's appearance components onto it and start its DEFAULT animation
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
class CAnimationEntry { public:
    long AnimIndex;                                            // +0x00
    unsigned long GetAnimateThingFlags() const;                // 0x005DC390
    long GetTransitionInTime() const;                          // 0x005DC1D0
};
class CAnimFilterBase { public:                                       // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {                  // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
};
class CAnimationSet { public:
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                                    // slot 1
    long RefCount;                                             // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                                   // +0x34
};
class CThingBaseDef : public CDefPointeeBase { public:
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

class CAnimFrameInfo { public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
    CAnimFrameInfo(const CAnimFrameInfo& o) : Current(o.Current), Last(o.Last) {}
};
class CAnimInit { public:                                             // 0x2c bytes
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
class CAnimSet { public:
    long          UpdateMode;                                  // +0x00
    CAnimRef*     Anims;                                       // +0x04 vector _M_start
    unsigned char _pad08[0x1c - 0x08];
    CAnimFrameInfo Frame;                                      // +0x1c
};
class CAnimateThing { public:
    CAnimSet MainAnimSet;
    CAnimFrameInfo GetFrameInfo() const { return MainAnimSet.Frame; }
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
    void UpdateFrameLogic(const CAnimFrameUpdate& update);     // 0x0070E8B0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                             // 0x0070B460
};
class CTCBase;
class CTCHeroAttachableAppearanceModifiers { public:                  // interface 0x5E
    void EditFrameUpdate();                                    // 0x00707930
};
struct CPresenceBits { bool test(long bit); };                 // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
class C3DVector { public: float X, Y, Z; };
class CThing { public:
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                                  // +0x20
    unsigned char  _pad21[0x23];
    CInterfaceMap  m_interfaces;                               // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCBase*       PTCBase64;                                  // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CThingBaseDef* PBaseDef;                                   // +0x70
    const C3DVector& GetPos() const;                      // 0x004C73D0
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

class CInventoryDef : public CDefPointeeBase { public:
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
class CTCInventoryClothing { public:
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
    PMannequin = CreateThing(GetInventoryDef()->MannequinDef, GetParentThing()->GetPos(), true);
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