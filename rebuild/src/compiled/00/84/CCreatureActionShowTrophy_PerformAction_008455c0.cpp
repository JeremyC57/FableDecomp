// CCreatureAction_ShowTrophy::PerformAction() -- while the shown trophy is alive: kill a live emitter, respawn the
// "SHOW_TROPHY_HELD_ALOFT" effect at the creature's WEAPON_FOCUS_01 dummy as PEmitter (attached to the right hand), then tell the
// trophy's CTCTrophy (interface 0x39) that the creature showed it
// (the effect lookup is the Initialise 0x00845350 shape)
// Leaf: ?PerformAction@CCreatureAction_ShowTrophy@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008455C0, length 641 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10,
//   CVectorMap::LowerBound 0x0040F020, CTCTrophy::OnShownByCreature 0x006DE8C0 (descriptive name), CThing::Kill(bool) 0x004C9B80,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30,
//   CThing::GetPos 0x004C73D0, CTCGraphicAppearance::GetDummyObject 0x004C08C0, CreateEffectThing 0x006E0880,
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME 0x013BAC30,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Layout: Ego_r.pdb PTrophyBeingShown +0xb4, PEmitter +0xc1 (packed); retail CCreatureActionBase is 0xa8, so the members
//   sit 12 bytes lower (PEmitter at the odd offset +0xb5; the landed FinishAction 0x00845850).
// Shape: GetSubDef / GetEffectFromString get same-TU stand-in bodies; operator= names the item before SetPItem (this+0xb5 loads late); the && chain carries the by-value def copy and the
//   name temporary (conditional destruction flags 1/2); the sub def pointer lives to the end of the function.
// Name fix (agentNF6 2026-11-10): 0x013BAC30 is ?PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME@@3VCCharString@@B (const CCharString "WEAPON_FOCUS_01"; Ego_r.pdb / FableWin.pdb public records and their dynamic initializer names).
// Name fix (agentNF6 2026-11-10): NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0 is ?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z (Ego_r.pdb / FableWin.pdb): namespace NGraphicAppearance, const, position and orientation by reference.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;          // 0x013BAC30
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Find_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CSpecialEffectsDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing;
class CTCBase;
class CTCTrophy { public:
    void OnShownByCreature_006de8c0(CThing* creature);   // 0x006DE8C0
};
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
private: public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& position, CRightHandedSet& orientation, float scale) const;   // 0x004C08C0
};
}
using NGraphicAppearance::CTCGraphicAppearance;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x10];
    CTCGraphicAppearance* GraphicAppearance;    // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    void Kill(bool silent);                     // 0x004C9B80
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
    bool GetDummyObject(const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
    {
        return GraphicAppearance->GetDummyObject(name, 0, 0, *position, *orientation, 1.0f);
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

#pragma pack(push, 1)
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentThingPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentThingPointer() : CBaseIntelligentPointer() { }
    FABLE_FORCEINLINE CIntelligentThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentThingPointer& operator=(const CIntelligentThingPointer& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};
class CCreatureActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CThing* GetCreature() const { return ((CThing*)PCreature.GetPItem()); }
};
class CCreatureAction_ShowTrophy : public CCreatureActionBase {
public:
    CIntelligentThingPointer PTrophyBeingShown; // +0xa8 (PDB +0xb4)
    unsigned long            ShowingTrophyDeedHandle;   // +0xb0
    bool                     PostedDeed;        // +0xb4
    CIntelligentThingPointer PEmitter;          // +0xb5 (packed)
    virtual void PerformAction();
    FABLE_FORCEINLINE CTCTrophy* GetTrophy() const
    {
        CTCTrophy* trophy;
        ((CThing*)PTrophyBeingShown.GetPItem())->GetInterface(&trophy, 0x39);
        return trophy;
    }
};
#pragma pack(pop)

void CCreatureAction_ShowTrophy::PerformAction()
{
    if (((CThing*)PTrophyBeingShown.GetPItem()) && !(((CThing*)PTrophyBeingShown.GetPItem())->m_flags91 & 1)) {
        if (((CThing*)PEmitter.GetPItem()) && !(((CThing*)PEmitter.GetPItem())->m_flags91 & 1))
            ((CThing*)PEmitter.GetPItem())->Kill(true);
        CDefPointer<CSpecialEffectsDef> effects;
        long effectType = 0;
        if (GetCreature() && GetCreature()->GetDef()->GetSubDef(&effects)
            && effects->GetEffectFromString("SHOW_TROPHY_HELD_ALOFT", effectType)) {
            C3DVector position = GetCreature()->GetPos();
            CRightHandedSet orientation;
            GetCreature()->GetDummyObject("WEAPON_FOCUS_01", &position, &orientation);
            CThing* effect = CTCDParticleEmitter::Create(effectType, *(&position), false);
            if (effect) {
                PEmitter = CIntelligentThingPointer(effect);
                CTCDParticleEmitter* emitter;
                if (((CThing*)PEmitter.GetPItem())->GetInterface(&emitter, 0x4b))
                    emitter->AttachToThing(*GetCreature(), 10, PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
            }
        }
        GetTrophy()->OnShownByCreature_006de8c0(GetCreature());
    }
}