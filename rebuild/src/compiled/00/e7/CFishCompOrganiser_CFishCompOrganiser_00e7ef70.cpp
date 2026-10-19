// CFishCompOrganiser::CFishCompOrganiser(CScriptOwner* parent, void* masterData, const CScriptThing& me) -- the fishing-competition organiser's entity script: entity-script base over the quest's interface and Me, the hero and organiser handles, the empty cut-scene argument map, the competition-entry string and the two empty def-name vectors, the master data last
// Leaf: ??0CFishCompOrganiser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7EF70, 169 bytes, `ret 0Ch`. The catalog label "CGameScriptThing" is propagated:
//   the only caller is the factory 0x00E7EEF0 (`push 78h`, operator new, this ctor), registered at 0x00E7E516 under
//   "FishCompOrganiser"; vtable 0x012EA620 over the entity-script base vtable 0x012C3224.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CBaseIntelligentPointer::CBaseIntelligentPointer
//   @ 0x0099A380 (x2, vtable 0x0127094C), malloc @ 0x00BFEA0E (the map's 0x18-byte header), CCharString::CCharString() @
//   0x0099E4B0.
// Layout (Ego_r.pdb CFishCompOrganiser, 0x84 bytes; retail 0x78: the map and the two vectors are 12 bytes, PDB 16):
//   CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, seh_Hero +0x1c, seh_Organiser +0x2c,
//   csargs +0x3c, CommentHitByHero / Conversation / FishingRodsInGame / FishTypesInGame / FishingPrizes +0x48..+0x58
//   (not initialised), CompetitionEntry +0x5c, RodDef +0x60, FishDef +0x6c.
// Shape: the landed entity-script ctor model (CBriarRose 0x00D9E930): Game (= parent->+0x40) is a plain initialiser
//   stored before the base vtable, ParentClass before this class's vtable, member objects after it (zero words share
//   ebx), the STLport map header cleared/allocated/self-linked, PMasterData a body assignment stored last.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CBaseClass {                           // vtable 0x01231710
    virtual void v00();
    CBaseClass();                             // 0x0099A2D0
};
struct CCountedPointerWords {                 // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CScriptThing : CBaseClass {            // vtable 0x01238C8C, 12 bytes (PImp_Data +4, PImp_Info +8)
    CCountedPointerWords PImp;                // +0x04
    virtual void v00();
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
struct CBaseObject {                          // CBaseIntelligentPointer root
    virtual void v00();
    void* m_pIntelligentPointer;              // +0x04
    CBaseObject();                            // 0x0099A380
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                // +0x08 Data, +0x0c Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
};
struct CCharString {
    void* m_pStringData;
    CCharString();                            // 0x0099E4B0
};

struct _Rb_tree_node_base {
    char                _M_color;             // +0x00
    _Rb_tree_node_base* _M_parent;            // +0x04
    _Rb_tree_node_base* _M_left;              // +0x08
    _Rb_tree_node_base* _M_right;             // +0x0c
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

struct CScriptOwner {                         // the quest script that owns this entity script
    unsigned char _pad00[0x40];
    void*         m_pInterface;               // +0x40 CGameScriptInterfaceBase*
};
struct CEntityScriptBase {                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    void*        Game;                        // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing Me;                          // +0x08
    CEntityScriptBase(const CScriptThing& thing, void* game) : Game(game), Me(thing) { }
};

class CFishCompOrganiser : public CEntityScriptBase {   // vtable 0x012EA620
public:
    CScriptOwner* ParentClass;                // +0x14 CV_FishingCompetitionScript*
    void*         PMasterData;                // +0x18 CQ_SunnyvaleMasterData*
    CScriptGameResourceObjectScriptedThingBase seh_Hero;        // +0x1c
    CScriptGameResourceObjectScriptedThingBase seh_Organiser;   // +0x2c
    CRetailMap<0x18> csargs;                  // +0x3c map<CCharString, CCharString>
    bool          CommentHitByHero;           // +0x48
    long          Conversation;               // +0x4c
    long          FishingRodsInGame;          // +0x50
    long          FishTypesInGame;            // +0x54
    long          FishingPrizes;              // +0x58
    CCharString   CompetitionEntry;           // +0x5c
    CRetailVector<CCharString> RodDef;        // +0x60
    CRetailVector<CCharString> FishDef;       // +0x6c
    virtual void v00();
    CFishCompOrganiser(CScriptOwner* parent, void* masterData, const CScriptThing& me);
};

CFishCompOrganiser::CFishCompOrganiser(CScriptOwner* parent, void* masterData, const CScriptThing& me)
    : CEntityScriptBase(me, parent->m_pInterface), ParentClass(parent), seh_Hero(), seh_Organiser()
{
    PMasterData = masterData;
}