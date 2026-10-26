// NCreatureMode::CModeScorpionKingTailPlunge::Recover() -- end the tail plunge: kill the live tail emitter, spawn the def's TAIL_DUST_EFFECT special effect at the tail position, kill the creature's dependent things (interface 0xAD) and set a CCreatureAction_ScorpionTailWithdraw towards the target as the creature's current action
// Leaf: ?Recover@CModeScorpionKingTailPlunge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DF510, 304 bytes incl. int3 padding, `ret` (private, thiscall).
// Identity: catalog "CTCLightningOrb_OnUpdate" (_global) is wrong: the PDB name set's
//   `?Recover@CModeScorpionKingTailPlunge@NCreatureMode@@AAEXXZ`, called from the landed FrameUpdate 0x008E0D20 (+0x3AE),
//   which already names 0x008DF510 Recover.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill(bool) 0x004C9B80, CDefClassBase::GetSubDef<
//   CSpecialEffectsDef> 0x00669580 (catalog "<CAICreatureWillPowerIndicatorDef>" propagated), CSpecialEffectsDef::
//   GetEffectFromString 0x00669D30 (catalog "CCRC::CalcAndLookup"), CCharString 0x0099EBF0 / ~ 0x0099EAE0, CreateEffectThing
//   0x006E0880 (__fastcall), CVectorMap::LowerBound 0x0040F020 (inline GetTC 0xAD), the 0xAD component's 0x007AEF60
//   (catalog "CTCBoss::ClearEffects"; it kills every live thing of its 8-byte intelligent-pointer vector +0x3C/+0x40:
//   descriptive KillDependentThings), CCreatureAction_ScorpionTailWithdraw ctor 0x008E45B0 (catalog
//   "CCreatureAction_BriarRoseShotFarInto"; it stores vtable 0x01286334 = RTTI ScorpionTailWithdraw),
//   CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900 (catalog "~CTCLightningOrb"); the
//   effects def's virtual slot 1 on the last release.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge (packed, as the landed FrameUpdate / Spike): PCreature +0x84, TailPos +0x95,
//   PTarget +0xA1, PTailEmitter +0xA9; CModeBase ParentThing +4; CThing def +0x70, dead bit 0 of +0x91. Retail
//   CCreatureAction_ScorpionTailWithdraw is 0x120 bytes (CCombatActionBase 0x114 + C3DVector TargetPos; Ego_r 0x138).
// Shape: the landed Spike 0x008DA0F0 model: same-TU out-only stand-ins for GetSubDef / GetEffectFromString (the effects
//   def lives in ebp after the call, its slot reused by the TAIL_DUST_EFFECT temporary), a counted out pointer released at
//   the function end, GetTCIfPresent (`xor ecx,ecx` default) and IS_THING_ALIVE.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();                     // slot 0
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template <class T> struct CCountedDefPointer {
    T* m_p;
    CCountedDefPointer() : m_p(0) { }
    ~CCountedDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CCountedDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CCountedDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCAD {                                  // interface type 0xAD (descriptive: the dependent-things component)
    void KillDependentThings();                 // 0x007AEF60
};
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    void Kill(bool immediately);                // 0x004C9B80
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_ScorpionTailWithdraw : public CCombatActionBase { public:   // retail sizeof 0x120
    C3DVector TargetPos;                        // +0x114 (retail sizeof 0x120; Ego_r +0x12C of 0x138)
    CCreatureAction_ScorpionTailWithdraw(CThingCreatureBase* creature, CThing* target);   // 0x008E45B0
};

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                        // +0x04
};
namespace NCreatureMode {
#pragma pack(push, 1)
class CModeScorpionKingTailPlunge : public CModeBase { public:
    unsigned char       _pad08[0x84 - 0x08];
    CThingCreatureBase* PCreature;              // +0x84
    bool                bRecovering;            // +0x88
    C3DVector           TargetPos;              // +0x89
    C3DVector           TailPos;                // +0x95
    CBaseIntelligentPointer PTarget;            // +0xa1
    CBaseIntelligentPointer PTailEmitter;       // +0xa9
    CBaseIntelligentPointer PDustTrailEmitter;  // +0xb1
    void Recover();
};
#pragma pack(pop)

void CModeScorpionKingTailPlunge::Recover()
{
    if (IS_THING_ALIVE(PTailEmitter))
        PTailEmitter.GetPItem()->Kill(true);
    CCountedDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    ParentThing->PDef->GetSubDef(effects);
    if (effects.m_p) {
        if (effects.m_p->GetEffectFromString("TAIL_DUST_EFFECT", effectType))
            CreateEffectThing(effectType, &TailPos, false);
    }
    PCreature->GetTCIfPresent<CTCAD>(0xad)->KillDependentThings();
    PCreature->SetCurrentAction(CCreatureAction_ScorpionTailWithdraw(PCreature, PTarget.GetPItem()));
}
}