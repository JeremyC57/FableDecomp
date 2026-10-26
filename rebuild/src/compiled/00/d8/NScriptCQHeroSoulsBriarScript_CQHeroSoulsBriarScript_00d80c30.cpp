// CQ_HeroSoulsBriarScript::CQ_HeroSoulsBriarScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, own vtable, the empty Briar Rose entity handle, the empty cut-scene argument map and the two imposter timers
// Leaf: ??0CQ_HeroSoulsBriarScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80C30, length 135 bytes (padded to 144), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (catalog label "CHeroMorphDef" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (the handle's base), malloc @ 0x00BFEA0E (the map's
// 0x18-byte header), CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the global @ 0x0143E8F8 (x2).
// Vtable 0x012D41BC (slot 3 = Init 0x00D80CC0). Layout (retail; PDB offsets -0x14, the retail STLport map is 12 bytes):
// PlayedBriarCS +0x48, seh_BriarRose (CScriptGameResourceObjectScriptedThingBase, vtable 0x0127094C) +0x4c,
// csargs map +0x5c, BriarRoseImposterTauntTimer +0x68, BriarRoseImposterHereTimer +0x6c, KickOffQuestStartScreen +0x70.
// Shape (landed 0x00D2F6B0 / 0x00E299F0 models): the argument words before the vtable store come from the inline
// `__declspec(novtable)` intermediate base; the handle's counted-pointer words come from an embedded {Data, Info}
// member object (one `xor ebx,ebx` shared with the map's zero stores); the timers' inline ctors register them.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
class CBaseObject { public:                               // CBaseIntelligentPointer root (+4 nulled by its ctor)
    void* m_pIntelligentPointer;                   // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00();
};
namespace NScript {
class CQ_HeroSoulsBriarScript : public CQuestScriptObject {
public:   // vtable 0x012D41BC
    bool             PlayedBriarCS;                // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;   // +0x4c
    CRetailMap<0x18> csargs;                       // +0x5c
    CTimer           BriarRoseImposterTauntTimer;  // +0x68
    CTimer           BriarRoseImposterHereTimer;   // +0x6c
    bool             KickOffQuestStartScreen;      // +0x70
    CQ_HeroSoulsBriarScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_HeroSoulsBriarScript;
NScript::CQ_HeroSoulsBriarScript::CQ_HeroSoulsBriarScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}