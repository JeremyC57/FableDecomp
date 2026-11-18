// Behaviour test for 0x006C14D0 CTCBuyableHouse::OnInitialActivate(): the row's defines, includes and body verbatim over the shared headers,
// run on zeroed object images (no engine constructor runs). Doubles: CTCBase::GetDefinitionManager, CDefinitionManager GetPDefFromGlobalIndex
// (both overloads) / GetDefGlobalIndexFromName, the def's GetPSubDefInfo (vtable slot 14), CVectorMap::LowerBound (table lookup by key),
// CThing::AddTC (installs the thing-owner) / Kill, CBaseIntelligentPointer (item stored in PData), CCharString, GFRemapToThingAfterSerialisation,
// NeedsMainSignPost / CreateMainSignPost. The shared GetDef / GetSubDef bodies run for real. Cases: def index 0 / set, GetDef index > 0,
// GetSubDef info found / missing; thing-owner present / added; each sign-post def index (main / owned / rent / dressing / other / null item);
// the main sign post alive (killed, reset), dead, null; NeedsMainSignPost false / true with the post dead, null or (made by the double) alive.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTHINGOWNER
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_ONINITIALACTIVATE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CBuyableHouseDef.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/ThingAlive.h"
namespace { extern const CCharString MAIN_SIGN_POST_DEF_NAME; }          // 0x013BAAF8 "OBJECT_BUYABLE_HOUSE_MAIN_SIGNPOST"
namespace { extern const CCharString MAIN_OWNED_SIGN_POST_DEF_NAME; }    // 0x013BAB00
namespace { extern const CCharString MAIN_RENT_SIGN_POST_DEF_NAME; }     // 0x013BAB04
namespace { extern const CCharString DRESSING_SIGN_POST_DEF_NAME; }      // 0x013BAAFC
void FABLE_FASTCALL GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>& ptr);   // 0x0088AFF0 Ego_r / FableWin ?GFRemapToThingAfterSerialisation@@YIXAAV?$CIntelligentPointer@VCThing@@@@@Z (Ego_d parameter ptr)

void CTCBuyableHouse::OnInitialActivate()
{
    CTCThingOwner* ptc_thing_owner;
    if (GetParentThing().GetDefGlobalIndex())
    {
        CDefPointer<const CThingBaseDef> pdef;
        GetDefinitionManager().GetDef(GetParentThing().GetDefGlobalIndex(), &pdef);
        pdef->GetSubDef(&PDef);
    }
    if (!GetParentThing().HasTC(TCI_THING_OWNER))
        GetParentThing().AddTC("CTCThingOwner", false, 0);
    long main_signpost_def_index = GetDefinitionManager().GetDefGlobalIndexFromName(MAIN_SIGN_POST_DEF_NAME);
    long main_owned_def_index = GetDefinitionManager().GetDefGlobalIndexFromName(MAIN_OWNED_SIGN_POST_DEF_NAME);
    long main_rent_def_index = GetDefinitionManager().GetDefGlobalIndexFromName(MAIN_RENT_SIGN_POST_DEF_NAME);
    long dressing_signpost_def_index = GetDefinitionManager().GetDefGlobalIndexFromName(DRESSING_SIGN_POST_DEF_NAME);
    ptc_thing_owner = 0;
    GetParentThing().GetTC(TCI_THING_OWNER, &ptc_thing_owner);
    {
        std::list<CIntelligentPointer<CThing> >::const_iterator end_it = ptc_thing_owner->GetObjectList().end();
        std::list<CIntelligentPointer<CThing> >::const_iterator it;
        for (it = ptc_thing_owner->GetObjectList().begin(); it != end_it; ++it)
        {
            CThing* pthing = it->Get();
            if (pthing)
            {
                if (pthing->GetDefGlobalIndex() == main_signpost_def_index)
                    MainSignPost = CIntelligentPointer<CThing>(pthing);
                else if (pthing->GetDefGlobalIndex() == main_owned_def_index)
                    MainSignPost = CIntelligentPointer<CThing>(pthing);
                else if (pthing->GetDefGlobalIndex() == main_rent_def_index)
                    MainSignPost = CIntelligentPointer<CThing>(pthing);
                else if (pthing->GetDefGlobalIndex() == dressing_signpost_def_index)
                    DressingSignPost = CIntelligentPointer<CThing>(pthing);
            }
        }
    }
    GFRemapToThingAfterSerialisation(WifeLivingHere);
    if (IS_THING_ALIVE(MainSignPost))
    {
        MainSignPost->Kill(true);
        MainSignPost = CIntelligentPointer<CThing>(0);
    }
    if (NeedsMainSignPost() && !IS_THING_ALIVE(MainSignPost))
        CreateMainSignPost();
}

