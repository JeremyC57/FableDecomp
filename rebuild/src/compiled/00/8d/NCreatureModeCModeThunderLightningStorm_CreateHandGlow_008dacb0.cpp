// NCreatureMode::CModeThunderLightningStorm::CreateHandGlow() -- spawn Thunder's LIGHTNING_HAND_EFFECT at his position, keep it in the mode's hand-glow pointer and attach its particle emitter to his WEAPON_FOCUS_01 bone
// Leaf: CreateHandGlow
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
    unsigned char  _pad54[0x3d];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
private: public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};
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
        CThing* glow = CTCDParticleEmitter::Create(effectType, *(&position), false);
        if (glow && !glow->IsDead()) {
            PHandGlow = CIntelligentPointer<CThing>(glow);
            CTCDParticleEmitter* emitter;
            if (glow->GetInterface(&emitter, 0x4b) && emitter)
                emitter->AttachToThing(*ParentThing, 10, "WEAPON_FOCUS_01", 0, 0.0f);
        }
    }
}
}