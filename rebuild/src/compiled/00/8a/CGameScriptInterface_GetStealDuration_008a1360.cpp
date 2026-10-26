// CGameScriptInterface::GetStealDuration(const CScriptThing& thing) const -- for a stealable thing (component 0x33) whose item has a stealable sub-def and whose own def has a creature-stats sub-def: the steal duration interpolated linearly from MinDuration (skill 0) to MaxDuration (skill MaxStealSkill), clamped to MaxDuration above MaxStealSkill; 2 otherwise
// Leaf: GetStealDuration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A1360, length 553 bytes (virtual, `ret 4`).
// Callees: CScriptThing slot 11 GetThing, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0, GetPItem 0x00A01B50,
//   CDefClassBase::GetSubDef 0x005130A0 / 0x0059350C (same-TU auto_inline(off) stand-ins that write only the out slot), __ftol2 0x00BFEA70, the defs' slot 1.
// Layout: CThing presence bits +0x20, component map +0x44 (as the landed 0x007BB150), def pointer +0x70; the stealable component's item
//   intelligent pointer at the packed offset +0x0D (as CTCTrespasser +0x2D); the stealable def's MaxStealSkill/MinDuration/MaxDuration at +0xA4/A8/AC
//   and the stats def's skill (int) at +0x28 are read from this body (no PDB match checked).
// Shape: `itemDef && itemDef->GetSubDef(...)` with the sub-def declared before the test; the thing's def is re-fetched twice through an inline
//   by-value GetDef() in one && condition (the two conditionally destroyed temporaries keep retail's flag word); the three floats are named
//   locals in retail's load order (Min, Max, then the skill cap).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;   // 0x004365B0 (const instantiation)
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};
struct CStealableDef : CDefBase {
    unsigned char _pad08[0xa4 - 0x08];
    float MaxStealSkill;                        // +0xa4
    float MinDuration;                          // +0xa8
    float MaxDuration;                          // +0xac
};
class CCreatureStatsDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long StealSkill;                            // +0x28
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_005130a0(CDefPointer<CStealableDef>& out);     // 0x005130A0
    bool GetSubDef_0059350c(CDefPointer<CCreatureStatsDef>& out); // 0x0059350C
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_005130a0(CDefPointer<CStealableDef>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (CStealableDef*)def;
    return true;
}
__declspec(noinline) bool CDefClassBase::GetSubDef_0059350c(CDefPointer<CCreatureStatsDef>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (CCreatureStatsDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> Def;             // +0x70
    template<class T> __forceinline const T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    CDefPointer<CDefClassBase> GetDef() const { return Def; }
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(push, 1)
struct CTCStealable {
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer Item;               // +0x0d
};
#pragma pack(pop)
class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;           // slot 11 (+0x2c)
};
class CWorld; class CMainGameComponent; class CDisplayEngine; class CDefinitionManager; class CPlayerManager;
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                 World;              // +0x04
    virtual long GetStealDuration(const CScriptThing& thing) const;
};

long CGameScriptInterface::GetStealDuration(const CScriptThing& thing) const
{
    CThing* t = thing.GetThing();
    if (t && t->HasTC(0x33)) {
        const CTCStealable* stealable = t->GetTC<CTCStealable>(0x33);
        if (stealable->Item.GetPItem()) {
            CDefPointer<CDefClassBase> itemDef = stealable->Item.GetPItem()->GetDef();
            CDefPointer<CStealableDef> stealDef;
            if (itemDef && itemDef->GetSubDef_005130a0(stealDef)) {
                float minDuration = stealDef->MinDuration;
                float maxDuration = stealDef->MaxDuration;
                float maxSkill = stealDef->MaxStealSkill;
                CDefPointer<CCreatureStatsDef> stats;
                if (t->GetDef() && t->GetDef()->GetSubDef_0059350c(stats)) {
                    float skill = (float)stats->StealSkill;
                    if (skill > maxSkill)
                        return (long)maxDuration;
                    return (long)((maxDuration - minDuration) / maxSkill * skill + minDuration);
                }
            }
        }
    }
    return 2;
}