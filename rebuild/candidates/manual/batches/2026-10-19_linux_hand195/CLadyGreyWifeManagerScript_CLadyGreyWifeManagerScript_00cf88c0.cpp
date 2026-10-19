// CLadyGreyWifeManagerScript::CLadyGreyWifeManagerScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012C6C10, five CScriptThing handles and the two region names
// Leaf: ??0CLadyGreyWifeManagerScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CF88C0, length 172 bytes (`ret 8`).
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBaseClass::CBaseClass @ 0x0099A2D0 (each CScriptThing), CCharString::CCharString() @ 0x0099E4B0.
// Layout: Ego_r.pdb CLadyGreyWifeManagerScript (0xA8) with retail = PDB - 0x14 (the 0x40-byte retail script base): LadyGrey +0x48,
// BowerstoneNorthVillage +0x54, MayorsBedroomVillage +0x60, MayorsBedroom +0x6C, AppearPoint +0x78, CurrentTime +0x84 (no ctor),
// NewRegionEntered +0x88 (no ctor), InteriorRegion +0x8C, ExteriorRegion +0x90. Model shared with the landed quest-script ctors
// (CQ_HangingTreeEvilScript 0x00D68F00): the two argument stores before the vtable store are the inlined `__declspec(novtable)`
// argument-carrying intermediate base; each CScriptThing is CBaseClass + vtable 0x01238C8C + an embedded zeroed CCountedPointer.
struct CGameScriptInterface;
struct CScriptBase {                                    // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04..+0x3f
    CScriptBase();                                      // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                         // +0x40 (second argument)
    void*                 Data;                         // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CCharString { void* PStringData; CCharString(); };   // default ctor 0x0099E4B0
struct CBaseClass { virtual ~CBaseClass(); CBaseClass(); };   // 0x0099A2D0
template<class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    virtual ~CScriptThing();
};
struct CLadyGreyWifeManagerScript : CQuestScriptObject {
    CScriptThing LadyGrey;                              // +0x48
    CScriptThing BowerstoneNorthVillage;                // +0x54
    CScriptThing MayorsBedroomVillage;                  // +0x60
    CScriptThing MayorsBedroom;                         // +0x6c
    CScriptThing AppearPoint;                           // +0x78
    long CurrentTime;                                   // +0x84
    bool NewRegionEntered;                              // +0x88
    CCharString InteriorRegion;                         // +0x8c
    CCharString ExteriorRegion;                         // +0x90
    virtual ~CLadyGreyWifeManagerScript();
    CLadyGreyWifeManagerScript(void* data, CGameScriptInterface* game);
};
CLadyGreyWifeManagerScript::CLadyGreyWifeManagerScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
