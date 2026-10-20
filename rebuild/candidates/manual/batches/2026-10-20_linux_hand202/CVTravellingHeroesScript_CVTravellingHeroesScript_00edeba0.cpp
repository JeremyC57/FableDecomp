// CV_TravellingHeroesScript::CV_TravellingHeroesScript(CScriptDataBase*, CGameScriptInterface&) -- quest-script ctor: script base, interface/data pair, own vtable, the three hero handles and the empty cutscene-argument map
// Leaf: ??0CV_TravellingHeroesScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDEBA0, length 161 bytes, `ret 8`; vtable 0x012F407C; built by the factory
// 0x00EE40A0 (registered as "V_TravellingHeroes" / "S_VTH", `push 8Ch` = Ego_r.pdb 0xa4 - 0x18). The catalog label
// CQ_WizardBattleScript::CQ_WizardBattleScript is propagated (that ctor is the landed 0x00E1A800).
// Callees: CScriptBase() 0x00CB8110, CBaseObject() 0x0099A380 (each handle's root), malloc 0x00BFEA0E (the map's header node).
// Layout (Ego_r.pdb - 0x14, 12-byte retail map): seh_Rival +0x48, seh_Girl +0x58, seh_Boy +0x68
// (CScriptGameResourceObjectScriptedThingBase, vtable 0x0127094C), the six cutscene flags +0x78..+0x7d, csargs
// (map<CCharString, CCharString>, 0x18-byte nodes) +0x80.
// Shape: the novtable intermediate base stores the argument pair before the vtable; the handles' inline ctors zero their
// counted pointers from one ebx, which also clears the map header word before the malloc and the node count after it
// (the landed CQ_WizardBattleScript map model).
struct CCharString {                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
    unsigned char _body[0xc];
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    ~CScriptThing();                                     // 0x004AA840
};

struct CIsThingConscious {                               // NScript predicate, vtable 0x012C2FE8: Thing alive and not unconscious
    CScriptThing Thing;                                  // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) {}
    ~CIsThingConscious() {}
    virtual bool operator()() const;                     // slot 0 (0x00CDDAB0)
};

struct CBaseObject {                                     // vtable 0x0129A7D8 {vptr, intelligent-pointer link}
    void* IntelligentPointer;                            // +0x04
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};

struct CScriptGameResourceObjectBase : CBaseObject {     // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};

struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // 16 bytes, vtable 0x0127094C
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    virtual void v01();                                  // slot 1 (0x007E72C0)
};
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E (MSVCR71)

struct _Rb_tree_node_base {
    char                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template <unsigned int NodeSize> struct map : _Rb_tree_base<NodeSize> {   // STLport map / _Rb_tree (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    map() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
struct CGameScriptInterface {                           // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c) 0x006E7410
};
struct CScriptDataBase;

struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers), vtable 0x012C1648
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    CScriptBase();                                       // 0x00CB8110
};

struct __declspec(novtable) CQuestScriptBase : CScriptBase {   // the argument pair stored before the derived vtable
    CGameScriptInterface* GSI;                           // +0x40
    CScriptDataBase*      Data;                          // +0x44
    CQuestScriptBase(CScriptDataBase* data, CGameScriptInterface* gsi) : CScriptBase(), GSI(gsi), Data(data) {}
};

struct CV_TravellingHeroesScript : CQuestScriptBase {    // vtable 0x012F407C
    CScriptGameResourceObjectScriptedThingBase seh_Rival;   // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_Girl;    // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Boy;     // +0x68
    bool ThunderKidsTalking;                             // +0x78
    bool ThunderSceneStarted;                            // +0x79
    bool ThunderSceneDone;                               // +0x7a
    bool MazeCutsceneDone;                               // +0x7b
    bool WhisperCutscenePlayed;                          // +0x7c
    bool SnowTrollSceneDone;                             // +0x7d
    map<0x18> csargs;                                    // +0x80 map<CCharString, CCharString>
    virtual void v00();
    CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterface& gsi);
};

CV_TravellingHeroesScript::CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterface& gsi)
    : CQuestScriptBase(data, &gsi)
{
}
