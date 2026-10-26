// CQ_WizardBattleScript::CQ_WizardBattleScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, own vtable and three empty CScriptThing members (Maze, TopOfLighthouse, MiddleOfLighthouse)
// Leaf: ??0CQ_WizardBattleScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1A800, length 106 bytes (padded to 112), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (catalog label "CHeroMorphDef" is propagated), CBase::CBase @ 0x0099A2D0
// (each CScriptThing's base, x3).
// Vtable 0x012E0F54 (slot 3 = Init 0x00E1A870, landed). Layout (retail; PDB offsets -0x14): the flags +0x48..+0x4f,
// MazePhase +0x50, NumberOfScreamersAlive +0x54, MazeConversation +0x58, Maze +0x5c, TopOfLighthouse +0x68,
// MiddleOfLighthouse +0x74 (CScriptThing, vtable 0x01238C8C, 12 bytes).
// Shape (landed 0x00D2F6B0 model): argument words before the vtable store from the inline `__declspec(novtable)`
// intermediate base; each CScriptThing's base ctor runs on `lea edi,[esi+N]`, its vtable is stored and its counted-pointer
// words are zeroed from one `xor ebx,ebx` by the inline ctor of an embedded {Data, Info} member object.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CGameScriptInterface { public:                      // retail vtable @ 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
class CTimer { public:                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
class CScriptBase { public:                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base: both words stored before the derived vtable
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct _Rb_tree_node_base {
    char                _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template <unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map/_Rb_tree (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
struct CCountedPointerWords {                     // {Data, Info}: a member object so its zero stores follow the owner's vtable store
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
};
class CScriptThing : public CBaseClass { public:                 // vtable 0x01238C8C, 12 bytes
    CCountedPointerWords PImp;                     // +0x04 Data, +0x08 Info
    CScriptThing() : CBaseClass(), PImp() { }
    virtual void v00();
};
struct CQ_WizardBattleScript : CQuestScriptObject {   // vtable 0x012E0F54
    bool         VillagersTurnOnHero;              // +0x48
    bool         VillagersChased;                  // +0x49
    bool         FightersStart;                    // +0x4a
    bool         MissionSucceeded;                 // +0x4b
    bool         MissionFailed;                    // +0x4c
    bool         MissionOver;                      // +0x4d
    bool         QuestStartScreenShown;            // +0x4e
    bool         MazeDefeated;                     // +0x4f
    long         MazePhase;                        // +0x50
    long         NumberOfScreamersAlive;           // +0x54
    long         MazeConversation;                 // +0x58
    CScriptThing Maze;                             // +0x5c
    CScriptThing TopOfLighthouse;                  // +0x68
    CScriptThing MiddleOfLighthouse;               // +0x74
    CQ_WizardBattleScript(void* data, CGameScriptInterface* game);
};
CQ_WizardBattleScript::CQ_WizardBattleScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}