// ---- doubles ----
static int g_bad=0;
static void check(bool ok, int line) { if (!ok) { printf("check failed line %d\n", line); ++g_bad; } }
struct SPair { long key; void* value; };
static SPair g_pairs[16]; static int g_npairs=0;
static void* g_map_owner[16];
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
    for (int i=0;i<g_npairs;++i) if (g_map_owner[i]==(void*)this && g_pairs[i].key==(long)key) return (iterator)&g_pairs[i];
    return end();
}
CCharString::CCharString(const char* text, long) { *(const char**)this=text; }
CCharString::~CCharString() { }
static const char* str_of(const CCharString& s) { return *(const char* const*)&s; }
namespace {
const CCharString MAIN_SIGN_POST_DEF_NAME("OBJECT_BUYABLE_HOUSE_MAIN_SIGNPOST");
const CCharString MAIN_OWNED_SIGN_POST_DEF_NAME("OBJECT_BUYABLE_HOUSE_OWNED_SIGNPOST");
const CCharString MAIN_RENT_SIGN_POST_DEF_NAME("OBJECT_BUYABLE_HOUSE_RENT_SIGNPOST");
const CCharString DRESSING_SIGN_POST_DEF_NAME("OBJECT_BUYABLE_HOUSE_DRESSING_SIGNPOST");
}
static unsigned char g_house[0x60], g_parent[0x200], g_owner[0x40], g_mgr[0x200];
static unsigned char g_thingdef[0x60], g_housedef[0x60];
static void* g_vtbl[24];
static long g_info_index; static bool g_info_found; static int n_info, n_getpdef, n_getpdef_nc; static long g_getpdef_arg, g_getpdef_nc_arg;
static int n_names;
const CGameDefinitionManager& CTCBase::GetDefinitionManager() const { check((const void*)this==g_house, __LINE__); return *(const CGameDefinitionManager*)g_mgr; }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const
{
    check((const void*)this==g_mgr, __LINE__); ++n_names;
    const char* s=str_of(name);
    if (strcmp(s,"OBJECT_BUYABLE_HOUSE_MAIN_SIGNPOST")==0) return 101;
    if (strcmp(s,"OBJECT_BUYABLE_HOUSE_OWNED_SIGNPOST")==0) return 102;
    if (strcmp(s,"OBJECT_BUYABLE_HOUSE_RENT_SIGNPOST")==0) return 103;
    if (strcmp(s,"OBJECT_BUYABLE_HOUSE_DRESSING_SIGNPOST")==0) return 104;
    check(false, __LINE__); return -1;
}
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) const
{
    check((const void*)this==g_mgr, __LINE__); ++n_getpdef; g_getpdef_arg=index;
    CDefPointer<const CDefClassBase> r; *(void**)&r=g_thingdef; ++*(long*)(g_thingdef+4); return r;
}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    check((void*)this==g_mgr, __LINE__); ++n_getpdef_nc; g_getpdef_nc_arg=index;
    CDefPointer<CDefClassBase> r; *(void**)&r=g_housedef; ++*(long*)(g_housedef+4); return r;
}
struct SFakeDefCalls {
    const CDefClassBase::CSubDefInfo* GetInfo(const CCharString& name)
    {
        static CDefClassBase::CSubDefInfo info;
        check((void*)this==g_thingdef, __LINE__); check(strcmp(str_of(name),"CBuyableHouseDef")==0, __LINE__); ++n_info;
        info.DefIndex=g_info_index; return g_info_found ? &info : 0;
    }
    void Delete() { check(false, __LINE__); }
};
CBaseIntelligentPointer::CBaseIntelligentPointer() { PData=0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData=(CBaseObjectPointer*)item; }
static int n_addtc; static char g_addtc_name[64]; static bool g_addtc_flag; static const void* g_addtc_params;
static void set_bit(unsigned char* thing, long type) { ((unsigned long*)(thing+0x20))[type>>5] |= 1UL<<(type&31); }
static void add_tc(unsigned char* thing, long type, void* tc) { set_bit(thing,type); g_map_owner[g_npairs]=thing+0x44; g_pairs[g_npairs].key=type; g_pairs[g_npairs].value=tc; ++g_npairs; }
CTCBase* CThing::AddTC(const CCharString& name, bool flag, const CTCParamsBase* params)
{
    check((void*)this==g_parent, __LINE__); ++n_addtc; strcpy(g_addtc_name,str_of(name)); g_addtc_flag=flag; g_addtc_params=params;
    add_tc(g_parent,TCI_THING_OWNER,g_owner); return (CTCBase*)g_owner;
}
static void* g_killed; static bool g_kill_arg; static int n_kill;
void CThing::Kill(bool b) { ++n_kill; g_killed=this; g_kill_arg=b; }
static int n_remap; static void* g_remap_arg;
void FABLE_FASTCALL GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>& ptr) { ++n_remap; g_remap_arg=&ptr; }
static bool g_needs; static int n_needs, n_create; static void* g_needs_sets;
bool CTCBuyableHouse::NeedsMainSignPost() { check((void*)this==g_house, __LINE__); ++n_needs; if (g_needs_sets) ((CBaseIntelligentPointer*)(g_house+0x24))->SetPItem((CBaseObject*)g_needs_sets); return g_needs; }
void CTCBuyableHouse::CreateMainSignPost() { check((void*)this==g_house, __LINE__); ++n_create; }

