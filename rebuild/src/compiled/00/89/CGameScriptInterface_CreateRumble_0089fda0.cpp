// CGameScriptInterface::CreateRumble(const C3DVector& pos, float strength, float radius, CCharString scriptName) const -- create a rumble at a position, give it the script name and return it as a script thing (the null script thing when nothing live was made)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089FDA0, length 120 bytes.
// Callees: the rumble factory (__fastcall: pos in ecx, the bool in dl, two floats on the stack) @ 0x007F1AC0, CCharString::operator= @ 0x0099EFB0,
//          CThing::GetScriptThing @ 0x004C7E50, CBase::CBase @ 0x0099A2D0, CCharString::~CCharString @ 0x0099EAE0 (the by-value name).
// Shape: CScriptThing returned by hidden pointer; the null script thing is built inline.
// Leaf: CreateRumble
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C3DVector { public: float x, y, z; };
struct SCountBlock;
namespace N_0099a2d0 { struct CBase {
    CBase();                                       // 0x0099A2D0 (out of line)
    virtual void v00();
}; } using N_0099a2d0::CBase;
struct CScriptThingImp;
struct CScriptThingPImp {                          // the counted pointer member: an inline constructor zeroes both words after the vtable store
    CScriptThingImp* Data;                         // +4
    SCountBlock*     Info;                         // +8
    CScriptThingPImp() : Data(0), Info(0) {}
};
class CScriptThing : public CBase { public:                      // 12 bytes, vtable 0x01238C8C (rtti-vtables.tsv)
    CScriptThingPImp PImp;
    CScriptThing() {}                              // inline: CBase::CBase, the vtable, two zero words
};
class CThing { public:
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
class CWorld; class CMainGameComponent; class CDisplayEngine; struct CPlayerManager_Retail; class CThingSearchTools;
class CTCDRumble {
public:
    static CThing* __fastcall Create(const C3DVector&, float, float, bool);   // 0x007F1AC0
};   // 0x007F1AC0
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager_Retail*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    virtual CScriptThing CreateRumble(const C3DVector& pos, float strength, float radius, CCharString scriptName) const;
};

CScriptThing CGameScriptInterface::CreateRumble(const C3DVector& pos, float strength, float radius, CCharString scriptName) const
{
    CThing* t = CTCDRumble::Create(pos, strength, radius, false);
    if (t && !t->m_dead) {
        t->ScriptName = scriptName;
        return t->GetScriptThing();
    }
    return CScriptThing();
}