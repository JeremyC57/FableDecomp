// CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool hidden) const -- collect the dead creatures, the experience-orb things (TC 0x69), the chest-reward and small gold-bag objects (by def name) and the TC 0xCC objects into one list and set each live one's drawable flag to !hidden
// Leaf: ?SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00894EC0, 335 bytes, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden@CGameScriptInterface@@UBEX_N@Z
//   (engine/CGameScriptInterface.h declares the slot).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list sentinel and nodes); the CThingSearchTools::GetAllThings<T,
//   list<CThing*>, F> instantiations on World->PThingSearchTools (+0x20), named from the Ego_r/FableWin name sets (the only
//   list<CThing*> instantiations with these filters): <CThingDeadCreature, CThingFilter_None> 0x008AC5D0, <CThing,
//   CThingFilter_HasTC> 0x004ACD70 (already bound with that spelling in reloc_bindings.tsv), <CThingObject,
//   CThingFilter_HasDefName> 0x008AC660 (twice), <CThingObject, CThingFilter_HasTC> 0x008AC700 (the manifest's
//   "GetAllThings<CThingAICreature/CThingMarker, vector, ...>" labels are propagated); CCharString(const char*, long)
//   0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0; CThing::SetAsDrawable(bool) slot 28 (+0x70, FableWin.pdb
//   ?SetAsDrawable@CThing@@UAEX_N@Z, as the landed for_each 0x0071B700).
// Strings: "OBJECT_CHEST_REWARD_ON_DEATH" 0x012694D4, "OBJECT_GOLDBAG_SMALL_WITH_COINS" 0x0125D760 (def names: HasDefName).
// Layout: CThing Dead = bit 0 of +0x91 (Ego_r.pdb +0xA1; IsDead() is the PDB's QBE accessor); filters per Ego_r.pdb
//   (CThingFilter_HasTC {ETCInterfaceType Type}, CThingFilter_HasDefName {CCharString Name}); engine/CThing.h is quarantined, so
//   CThing is a local model with its slot 28; CThingSearchTools uses the Layout include pattern (the header has no members
//   for the template).
// Shape: an STLport list (sentinel node malloc'ed in the ctor, nodes freed in clear()); the empty CThingFilter_None is a named
//   local VC7.1 homes in the dead `hidden` slot; each def-name filter is copy-built from a scoped named CCharString.
//   What matched (chunk 638's lever): the same-TU stand-ins of the search template take the HasTC filter's ADDRESS, so the
//   0x69 / 0xCC type stores sit between the lea of &filter and its push as in retail; the old stand-ins read filter.Type by
//   value and VC7.1 sank the store below the push. The empty None filter's stand-in does not take its address (the real
//   template's filter call reads nothing), which keeps it homed in the dead `hidden` slot.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetAsDrawable(bool drawable);          // slot 28 (+0x70)
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    bool IsDead() const { return Dead; }
};
class CThingDeadCreature;
class CThingObject;
namespace std {
template <class T> class allocator;
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T, class A> class list;
template <> class list<CThing*, allocator<CThing*> > {
public:
    _List_node<CThing*>* _M_node;
    list()
    {
        _List_node<CThing*>* node = (_List_node<CThing*>*)malloc(sizeof(_List_node<CThing*>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~list()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        _List_node<CThing*>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node<CThing*>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
}
typedef std::list<CThing*, std::allocator<CThing*> > CThingList;
enum ETCInterfaceType { };                          // 0x69 / 0xCC: enumerator names unproven
class CThingFilter_None { public: };
class CThingFilter_HasTC { public:
    ETCInterfaceType Type;
    CThingFilter_HasTC(ETCInterfaceType type) : Type(type) { }
};
class CThingFilter_HasDefName { public:
    CCharString Name;
    CThingFilter_HasDefName(const CCharString& name) : Name(name) { }
};
class CThingSearchTools { public:
    template <class T, class TContainer, class TFilter>
    void GetAllThings(const TFilter& filter, TContainer& out) const;
};
void FindThings_None(const CThingSearchTools* tools, std::_List_node<CThing*>* node);
void FindThings_HasTC(const CThingSearchTools* tools, const CThingFilter_HasTC* filter, std::_List_node<CThing*>* node);
void FindThings_HasDefName(const CThingSearchTools* tools, const CThingFilter_HasDefName* filter, std::_List_node<CThing*>* node);
// Same-TU write-only stand-ins for the template bodies (instantiated in the caller's TU in retail): they only pass the
//   HasTC / HasDefName filter's address and touch the output list's sentinel.
#pragma auto_inline(off)
template <> void CThingSearchTools::GetAllThings<CThingDeadCreature, CThingList, CThingFilter_None>(const CThingFilter_None& filter, CThingList& out) const
{ FindThings_None(this, out._M_node); }                                     // 0x008AC5D0
template <> void CThingSearchTools::GetAllThings<CThing, CThingList, CThingFilter_HasTC>(const CThingFilter_HasTC& filter, CThingList& out) const
{ FindThings_HasTC(this, &filter, out._M_node); }                                  // 0x004ACD70
template <> void CThingSearchTools::GetAllThings<CThingObject, CThingList, CThingFilter_HasDefName>(const CThingFilter_HasDefName& filter, CThingList& out) const
{ FindThings_HasDefName(this, &filter, out._M_node); }                             // 0x008AC660
template <> void CThingSearchTools::GetAllThings<CThingObject, CThingList, CThingFilter_HasTC>(const CThingFilter_HasTC& filter, CThingList& out) const
{ FindThings_HasTC(this, &filter, out._M_node); }                                  // 0x008AC700
#pragma auto_inline(on)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CWorld.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool hidden) const
{
    CThingList things;
    CThingFilter_None all;
    World->PThingSearchTools_ptr->GetAllThings<CThingDeadCreature>(all, things);
    World->PThingSearchTools_ptr->GetAllThings<CThing>(CThingFilter_HasTC((ETCInterfaceType)0x69), things);
    {
        CCharString name("OBJECT_CHEST_REWARD_ON_DEATH");
        World->PThingSearchTools_ptr->GetAllThings<CThingObject>(CThingFilter_HasDefName(name), things);
    }
    {
        CCharString name("OBJECT_GOLDBAG_SMALL_WITH_COINS");
        World->PThingSearchTools_ptr->GetAllThings<CThingObject>(CThingFilter_HasDefName(name), things);
    }
    World->PThingSearchTools_ptr->GetAllThings<CThingObject>(CThingFilter_HasTC((ETCInterfaceType)0xcc), things);
    for (std::_List_node<CThing*>* it = things._M_node->_M_next; it != things._M_node; it = it->_M_next) {
        CThing* thing = it->_M_data;
        if (thing && !thing->IsDead())
            thing->SetAsDrawable(!hidden);
    }
}