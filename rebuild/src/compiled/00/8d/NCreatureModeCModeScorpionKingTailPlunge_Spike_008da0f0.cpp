// NCreatureMode::CModeScorpionKingTailPlunge::Spike() -- the tail plunge lands: create the spike thing at the target position for the scorpion's player, trigger its type-0x7D component, and spawn the def's spike special effect there
// Leaf: Spike
// Shape: same-TU out-only stand-ins for GetDef<CThingObjectDef> (0x0042B040), GetSubDef<CSpecialEffectsDef> and
//   GetEffectFromString: no out slot escapes, so VC7.1 packs the name/temporaries at +10h/+14h and the GetDef out at +18h
//   and releases the def from one load (`dec [ecx+4] ; jne`) as retail.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA0F0, 0x1E7 bytes (padded to 496), `ret`.
// Callees: CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0 (static
//   __fastcall), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CDefinitionManager::GetDef(long, CDefPointer*)
//   0x0042B040, CThingObject::Create 0x00703210 (static __fastcall), CVectorMap::LowerBound 0x0040F020 (inline lookup 0x7D),
//   the 0x7D component's 0x00788388 (no arguments), CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CreateEffectThing 0x006E0880 (__fastcall); def release slot 1.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge (packed): PCreature +0x84, bRecovering +0x88, TargetPos +0x89 (matches
//   retail); CModeBase ParentThing +4; the creature's player byte +0x90 (signed). The 0x1C-byte creation record built and
//   dropped at the top {0, 0, CCharString x2, false, true, true, false, 0, 0} is a local model (no PDB match pinned).
// Shape: as the landed StrikeResponseScorpionKing::Initialise 0x008C4B80 (stand-in GetSubDef / GetEffectFromString bodies
//   that only write their out slots) and ScorpionTailPlunge::PerformAction 0x008C4810 (def index from a scoped name).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CRightHandedSet;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
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
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
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
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CThingObjectDef : public CDefBase { public: };
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                    // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);            // 0x009AD410
    bool GetDef(long globalIndex, CCountedDefPointer<CThingObjectDef>* out);   // 0x0042B040
};
void* FindDef_CDefinitionManager_GetDef(const void* manager, long globalIndex);   // stand-in callee
// Same-TU stand-in for the GetDef<CThingObjectDef> instantiation (0x0042B040, a template member compiled in the caller's
// TU): it writes the out slot only, so the def pointer's address does not escape.
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long globalIndex, CCountedDefPointer<CThingObjectDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef(this, globalIndex);
    if (!def)
        return false;
    out->m_p = (CThingObjectDef*)def;
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

struct CTCSpike {                               // interface type 0x7D
    void Trigger();                             // 0x00788388
};
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char  _pad74[0x90 - 0x74];
    signed char    Player;                      // +0x90
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
class CThingCreatureBase : public CThing { public: };
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct SCreationRecord {                        // 0x1c bytes
    long        A;
    long        B;
    CCharString Name1;
    CCharString Name2;
    bool        F0, F1, F2, F3;
    long        C;
    long        D;
    SCreationRecord(const CCharString& name1, const CCharString& name2)
        : A(0), B(0), Name1(name1), Name2(name2), F0(false), F1(true), F2(true), F3(false), C(0), D(0) { }
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
    void Spike();
};
#pragma pack(pop)

void CModeScorpionKingTailPlunge::Spike()
{
    long spikeIndex;
    {
        CCharString name("OBJECT_SCORPION_KING_SPIKE");
        spikeIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    SCreationRecord record("", "");
    CCountedDefPointer<CThingObjectDef> spikeDef;
    CGameDefinitionManager::Get().GetDef(spikeIndex, &spikeDef);
    C3DVector position = TargetPos;
    CThing* spike = CThingObject::Create(spikeIndex, position, PCreature->Player, 0, 0, "");
    spike->GetTCIfPresent<CTCSpike>(0x7d)->Trigger();
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    ParentThing->PDef->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        bool found = effects->GetEffectFromString("SCORPION_KING_SPIKE", effectType);
        if (found)
            CreateEffectThing(effectType, &position, false);
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
}
}