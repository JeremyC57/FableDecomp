// CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool hidden) const -- collect the AI creatures, the TC 0x69 things, the chest-reward and gold-bag markers and the TC 0xCC things into one list and set each live one's visibility to !hidden
// Grade: DIFFER(ours v retail) -- scheduling only: the TC-filter type stores (0x69, 0xCC) land after the filter-address push and the 0xCC call's ThingSearchTools load moves above it
// Leaf: ?SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00894EC0, length 333 bytes (`ret 4`).
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the STLport list sentinel and nodes), the CThingSearchTools::GetAllThings
//   instantiations on World->ThingSearchTools (+0x20): <CThingAICreature, CThingFilter_None> @ 0x008AC5D0,
//   <CThing, CThingFilter_HasTCOfLeafType> (type 0x69) @ 0x004ACD70, <CThingMarker, CThingFilter_HasScriptName> @ 0x008AC660
//   (twice), and the TC-type filter (type 0xCC) @ 0x008AC700 (manifest labels are propagated template names; the
//   container here is a std::list<CThing*>), CCharString ctor/copy ctor/dtor @ 0x0099EBF0 / 0x0099EC30 / 0x0099EAE0,
//   the thing's virtual slot 28 (+0x70, SetVisible-style bool).
// Shape: an STLport list (sentinel node allocated in the ctor, nodes freed in clear()); the empty CThingFilter_None is
//   homed in the dead `hidden` slot (the bool is read into bl before the loop); each script-name filter is built from a
//   temporary CCharString by an inline copying ctor, both destroyed at the end of the call statement.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CThing_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetVisible(bool visible);              // slot 28 (+0x70)
    unsigned char _pad04[0x8d];
    unsigned char m_dead : 1;                           // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CThing_Retail*  _M_data;
};
struct CThingList {                                     // STLport list<CThing*>: the sentinel node pointer
    SThingListNode* _M_node;
    CThingList()
    {
        SThingListNode* node = (SThingListNode*)malloc(sizeof(SThingListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CThingList()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        SThingListNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SThingListNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
struct CThingFilter_None {};
struct CThingFilter_HasTCOfLeafType {
    long m_type;
    CThingFilter_HasTCOfLeafType(long type) : m_type(type) {}
};
struct CThingFilter_HasScriptName {
    CCharString m_name;
    CThingFilter_HasScriptName(const CCharString& name) : m_name(name) {}
};
struct CThingSearchTools {
    void GetAllAICreatures(const CThingFilter_None& filter, CThingList& out);                  // 0x008AC5D0
    void GetAllThingsWithTC(const CThingFilter_HasTCOfLeafType& filter, CThingList& out);     // 0x004ACD70
    void GetAllMarkersWithScriptName(const CThingFilter_HasScriptName& filter, CThingList& out);   // 0x008AC660
    void GetAllObjectsWithTC(const CThingFilter_HasTCOfLeafType& filter, CThingList& out);    // 0x008AC700
};
// Same-TU stand-ins (the GetAllThings instantiations are emitted in this TU): their bodies only touch the list's nodes,
// so VC7.1 keeps the sentinel pointer in esi across the calls as retail does (GOTCHAS wave 25 chunk 180).
void AppendMatchingThings(CThingSearchTools* tools, long kind, SThingListNode* sentinel);
#pragma auto_inline(off)
void CThingSearchTools::GetAllAICreatures(const CThingFilter_None& filter, CThingList& out) { AppendMatchingThings(this, 0, out._M_node); }
void CThingSearchTools::GetAllThingsWithTC(const CThingFilter_HasTCOfLeafType& filter, CThingList& out) { AppendMatchingThings(this, filter.m_type, out._M_node); }
void CThingSearchTools::GetAllMarkersWithScriptName(const CThingFilter_HasScriptName& filter, CThingList& out) { AppendMatchingThings(this, -1, out._M_node); }
void CThingSearchTools::GetAllObjectsWithTC(const CThingFilter_HasTCOfLeafType& filter, CThingList& out) { AppendMatchingThings(this, filter.m_type, out._M_node); }
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
struct CGameScriptInterface {
    virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool hidden) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

void CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool hidden) const
{
    CThingList things;
    CThingFilter_None all;
    World->ThingSearchTools->GetAllAICreatures(all, things);
    World->ThingSearchTools->GetAllThingsWithTC(CThingFilter_HasTCOfLeafType(0x69), things);
    {
        CCharString name("OBJECT_CHEST_REWARD_ON_DEATH");
        World->ThingSearchTools->GetAllMarkersWithScriptName(CThingFilter_HasScriptName(name), things);
    }
    {
        CCharString name("OBJECT_GOLDBAG_SMALL_WITH_COINS");
        World->ThingSearchTools->GetAllMarkersWithScriptName(CThingFilter_HasScriptName(name), things);
    }
    World->ThingSearchTools->GetAllObjectsWithTC(CThingFilter_HasTCOfLeafType(0xcc), things);
    for (SThingListNode* it = things._M_node->_M_next; it != things._M_node; it = it->_M_next) {
        CThing_Retail* thing = it->_M_data;
        if (thing && !thing->m_dead)
            thing->SetVisible(!hidden);
    }
}