// list nodes (STLport layout: next, prev, data); the list object holds the sentinel pointer
struct SNode { SNode* next; SNode* prev; CIntelligentPointer<CThing> data; };
static SNode g_nodes[8]; static SNode g_sentinel;
static unsigned char g_things[8][0x100];
static void build_list(int n, const long* idx)
{
    SNode* prev=&g_sentinel;
    for (int i=0;i<n;++i) {
        memset(g_things[i],0,sizeof g_things[i]);
        *(unsigned short*)(g_things[i]+0x8c)=(unsigned short)idx[i];
        g_nodes[i].data.SetPItem(idx[i]<0 ? 0 : (CBaseObject*)g_things[i]);
        prev->next=&g_nodes[i]; g_nodes[i].prev=prev; prev=&g_nodes[i];
    }
    prev->next=&g_sentinel; g_sentinel.prev=prev;
    *(SNode**)(g_owner+0x0c)=&g_sentinel;
}
static void* item_at(int off) { return (void*)((CBaseIntelligentPointer*)(g_house+off))->GetPItem(); }
static void set_item(int off, void* p) { ((CBaseIntelligentPointer*)(g_house+off))->SetPItem((CBaseObject*)p); }
static SPair g_endpair;
static void setup(unsigned short def_index, bool owner_present, int n, const long* idx)
{
    memset(g_house,0,sizeof g_house); memset(g_parent,0,sizeof g_parent); memset(g_owner,0,sizeof g_owner);
    g_npairs=0; n_info=n_getpdef=n_getpdef_nc=n_names=n_addtc=n_kill=n_remap=n_needs=n_create=0;
    g_getpdef_arg=g_getpdef_nc_arg=-1; g_killed=0; g_remap_arg=0; g_addtc_name[0]=0; g_needs_sets=0; g_needs=false;
    g_info_found=true; g_info_index=55;
    *(void**)(g_house+4)=g_parent;
    *(void**)(g_parent+0x44+4)=&g_endpair;
    *(unsigned short*)(g_parent+0x8c)=def_index;
    if (owner_present) add_tc(g_parent,TCI_THING_OWNER,g_owner);
    memset(g_thingdef,0,sizeof g_thingdef); memset(g_housedef,0,sizeof g_housedef);
    *(void***)g_thingdef=g_vtbl; *(void***)g_housedef=g_vtbl; *(long*)(g_thingdef+4)=5; *(long*)(g_housedef+4)=5; *(void**)(g_thingdef+0x1c)=g_mgr;
    build_list(n, idx);
}
static void dead_all(int n) { for (int i=0;i<n;++i) g_things[i][0x91]=1; }
static void run() { ((CTCBuyableHouse*)g_house)->CTCBuyableHouse::OnInitialActivate(); }

