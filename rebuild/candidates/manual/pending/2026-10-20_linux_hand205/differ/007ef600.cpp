// CTCActionUseActivateQuest::OnActionUse(CThing& user) -- base OnActionUse; a live creature user gets the action of activating the def's quest script
// Grade: DIFFER(ours v retail) -- frame 0xc0 v 0xbc (GetSubDef out word not shared with the script-name string), user/def in ebx/esi v esi/ebx, def release mov edx,[esi+4];lea;dec;test;mov v dec [ebx+4];jne
// Leaf: ?OnActionUse@CTCActionUseActivateQuest@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EF600, length 187 bytes. Vtable slot 28 ("vfunc_28" in the catalog), `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified; manifest "$E2"), CDefClassBase::GetSubDef<CActivateQuestDef>
// @ 0x007B5680 (on the thing's def, +0x70; manifest instantiation name is a byte twin), CDefStringTable::GetString(long)
// @ 0x009D49B0 on g_DefStringTable 0x013CA828, CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest @ 0x00843F50,
// CThingCreatureBase::SetCurrentAction @ 0x006644F0 (result dropped), ~CBaseIntelligentPointer @ 0x00A01C10,
// ~CCharString @ 0x0099EAE0, ~CCreatureActionBase @ 0x00693EF0, the def's virtual Delete (slot 1).
// Layout: Ego_r.pdb CActivateQuestDef: ScriptName (CDefString index) +0x28, LoadResources +0x2C. CCreatureAction_ActivateQuest
// is PDB 0xC4 bytes with ScriptName +0xB4, LoadResources +0xB8, PObjectToUse +0xB9; retail CCreatureActionBase is 0xA8 (PDB
// 0xB4), so the members sit at +0xA8/+0xAC/+0xAD (the intelligent pointer is unaligned: declared packed).
// Shape: the GetSubDef out pointer lives in a `__forceinline` accessor, so its frame word is reused by the script-name string
// (both at esp+0xC); the def is held in ebx and released at the end with no null test (a null def returns first);
// the action temporary is passed by reference straight from the ctor's eax and destroyed inline with no vtable store
// (novtable leaf): PObjectToUse, ScriptName, then the base.
struct CCharString {
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};
class CThing {
public:
    unsigned char _pad00[0x70];
    struct CDefClassBase* PDef;                     // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                          // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
inline CThingCreatureBase* CastToCreature(CThing* thing) { return (CThingCreatureBase*)thing; }

struct CDefPointeeBase {
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    void Release() { if (--m_refCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CActivateQuestDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long ScriptName;                                // +0x28 CDefString (string-table index)
    bool LoadResources;                             // +0x2c
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CActivateQuestDef>* out) const;   // 0x007B5680
};
struct CDefStringTable {
    CCharString GetString(long index);              // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                    // unaligned inside the action (+0xad)
    void* __vftable;
    void* m_pItem;
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};
#pragma pack(pop)
class CCreatureActionBase {                         // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();                 // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class __declspec(novtable) CCreatureAction_ActivateQuest : public CCreatureActionBase {
public:
    CCharString ScriptName;                         // +0xa8
    bool        LoadResources;                      // +0xac
    CBaseIntelligentPointer PObjectToUse;           // +0xad
    CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing& objectToUse,
                                  const CCharString& scriptName, bool loadResources);   // 0x00843F50
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class CTCActionUseBase : public CTCBase {
public:
    const void* PActionUseDef;                      // +0x0c
    bool Usable;                                    // +0x10
    virtual void OnActionUse(CThing& user);         // 0x007EAB90
};
class CTCActionUseActivateQuest : public CTCActionUseBase {
public:
    __forceinline CActivateQuestDef* GetActivateQuestDef() const
    {
        CDefPointer<CActivateQuestDef> out;
        ParentThing->PDef->GetSubDef(&out);
        return out.Object;
    }
    virtual void OnActionUse(CThing& user);
};

void CTCActionUseActivateQuest::OnActionUse(CThing& user)
{
    CThingCreatureBase* creature = CastToCreature(&user);
    CTCActionUseBase::OnActionUse(user);
    CActivateQuestDef* def;
    {
        CDefPointer<CActivateQuestDef> out;
        ParentThing->PDef->GetSubDef(&out);
        def = out.Object;
    }
    if (!def)
        return;
    if (creature && !creature->IsDead()) {
        CCharString scriptName = g_DefStringTable.GetString(def->ScriptName);
        creature->SetCurrentAction(CCreatureAction_ActivateQuest(*creature, *ParentThing, scriptName, def->LoadResources));
    }
    if (--def->m_refCount == 0)
        def->Delete();
}
