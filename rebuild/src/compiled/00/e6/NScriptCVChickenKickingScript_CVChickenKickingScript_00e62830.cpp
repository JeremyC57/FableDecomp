// CV_ChickenKickingScript::CV_ChickenKickingScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012E7318, the empty cut-scene argument map and the Chicken/ChickenMaster entity handles
// Leaf: ??0CV_ChickenKickingScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E62830, length 126 bytes (padded to 128), `ret 8`.
// Catalog label "CV_BanditCampPathScript" is wrong: the factory 0x00E68E60 (`push 90h`, this ctor) is registered at
// 0x00CD8700 under "S_VCK" / "V_ChickenKicking", and the PDB CV_ChickenKickingScript layout fits the stores.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, malloc @ 0x00BFEA0E (the map's 0x18-byte header),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (x2).
// Layout (retail = PDB -0x14, then -0x18 after the 12-byte STLport map): ChickenLanded +0x48 .. InfoDisplayed +0x60
// (not initialised), csargs +0x64, seh_Chicken +0x70, seh_ChickenMaster +0x80 (vtable 0x0127094C); size 0x90.
// Shape: the landed CQ_HeroSoulsBriarScript (0x00D80C30) model.
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
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CCountedPointerWords {                     // {Data, Info}
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
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
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
class CV_ChickenKickingScript : public CQuestScriptObject {
public:   // vtable 0x012E7318
    bool ChickenLanded;                            // +0x48
    long FinalSector;                              // +0x4c
    long DistanceBand;                             // +0x50
    bool KnowGhostHasGone;                         // +0x54
    bool SpectatorsCreated;                        // +0x55
    bool RanOff;                                   // +0x56
    bool TalkedTo;                                 // +0x57
    bool SpectatorsUnderAttack;                    // +0x58
    unsigned long PrizesWon;                       // +0x5c
    bool InfoDisplayed;                            // +0x60
    CRetailMap<0x18> csargs;                       // +0x64
    CScriptGameResourceObjectScriptedThingBase seh_Chicken;         // +0x70
    CScriptGameResourceObjectScriptedThingBase seh_ChickenMaster;   // +0x80
    virtual void v00();
    CV_ChickenKickingScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CV_ChickenKickingScript;
NScript::CV_ChickenKickingScript::CV_ChickenKickingScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}