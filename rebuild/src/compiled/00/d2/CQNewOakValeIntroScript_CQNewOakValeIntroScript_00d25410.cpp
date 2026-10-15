// CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, own vtable, six zeroed counters, the empty map and two timers
// Leaf: ??0CQ_NewOakValeIntroScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D25410, length 153 bytes (padded to 160), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (catalog label "CHeroMorphDef" is propagated), malloc @ 0x00BFEA0E
// (the map's 0x18-byte header), CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the global @ 0x0143E8F8 (x2).
// Vtable 0x012CB70C (slot 3 = Init 0x00D254B0). The retail class does not follow the Ego_r PDB layout of
// CQ_NewOakValeIntroScript past +0x4c (it has a 12-byte map at +0x84 and timers at +0x90/+0x94), so the members are
// named descriptively: six zero-initialised words +0x4c..+0x60, the map +0x84, two timers +0x90/+0x94.
// Shape (landed 0x00E299F0 model): argument words before the vtable store from the inline `__declspec(novtable)`
// intermediate base; the six zero words come from the inline ctor of an embedded member object (one `xor ebx,ebx`
// after the vtable store, shared with the map; plain init-list zeros hoist it above the store); the map is STLport's
// _Rb_tree; the timers' inline ctors register them.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C
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
struct CTimer {                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
struct CScriptBase {                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
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
struct CZeroWords {                                // six counters zeroed by their own inline ctor (an init-list zero on the
    long Word[6];                                  // script itself hoists the zero register above the vtable store)
    CZeroWords() { Word[0] = 0; Word[1] = 0; Word[2] = 0; Word[3] = 0; Word[4] = 0; Word[5] = 0; }
};
struct CQ_NewOakValeIntroScript : CQuestScriptObject {   // vtable 0x012CB70C
    unsigned char    _pad48[4];                    // +0x48 (VictimShake)
    CZeroWords       m_counters;                   // +0x4c..+0x63
    unsigned char    _pad64[0x84 - 0x64];          // +0x64
    CRetailMap<0x18> m_map;                        // +0x84
    CTimer           m_timer90;                    // +0x90
    CTimer           m_timer94;                    // +0x94
    CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game);
};
CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}