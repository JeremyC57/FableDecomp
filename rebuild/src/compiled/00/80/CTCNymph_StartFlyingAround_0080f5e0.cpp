// CTCNymph::StartFlyingAround(long fadeTime) -- unless already flying: look up the INLIMBO special effect, take the start position from the appearance's DUMMY_CENTRE dummy (or the thing's position), kill any old effect and spawn the limbo effect attached at DUMMY_CENTRE, then start the fade counter and hide the owner
// Leaf: StartFlyingAround
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080F5E0, 656 bytes, `ret 4`.
#include "engine/CTCBase.h"

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
};

template <class T> class CDefPointer;
class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long RefCount;                                  // +0x04
    bool GetSubDef(CDefPointer<const class CSpecialEffectsDef>* out) const;   // GetSubDef<CSpecialEffectsDef> 0x00669580
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CSpecialEffectsDef : public CDefClassBase { public:
    bool GetEffectFromString(const CCharString& name, long* effect) const;   // 0x00669D30
};

class CTCBaseComponent;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long* value) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CInterfaceBits {                                     // bitset<> at CThing+0x20
    unsigned long _M_w[9];
    bool test(unsigned long pos) const { return (_M_w[pos / 32] & (1ul << (pos % 32))) != 0; }
};
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetVisible(bool visible);                  // slot 28 (+0x70)
    unsigned char _pad04[0x20 - 0x04];
    CInterfaceBits present;                                 // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    struct CTCGraphicAppearance* GraphicAppearance;         // +0x64
    struct CTCGraphicAppearance* GetGraphicAppearance() const { return GraphicAppearance; }
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* PDef;                                    // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                                  // +0x91 (bit 0: dead)
    unsigned char _pad92[0xbc - 0x92];
    unsigned char FlagsBC;                                  // +0xbc
    void Kill(bool removeNow);                              // 0x004C9B80
    bool GetDummyObject(const CCharString& name, C3DVector* position, class CRightHandedSet* orientation);
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
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
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
// Same-TU stand-in for the GetSubDef<T> COMDAT (as the landed 0x00801A50): with the body visible VC7.1 sees that the out
// pointer does not escape and keeps the def object in ebp through to its release, as retail.
const CDefClassBase* LookUpSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSpecialEffectsDef* def = (const CSpecialEffectsDef*)LookUpSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)



class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
inline bool CThing::GetDummyObject(const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
{
    return GraphicAppearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
}
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool attached);   // 0x006E0880
    void AttachToThing(CThing* thing, long mode, const CCharString& bone, long flags, float offset);   // 0x006E0BE0
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    bool IsAlive() const { return GetPItem() && !(GetPItem()->Flags91 & 1); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

extern long g_FramesPerSecond;                              // 0x01375550
class CNymphDef { public:
    unsigned char _pad00[0x4c];
    float FadeOutTime;                                      // +0x4c
};
class CTCNymph : public CTCBase { public:
    const CNymphDef* PDef;                                  // +0x0c
    C3DVector PointToFlyAround;                             // +0x10
    float EffectHeightOffset;                               // +0x1c
    C3DVector InitialPos;                                   // +0x20
    unsigned char _pad2c[0x40 - 0x2c];
    bool FlyingAround;                                      // +0x40
    CIntelligentPointer<CThing> PEffect;                    // +0x41
    unsigned char _pad49[0x5b - 0x49];
    bool HasBeenInLimbo;                                    // +0x5b
    unsigned char _pad5c[0x60 - 0x5c];
    long FadeCounter;                                       // +0x60
    long TotalFadeTime;                                     // +0x64
    void PickNewTargetPos();                                // 0x0080F270
    void StartFlyingAround(long fadeTime);
};

void CTCNymph::StartFlyingAround(long fadeTime)
{
    long effect = 0;
    if (FlyingAround)
        return;
    CDefPointer<const CSpecialEffectsDef> effects;
    effect = 0;
    const CDefClassBase* thingDef = ParentThing->PDef;
    thingDef->GetSubDef(&effects) && effects->GetEffectFromString("INLIMBO", &effect);
    {
        CRightHandedSet orientation;
        bool notFound = !ParentThing->GetGraphicAppearance()->GetDummyObject("DUMMY_CENTRE", 0, 0, &InitialPos, &orientation, 1.0f);
        if (notFound)
            InitialPos = ParentThing->GetPosition();
    }
    PointToFlyAround = InitialPos;
    EffectHeightOffset = InitialPos.Z - ParentThing->GetPosition().Z;
    if (PEffect.IsAlive()) {
        PEffect.GetPItem()->Kill(true);
        PEffect = CIntelligentPointer<CThing>(0);
    }
    PEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, PointToFlyAround, true));
    if (PEffect.IsAlive()) {
        CTCDParticleEmitter* emitter = 0;
        PEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(ParentThing, 0xa, "DUMMY_CENTRE", 0, 0.0f);
        FlyingAround = true;
        if (fadeTime == -1)
            fadeTime = (long)(g_FramesPerSecond * PDef->FadeOutTime);
        FadeCounter = TotalFadeTime = fadeTime;
        ParentThing->FlagsBC &= 0xfa;
        if (FadeCounter == 0) {
            PickNewTargetPos();
            ParentThing->SetVisible(false);
        }
    }
    HasBeenInLimbo = true;
}