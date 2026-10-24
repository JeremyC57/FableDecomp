// CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(CScriptDataBase* data, CGameScriptInterfaceBase* iface) -- quest-script constructor: CScriptBase(), the data/interface pair at +0x44/+0x40, own vtable, three cleared words and a timer registered with the game script interface
// Leaf: ??0CV_SwordInTheStoneScript@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF5C90, length 62 bytes.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (the manifest's "CHeroMorphDef" label is propagated),
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8 (the landed
// naming). Vtable 0x012DE6A4 (no RTTI, quest-script module); the manifest label CV_SwordInTheStoneScript is
// kept (Init @ 0x00EB6A00 and the two watchers in this chunk work on the same layout: +0x48..+0x51, +0x60, +0x7c).
// Shape (landed 0x00CE1A00 model): the two arguments stored before the derived vtable store belong to the
// inlined `__declspec(novtable)` intermediate base; the three zero stores after it come from an embedded state member's inline constructor (plain
// init-list zeros hoist the `xor eax,eax` above the vtable store);
// the body registers the timer into +0x6c; `ret 8`, `this` in eax.
struct CScriptBase {                               // 0x40 bytes, the quest-script root (landed 0x00CB8110)
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {   // the argument-carrying intermediate base: both words stored before the derived vtable
    void* m_pInterface;                            // +0x40 (second argument, CGameScriptInterfaceBase*)
    void* m_pData;                                 // +0x44 (first argument, CScriptDataBase*)
    CScriptObject(void* data, void* iface) : CScriptBase(), m_pInterface(iface), m_pData(data) { }
};

struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                  // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

struct SSwordState {                               // +0x48..+0x53: a member object so the zero register is set after the vtable store
    long m_swordState;                             // +0x48
    long m_crowdState;                             // +0x4c
    long m_pullCount;                              // +0x50
    SSwordState() : m_swordState(0), m_crowdState(0), m_pullCount(0) { }
};

struct CV_SwordInTheStoneScript : CScriptObject {  // vtable 0x012DE6A4
    SSwordState   m_state;                         // +0x48
    unsigned char _pad54[0x18];                    // +0x54..+0x6b
    long          m_crowdTimer;                    // +0x6c
    virtual void v00();
    CV_SwordInTheStoneScript(void* data, void* iface);
};

CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(void* data, void* iface)
    : CScriptObject(data, iface), m_state()
{
    m_crowdTimer = g_pGameScriptInterface->RegisterTimer();
}