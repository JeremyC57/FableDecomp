// NCreatureMode::CModeBriarRoseTeleportInvulnerable::OnActivate() -- Briar Rose charges a teleport: set the 1.25 s frame count, suspend a player creature's animation, advance the boss phase when ready, and spawn the TELEPORT_CHARGE effect at her position, keeping it in the mode's effect pointer
// Leaf: OnActivate
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_GameFramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Shape: GetEffectFromString (0x00669D30) gets a same-TU out-type-only stand-in body (the 0x008D9740 idiom), so the effect
//   type does not escape and the CIntelligentPointer temp overlays its word as retail; `out` declared before `effectType`
//   (zero-store order).
// Callees: CModeStandBase::OnActivate 0x008621A0, __ftol2 0x00BFEA70, SuspendAI 0x00833A10, LowerBound 0x0040F020, CTCBoss::IsReadyForPhaseChange 0x007AEE90 / IncrementBossPhase 0x007AED20, GetSubDef 0x00669580, CCharString 0x0099EBF0/~ 0x0099EAE0, GetEffectFromString 0x00669D30, GetPos 0x004C73D0, CreateEffectThing 0x006E0880, CBaseIntelligentPointer 0x00A01B10/B90/B50/C10.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CTCBase;
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
class CSpecialEffectsDef : public CDefPointeeBase { public:
    bool GetEffectFromString(const CCharString& name, long* type);   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out type only, so VC7.1 knows the effect type's address does
// not escape the call (the 0x008D9740 idiom).
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
struct CModeBase;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
class CThing { public:
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                      // +0x10
    unsigned char  _pad11[0x0f];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad54[0x1c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char  _pad74[0x1d];
    unsigned char  m_flags91;                   // +0x91
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CTCBoss { public:
    bool IsReadyForPhaseChange() const;               // 0x007AEE90
    void IncrementBossPhase();                  // 0x007AED20
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
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
    CIntelligentPointer<CThing> PEffect;           // +0x80
    void OnActivate();
};
void CModeBriarRoseTeleportInvulnerable::OnActivate()
{
    CModeStandBase::OnActivate();
    FrameCount = (long)(CMainGameComponent::GetConstantFPS() * 1.25f);
    if (ParentThing->m_type == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(true, this);
    {
    CTCBoss* boss;
    if (ParentThing->GetInterface(&boss, 0xad) && boss->IsReadyForPhaseChange())
        boss->IncrementBossPhase();
    }
    {
    CDefPointer<CSpecialEffectsDef> out;
    long effectType = 0;
    bool found = CDefCopy<CDefClassBase>(ParentThing->PDef)->GetSubDef(&out)
        && out.Object->GetEffectFromString("TELEPORT_CHARGE", &effectType);
    CDefHolder<CSpecialEffectsDef> effects(out.Object);
    if (found) {
        CThing* effect = CTCDParticleEmitter::Create(effectType, *(&GetParent()->GetPos()), false);
        if (effect && !(effect->m_flags91 & 1))
            PEffect = CIntelligentPointer<CThing>(effect);
    }
    }
}
}