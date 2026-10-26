// CQ_SecretPassageScript::CQ_SecretPassageScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012DDC30, the two empty string-keyed maps (GlobalMymap, FlagMap), the eleven minion handles and the Mother handle
// Leaf: ??0CQ_SecretPassageScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEC7F0, 175 bytes, `ret 8`. The catalog label "CHeroMorphDef::CHeroMorphDef" is
//   propagated: the only caller is the factory 0x00DF1080 (`push 12Ch`, operator new 0x00BFEA1A, then this ctor), which
//   0x00CD74E8 registers under "S_QSP" / "Q_SecretPassage" (string 0x012DDC44).
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, malloc @ 0x00BFEA0E (the maps' 0x24- and 0x18-byte header nodes),
//   CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (x12, vtable 0x0127094C
//   CScriptGameResourceObjectScriptedThingBase).
// Layout (Ego_r.pdb CQ_SecretPassageScript, 0x148 bytes; retail 0x12C = PDB - 0x14 for the script base, then -4 per
//   12-byte STLport map): MissionSucceeded..QuestStartScreenShown +0x48..+0x4f and GhostSightings +0x50 (not
//   initialised), GlobalMymap +0x54, FlagMap +0x60, minions[11] +0x6c, seh_Mother +0x11c.
// Shape: the landed quest-script model (CV_ChickenKickingScript 0x00E62830, CQ_HangingTreeEvilScript 0x00D68F00): the
//   argument pair stored by the `__declspec(novtable)` intermediate base before the vtable store, each map's header
//   cleared, allocated and linked to itself (header re-read for each link), the minion array as an inline counted
//   loop (`mov ebp,0Bh ... dec ebp ; jne`, the element has no destructor), then seh_Mother.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CGameScriptInterface;

class CScriptBase { public:                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};

struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};

struct CCountedPointerWords {                      // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

class CBaseObject { public:                               // CBaseIntelligentPointer root
    virtual void v00();
    void* m_pIntelligentPointer;                   // +0x04
    CBaseObject();                                 // 0x0099A380
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00();
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

namespace NScript {
class CQ_SecretPassageScript : public CQuestScriptObject {
public:   // vtable 0x012DDC30
    bool MissionSucceeded;                         // +0x48
    bool MissionFailed;                            // +0x49
    bool MissionOver;                              // +0x4a
    bool MotherFound;                              // +0x4b
    bool DoorsBlocked;                             // +0x4c
    bool DoorsBeingDone;                           // +0x4d
    bool OkayToSpawn;                              // +0x4e
    bool QuestStartScreenShown;                    // +0x4f
    long GhostSightings;                           // +0x50
    CRetailMap<0x24> GlobalMymap;                  // +0x54 map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    CRetailMap<0x18> FlagMap;                      // +0x60 map<CCharString, bool>
    CScriptGameResourceObjectScriptedThingBase minions[11];   // +0x6c
    CScriptGameResourceObjectScriptedThingBase seh_Mother;   // +0x11c
    virtual void v00();
    CQ_SecretPassageScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_SecretPassageScript;

NScript::CQ_SecretPassageScript::CQ_SecretPassageScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}