// CThingObject::ConstructFromLevelScript(const CLevelScriptThingInfo& info) -- base CThingGameObject construction; on success re-fetches the object def by its global index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703110, length 58 bytes (vtable 0x01264DCC slot 4).
// Callees: CThingGameObject::ConstructFromLevelScript @ 0x00838930 (slot 4 of the CThingGameObject vtable
// 0x01244F7C, called non-virtually), CThing::GetDefinitionManager() @ 0x004C7990, CDefinitionManager::GetDef<CThingObjectDef> @ 0x0042B040.
// Shape: the 0x0083BBA0 CThingObject twin without the def read: the argument is pushed
// straight from its slot, &PDef (+0xd0) is a scratch register (no edi save). The catalog label
// "vfunc_4" is the vtable slot; slot 4 across the CThing hierarchy is ConstructFromLevelScript.
struct CLevelScriptThingInfo;
struct CThingObjectDef;
struct CDefPointerThingObjectDef {
    CThingObjectDef* m_pDef;
    CThingObjectDef* operator->() const { return m_pDef; }
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointerThingObjectDef& out);   // 0x0042B040
};
struct CThingGameObject {
    virtual void v00();
    unsigned char  _pad04[0x88];
    unsigned short DefGlobalIndex;                             // +0x8c
    unsigned char  _pad8e[0x42];
    CDefinitionManager* GetDefinitionManager();            // 0x004C7990
    virtual bool ConstructFromLevelScript(const CLevelScriptThingInfo& info);   // 0x00838930
};
struct CThingObject : CThingGameObject {
    CDefPointerThingObjectDef PDef;                        // +0xd0
    virtual bool ConstructFromLevelScript(const CLevelScriptThingInfo& info);
};

bool CThingObject::ConstructFromLevelScript(const CLevelScriptThingInfo& info)
{
    if (CThingGameObject::ConstructFromLevelScript(info)) {
        GetDefinitionManager()->GetDef(DefGlobalIndex, PDef);
        return true;
    }
    return false;
}
