// CGameScriptInterface::CreatePhysicalBarrier(float radius, const C3DVector& start, const C3DVector* end, CCharString scriptName) const -- create a physical barrier between two points (one point when no end is given), give it the script name and return it as a script thing (the null script thing when nothing live was made)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089FD20, length 123 bytes.
// Callees: the barrier factory (__fastcall: start, end, float) @ 0x007FD4B0, CCharString::operator= @ 0x0099EFB0,
//          CThing::GetScriptThing @ 0x004C7E50, CBase::CBase @ 0x0099A2D0, CCharString::~CCharString @ 0x0099EAE0 (the by-value name).
// Shape: CScriptThing returned by hidden pointer; the null script thing is built inline.
// Leaf: CreatePhysicalBarrier
struct CCharString {                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct C3DVector { float x, y, z; };
struct SCountBlock;
struct CBase {
    CBase();                                       // 0x0099A2D0 (out of line)
    virtual void v00();
};
struct CScriptThingImp;
struct CScriptThingPImp {                          // the counted pointer member: an inline constructor zeroes both words after the vtable store
    CScriptThingImp* Data;                         // +4
    SCountBlock*     Info;                         // +8
    CScriptThingPImp() : Data(0), Info(0) {}
};
struct CScriptThing : CBase {                      // 12 bytes, vtable 0x01238C8C (rtti-vtables.tsv)
    CScriptThingPImp PImp;
    CScriptThing() {}                              // inline: CBase::CBase, the vtable, two zero words
};
struct CThing_Retail {
    unsigned char  _pad00[0x74];
    CCharString    ScriptName;                     // +0x74
    unsigned char  _pad78[0x91 - 0x78];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    CScriptThing GetScriptThing();                 // 0x004C7E50 (manifest "GFPredicateAnd<...>" is propagated)
};
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager_Retail {
    static CDefinitionManager_Retail& Get();       // 0x0044C6B0
};
struct CWorld; struct CMainGameComponent; struct CDisplayEngine; struct CPlayerManager_Retail; struct CThingSearchTools;
CThing_Retail* __fastcall CreatePhysicalBarrierThing(const C3DVector& start, const C3DVector& end, float radius);   // 0x007FD4B0
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager_Retail*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    virtual CScriptThing CreatePhysicalBarrier(float radius, const C3DVector& start, const C3DVector* end, CCharString scriptName) const;
};

CScriptThing CGameScriptInterface::CreatePhysicalBarrier(float radius, const C3DVector& start, const C3DVector* end, CCharString scriptName) const
{
    CThing_Retail* t = CreatePhysicalBarrierThing(start, end ? *end : start, radius);
    if (t && !t->m_dead) {
        t->ScriptName = scriptName;
        return t->GetScriptThing();
    }
    return CScriptThing();
}
