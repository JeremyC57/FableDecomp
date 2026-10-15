// CThingObject::SerialiseGameState(CPersistContext& context) -- base CThingPhysical serialisation; unless the context mode is 1 or 3, re-fetches the object def by its global index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007030C0, length 74 bytes (vtable 0x01264DCC slot 3).
// Callees: CThingPhysical::SerialiseGameState @ 0x006A5AF0 (called non-virtually),
// CThing::GetDefinitionManager() @ 0x004C7990 (manifest "CViewBase::GetDisplayEngine" is propagated: the body
// reads the world's +0x20 manager and never uses `this`), CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer&) @ 0x0042B040 (manifest
// "GetDef<CSpecialAbilitiesDrunkennessDef>": one body of the shared GetDef template).
// Shape: the 0x0083BB40 CThingHolySite twin without the trailing def read: the context stays in
// edi for the mode test; the GetDef arguments (the 16-bit def index at +0x8c, &PDef at +0xd0)
// are pushed before the manager call (VC evaluates arguments before the object expression).
// The catalog label "vfunc_3" is the vtable slot (the base call is CThingPhysical::SerialiseGameState).
struct CPersistContext {
    unsigned char _pad00[0x18];
    long          m_mode;                                  // +0x18
};
struct CThingObjectDef;
struct CDefPointerThingObjectDef {
    CThingObjectDef* m_pDef;
    CThingObjectDef* operator->() const { return m_pDef; }
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointerThingObjectDef& out);   // 0x0042B040
};
struct CThingPhysical {
    virtual void v00();
    unsigned char  _pad04[0x88];
    unsigned short DefGlobalIndex;                             // +0x8c
    unsigned char  _pad8e[0x42];
    CDefinitionManager* GetDefinitionManager();            // 0x004C7990
    virtual bool SerialiseGameState(CPersistContext& context);   // 0x006A5AF0
};
struct CThingObject : CThingPhysical {
    CDefPointerThingObjectDef PDef;                        // +0xd0
    virtual bool SerialiseGameState(CPersistContext& context);
};

bool CThingObject::SerialiseGameState(CPersistContext& context)
{
    if (CThingPhysical::SerialiseGameState(context)) {
        if (context.m_mode != 1 && context.m_mode != 3) {
            GetDefinitionManager()->GetDef(DefGlobalIndex, PDef);
        }
        return true;
    }
    return false;
}