int main()
{
    union { const CDefClassBase::CSubDefInfo* (SFakeDefCalls::*m)(const CCharString&); void* p; } u1; u1.m=&SFakeDefCalls::GetInfo;
    union { void (SFakeDefCalls::*m)(); void* p; } u2; u2.m=&SFakeDefCalls::Delete;
    for (int i=0;i<24;++i) g_vtbl[i]=u2.p;
    g_vtbl[14]=u1.p;
    const long none[1]={0};
    // def index 0: no GetDef; owner present: no AddTC; empty list; nothing needed
    setup(0,true,0,none); run();
    check(n_getpdef==0 && n_info==0 && n_addtc==0 && n_names==4, __LINE__); check(*(void**)(g_house+0x0c)==0, __LINE__);
    check(n_remap==1 && g_remap_arg==(void*)(g_house+0x38), __LINE__); check(n_needs==1 && n_create==0 && n_kill==0, __LINE__);
    // def index 7: GetDef(7) + GetSubDef (info found -> house def taken, ref held; the thing def released back to 5)
    setup(7,true,0,none); run();
    check(n_getpdef==1 && g_getpdef_arg==7 && n_info==1 && n_getpdef_nc==1 && g_getpdef_nc_arg==55, __LINE__);
    check(*(void**)(g_house+0x0c)==g_housedef && *(long*)(g_housedef+4)==6 && *(long*)(g_thingdef+4)==5, __LINE__);
    // GetSubDef info missing: PDef untouched
    setup(7,true,0,none); g_info_found=false; run();
    check(n_info==1 && n_getpdef_nc==0 && *(void**)(g_house+0x0c)==0 && *(long*)(g_thingdef+4)==5, __LINE__);
    // owner missing: AddTC("CTCThingOwner", false, 0) then the owner is used
    { const long l[1]={101}; setup(0,false,1,l); dead_all(1); run(); }
    check(n_addtc==1 && strcmp(g_addtc_name,"CTCThingOwner")==0 && !g_addtc_flag && g_addtc_params==0, __LINE__);
    check(item_at(0x24)==g_things[0], __LINE__);
    // (posts marked dead so the end-of-function kill keeps them) each sign-post index: main, owned, rent go to MainSignPost (last wins), dressing to DressingSignPost, others / null ignored
    { const long l[3]={999,104,-1}; setup(0,true,3,l); dead_all(3); run(); check(item_at(0x24)==0 && item_at(0x2c)==g_things[1], __LINE__); }
    { const long l[2]={102,999}; setup(0,true,2,l); dead_all(2); run(); check(item_at(0x24)==g_things[0] && item_at(0x2c)==0, __LINE__); }
    { const long l[2]={103,104}; setup(0,true,2,l); dead_all(2); run(); check(item_at(0x24)==g_things[0] && item_at(0x2c)==g_things[1], __LINE__); }
    { const long l[3]={101,102,103}; setup(0,true,3,l); dead_all(3); run(); check(item_at(0x24)==g_things[2], __LINE__); }
    // a live main sign post is killed (true) and reset; then NeedsMainSignPost -> create
    { const long l[1]={101}; setup(0,true,1,l); g_needs=true; run();
      check(n_kill==1 && g_killed==g_things[0] && g_kill_arg, __LINE__); check(item_at(0x24)==0, __LINE__); check(n_needs==1 && n_create==1, __LINE__); }
    // a dead main sign post is kept, not killed; needed -> created (not alive)
    { const long l[1]={101}; setup(0,true,1,l); g_things[0][0x91]=1; g_needs=true; run();
      check(n_kill==0 && item_at(0x24)==g_things[0] && n_create==1, __LINE__); }
    // dead post, not needed: no create
    { const long l[1]={101}; setup(0,true,1,l); g_things[0][0x91]=1; g_needs=false; run(); check(n_kill==0 && n_create==0, __LINE__); }
    // needed, but a live post exists when tested (made by the double): no create
    { setup(0,true,0,none); memset(g_things[5],0,sizeof g_things[5]); g_needs=true; g_needs_sets=g_things[5]; run(); check(n_needs==1 && n_create==0, __LINE__); }
    // needed, the post set by the double is dead: create
    { setup(0,true,0,none); memset(g_things[5],0,sizeof g_things[5]); g_things[5][0x91]=1; g_needs=true; g_needs_sets=g_things[5]; run(); check(n_create==1, __LINE__); }
    if (g_bad==0) printf("006c14d0_TEST PASS\n"); else printf("006c14d0_TEST FAIL %d\n", g_bad);
    return 0;
}