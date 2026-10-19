// Behaviour test for CFishCompOrganiser::CFishCompOrganiser (0x00E7EF70): interface/parent/master-data/Me wiring,
// cleared handles, empty self-linked map header and empty vectors, built over garbage-filled storage.
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <new>
struct CBaseClass {                           // vtable 0x01231710
    virtual void v00() {}
    CBaseClass() {}
};
struct CCountedPointerWords {                 // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CScriptThing : CBaseClass {            // vtable 0x01238C8C, 12 bytes (PImp_Data +4, PImp_Info +8)
    CCountedPointerWords PImp;                // +0x04
    virtual void v00() {}
    CScriptThing() {}
    CScriptThing(const CScriptThing& other) : CBaseClass() { PImp.Data = other.PImp.Data; PImp.Info = other.PImp.Info; }
};
struct CBaseObject {                          // CBaseIntelligentPointer root
    virtual void v00() {}
    void* m_pIntelligentPointer;
    CBaseObject() : m_pIntelligentPointer(0) {}
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                // +0x08 Data, +0x0c Info
    virtual void v00() {}
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
};
struct CCharString {
    void* m_pStringData;
    CCharString() : m_pStringData(0) {}
};

struct _Rb_tree_node_base {
    char                _M_color;             // +0x00
    _Rb_tree_node_base* _M_parent;            // +0x04
    _Rb_tree_node_base* _M_left;              // +0x08
    _Rb_tree_node_base* _M_right;             // +0x0c
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize * sizeof(void*) / 4); }   // host-width node (32-bit: NodeSize)
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
    virtual void v00() {}
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
    virtual void v00() {}
    CFishCompOrganiser(CScriptOwner* parent, void* masterData, const CScriptThing& me);
};

CFishCompOrganiser::CFishCompOrganiser(CScriptOwner* parent, void* masterData, const CScriptThing& me)
    : CEntityScriptBase(me, parent->m_pInterface), ParentClass(parent), seh_Hero(), seh_Organiser()
{
    PMasterData = masterData;
}

int main()
{
    void* mem = malloc(sizeof(CFishCompOrganiser)); memset(mem, 0xcd, sizeof(CFishCompOrganiser));
    CScriptOwner owner; int iface = 0, master = 0, impData = 0, impInfo = 0;
    owner.m_pInterface = &iface;
    CScriptThing me; me.PImp.Data = &impData; me.PImp.Info = &impInfo;
    CFishCompOrganiser* p = new (mem) CFishCompOrganiser(&owner, &master, me);
    int bad = 0;
    if (p->Game != &iface || p->ParentClass != &owner || p->PMasterData != &master) bad = 1;
    if (p->Me.PImp.Data != &impData || p->Me.PImp.Info != &impInfo) bad = 2;
    if (p->seh_Hero.PImp.Data || p->seh_Organiser.PImp.Info || p->seh_Hero.m_pIntelligentPointer) bad = 3;
    _Rb_tree_node_base* h = p->csargs._M_header;
    if (!h || h->_M_color || h->_M_parent || h->_M_left != h || h->_M_right != h || p->csargs._M_node_count) bad = 4;
    if (p->CompetitionEntry.m_pStringData || p->RodDef._M_start || p->RodDef._M_finish || p->RodDef._M_end_of_storage._M_data) bad = 5;
    if (p->FishDef._M_start || p->FishDef._M_finish || p->FishDef._M_end_of_storage._M_data) bad = 6;
    if (bad) { std::printf("00e7ef70_TEST FAIL\n"); return 1; }
    std::printf("00e7ef70_TEST PASS\n");
    return 0;
}