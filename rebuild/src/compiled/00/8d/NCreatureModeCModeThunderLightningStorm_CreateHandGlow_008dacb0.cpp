// NCreatureMode::CModeThunderLightningStorm::CreateHandGlow() -- spawn Thunder's LIGHTNING_HAND_EFFECT at his position, keep it in the mode's hand-glow pointer and attach its particle emitter to his WEAPON_FOCUS_01 bone
// Leaf: CreateHandGlow
// Shape: GetSubDef and GetEffectFromString have same-TU out-slot-only stand-in bodies, so the effect type does not escape:
//   VC7.1 overlays its word with the intelligent-pointer temporary, the GetInterface key and the bone-name string (as
//   retail); GetInterface out-store as one ternary.
// Callees: GetSubDef 0x00669580 (same-TU stand-in body), CCharString 0x0099EBF0/~ 0x0099EAE0, GetEffectFromString 0x00669D30, GetPos 0x004C73D0, CreateEffectThing 0x006E0880, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10, LowerBound 0x0040F020, CTCDParticleEmitter::AttachToThing 0x006E0BE0.
class CTCBase;
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
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

class CDefPointeeBase { public:
    virtual void Destroy();
    virtual void Delete();
    long m_refCount;
};
template <class T> class CDefPointer { public:
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
class CSpecialEffectsDef : public CDefPointeeBase { public:
    bool GetEffectFromString(const CCharString& name, long* type);   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out type only, so VC7.1 knows the effect type's address does
// not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long* type)
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    *type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefPointeeBase { public:
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
class CThing { public:
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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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
    CIntelligentPointer<CThing> PHandGlow;         // +0x9c
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
        C3DVector position = ParentThing->GetPos();
        CThing* glow = CreateEffectThing(effectType, &position, false);
        if (glow && !glow->IsDead()) {
            PHandGlow = CIntelligentPointer<CThing>(glow);
            CTCDParticleEmitter* emitter;
            if (glow->GetInterface(&emitter, 0x4b) && emitter)
                emitter->AttachToThing(ParentThing, 10, "WEAPON_FOCUS_01", 0, 0.0f);
        }
    }
}
}