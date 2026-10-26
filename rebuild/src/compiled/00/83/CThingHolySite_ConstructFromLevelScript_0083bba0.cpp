// CThingHolySite::ConstructFromLevelScript(const CLevelScriptThingInfo& info) -- base CThingGameObject construction; on success re-fetches the def by index and reads PlayerStart from it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BBA0, length 74 bytes (vtable 0x012452DC slot 4).
// Callees: CThingGameObject::ConstructFromLevelScript @ 0x00838930 (slot 4 of the CThingGameObject vtable
// 0x01244F7C, called non-virtually), CThing::GetDefinitionManager() @ 0x004C7990, CDefinitionManager::GetDef<CThingHolySiteDef> @ 0x0083BD10.
// Shape: the argument is pushed straight from its slot (no register copy, unlike slot 3);
// edi is saved only inside the success arm (`push edi` after the test) and holds &Def (+0xd0)
// across both calls; `Def->PlayerStart != 0` into the byte at +0xd4 (`setne al`). The catalog
// label "vfunc_4" is the vtable slot; slot 4 across the CThing hierarchy is ConstructFromLevelScript.
struct CLevelScriptThingInfo;
class CThingHolySiteDef { public:
    unsigned char _pad00[0x7e];
    unsigned char PlayerStart;                             // +0x7e
};
struct CDefPointerHolySiteDef {
    CThingHolySiteDef* m_pDef;
    CThingHolySiteDef* operator->() const { return m_pDef; }
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointerHolySiteDef& out);   // 0x0083BD10
};
class CThingGameObject { public:
    virtual void v00();
    unsigned char  _pad04[0x88];
    unsigned short m_defIndex;                             // +0x8c
    unsigned char  _pad8e[0x42];
    CDefinitionManager* GetDefinitionManager();            // 0x004C7990
    virtual bool ConstructFromLevelScript(const CLevelScriptThingInfo& info);   // 0x00838930
};
class CThingHolySite : public CThingGameObject { public:
    CDefPointerHolySiteDef Def;                            // +0xd0
    bool                   PlayerStart;                    // +0xd4
    virtual bool ConstructFromLevelScript(const CLevelScriptThingInfo& info);
};

bool CThingHolySite::ConstructFromLevelScript(const CLevelScriptThingInfo& info)
{
    if (CThingGameObject::ConstructFromLevelScript(info)) {
        GetDefinitionManager()->GetDef(m_defIndex, Def);
        PlayerStart = Def->PlayerStart != 0;
        return true;
    }
    return false;
}