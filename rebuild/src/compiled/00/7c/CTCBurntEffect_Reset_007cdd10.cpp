// Leaf: Reset
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// SW2 sweep (advisor/007cdd10/probe/t3.cpp).
// CTCBurntEffect::Reset() -- restart the burn: zero CurTime, scale the def's fade times by the frame rate, and if the
// def's "SINGE_EFFECT" sub-def names an effect type, spawn three effect things at the owner, attach their particle emitters
// (two global helper names on attach type 10, "" on type 8) and keep all three in Effects
// Retail 0x007CDD10, 0x2CF bytes. Callees: __ftol2 0x00BFEA70; CThing::GetPos 0x004C73D0;
// CDefClassBase::GetSubDef 0x00669580; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
// GetEffectFromString 0x00669D30; CreateEffectThing 0x006E0880 (__fastcall); CVectorMap::LowerBound 0x0040F020
// (inlined GetTC 0x4b); CTCDParticleEmitter::AttachToThing 0x006E0BE0; CBaseIntelligentPointer() 0x00A01B10 /
// SetPItem 0x00A01B90 / ~CBaseIntelligentPointer 0x00A01C10; vector<CIntelligentPointer<CThing>>::push_back 0x004AD520.
// Layout: the landed CTCBurntEffect ctor 0x007CDFE0 (PDef +0x0c, CurTime +0x10, fade times +0x14..+0x1c, Effects +0x20).
// Name fix (agentNF6 2026-11-10): 0x013BAC30 is ?PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME@@3VCCharString@@B (const CCharString "WEAPON_FOCUS_01"; Ego_r.pdb / FableWin.pdb public records and their dynamic initializer names).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BAC34 SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME (global, const; initializer 0x0121E620), was g_BurntHelperB.
#include "fable_compiler.h"
#include "engine/CTCBase.h"

#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CEffectDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) {}
    ~CDefPointer() { if (Data) { if (--Data->RefCount == 0) Data->Destroy(); } }
    T* operator->() const { return Data; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CEffectDef>* out);     // 0x00669580
};
class CSpecialAbilitiesBurntEffectDef : public CDefClassBase { public:
    unsigned char _pad00[0x3c];
    float FadeInTime;                                 // +0x3c
    float FadeOutTimeStart;                           // +0x40
    float FadeOutTime;                                // +0x44
};
class CThing;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& value);                   // 0x004AD520
};
struct CTCBaseX;
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
private: public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                          // +0x91 bit 0 = being deleted
    const C3DVector& GetPos() const;             // 0x004C73D0
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((CTCBaseX*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second);
    }
    template <class T> FABLE_FORCEINLINE T* GetTCDriver(long interfaceType)
    {
        if (!(m_flags91 & 1) && HasTC(interfaceType))
            return GetTC<T>(interfaceType);
        return 0;
    }
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;              // 0x013BAC30
extern const CCharString SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;              // 0x013BAC34

class CTCBurntEffect : public CTCBase { public:
    CSpecialAbilitiesBurntEffectDef* PDef;            // +0x0c
    long CurTime;                                     // +0x10
    long FadeInTime;                                  // +0x14
    long FadeOutTimeStart;                            // +0x18
    long FadeOutTime;                                 // +0x1c
    CRetailVector< CIntelligentPointer<CThing> > Effects;   // +0x20
    CSpecialAbilitiesBurntEffectDef* GetDef() const { return PDef; }
    void Reset();
};

long LookupEffectStandIn(const CEffectDef* def, const CCharString& name);   // stand-in callee
__declspec(noinline) bool CEffectDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = LookupEffectStandIn(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}

void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CEffectDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}

void CTCBurntEffect::Reset()
{
    CurTime = 0;
    FadeInTime = (long)(PDef->FadeInTime * CMainGameComponent::GetConstantFPS());
    FadeOutTimeStart = (long)(PDef->FadeOutTimeStart * CMainGameComponent::GetConstantFPS());
    FadeOutTime = (long)(PDef->FadeOutTime * CMainGameComponent::GetConstantFPS());
    CDefPointer<CEffectDef> def;
    long type = 0;
    C3DVector position = ParentThing->GetPos();
    if (GetDef()->GetSubDef(&def) && def->GetEffectFromString("SINGE_EFFECT", type)) {
        CThing* effectA = CTCDParticleEmitter::Create(type, *(&position), false);
        if (effectA) {
            CTCDParticleEmitter* emitter = effectA->GetTCDriver<CTCDParticleEmitter>(0x4b);
            if (emitter)
                emitter->AttachToThing(*ParentThing, 10, PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
        }
        CThing* effectB = CTCDParticleEmitter::Create(type, *(&position), false);
        if (effectB) {
            CTCDParticleEmitter* emitter = effectB->GetTCDriver<CTCDParticleEmitter>(0x4b);
            if (emitter)
                emitter->AttachToThing(*ParentThing, 10, SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
        }
        CThing* effectC = CTCDParticleEmitter::Create(type, *(&position), false);
        if (effectC) {
            CTCDParticleEmitter* emitter = effectC->GetTCDriver<CTCDParticleEmitter>(0x4b);
            if (emitter)
                emitter->AttachToThing(*ParentThing, 8, "", 0, 0.0f);
        }
        Effects.push_back(CIntelligentPointer<CThing>(effectA));
        Effects.push_back(CIntelligentPointer<CThing>(effectB));
        Effects.push_back(CIntelligentPointer<CThing>(effectC));
    }
}