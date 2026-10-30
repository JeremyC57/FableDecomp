// Grade: DIFFER(1409v1393) 366 difflines (wave 2 sec3: first reconstruction from the disassembly; was the 16-byte stub, 446 lines)
// Tried: g1 straight draft (1409v1393/466: the 8-aligned frame `and esp,-8` already reproduces); g2 the quest cards as a
//   list object bound to a reference local (retail keeps `&g->Cards` in ebx and re-reads the end node; 1409v1393/410);
//   g3 the search arguments as temporaries (frame now 0x50 as retail; this file, 1409v1393/366).
// Residue: stack-slot assignment is shifted by 4-8 bytes (this/world/map saves at +0x24/+0x20/+0x14 in retail), ebp is the
//   zero register in retail (ours edi); region-exit marker block and the set teardown line up structurally.
// Next: order of the local declarations (region before the sets; `exit`/`connection` stale locals placement), then the
//   shared STLport set/list models if they land; names to prove: g_pQuestManager 0x013B89FC, g_szNoRegion 0x01257444,
//   CTCRegionConnection (interface 0xC2), CThingTypeFilter for the 0x00618E60 overload.
// CMiniMapDisplay::InitialiseRegionExitMarkers() -- rebuild the mini-map region-exit markers: collect the next region on
//   the route to every live quest card's region (core quests in one set, minimap-route quests in another), then add an
//   auto marker for every active region exit (icon 0xB, 0xC on a core route, 0xD on a minimap route), then one marker per
//   region-connection thing whose target is alive
// Leaf: InitialiseRegionExitMarkers
// Callees: CWorldMap::GetLoadedRegion 0x004FB150, GetRegionNumberFromName 0x004FC210, GetNextRegionOnRouteTo 0x004FB4D0,
//   GetRegionNumberMapIsIn 0x004FC190; CTCQuestCard::GetRegionName 0x007025A0, IsCore 0x00702280,
//   IsRouteToAppearOnMinimap 0x007022F0; CTCDRegionExit::GetNameOfRegionConnectingTo 0x007792D0; CThingSearchTools::
//   GetAllThings 0x00617CA0 / 0x00618E60; CPlayerGui::PeekGuiDef 0x00438570 (+0x958 icon vector);
//   CMiniMapDisplay::AddAutoMarker 0x0082A500; STLport set<long> insert_unique 0x00512DA0, insert 0x00514230, find
//   0x00616580, _M_erase 0x004A9CB0; list<CMarkerInfo>::insert 0x0082C920; CBaseIntelligentPointer 0x00A01B10 /
//   SetPItem 0x00A01B90 / ~ 0x00A01C10; CThing::GetPos 0x004C73D0; CCharString::operator!= 0x0099E960; malloc / free.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);

class CTCBase;
class CBaseObject;
class CBaseObjectPointer;
class C3DVector { public: float X, Y, Z; };
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
    bool operator!=(const char* text) const;             // 0x0099E960
};

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer() { }
};

struct _Rb_tree_node { unsigned char _M_color; _Rb_tree_node* _M_parent; _Rb_tree_node* _M_left; _Rb_tree_node* _M_right; long _M_value_field; };
struct set_iterator { _Rb_tree_node* _M_node; };
struct set_pair { set_iterator first; bool second; };
class CLongSet {                                         // STLport 4.6 set<long>
public:
    _Rb_tree_node* _M_header;
    unsigned int   _M_node_count;
    __forceinline CLongSet()
    {
        _M_header = 0;
        _M_header = (_Rb_tree_node*)malloc(sizeof(_Rb_tree_node));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    void _M_erase(_Rb_tree_node* x);                     // 0x004A9CB0
    __forceinline void clear()
    {
        if (_M_node_count != 0) {
            _Rb_tree_node* x = _M_header->_M_parent;
            while (x != 0) {
                _M_erase(x->_M_right);
                _Rb_tree_node* y = x->_M_left;
                free(x);
                x = y;
            }
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    __forceinline ~CLongSet() { clear(); if (_M_header) free(_M_header); }
    set_pair insert_unique(const long& v);               // 0x00512DA0
    set_pair insert(const long& v);                      // 0x00514230
    set_iterator find(const long& k);                    // 0x00616580
    __forceinline _Rb_tree_node* end() const { return _M_header; }
    __forceinline _Rb_tree_node* find_inline(long k) const
    {
        _Rb_tree_node* y = _M_header;
        _Rb_tree_node* x = _M_header->_M_parent;
        while (x != 0) {
            if (x->_M_value_field < k)
                x = x->_M_right;
            else {
                y = x;
                x = x->_M_left;
            }
        }
        if (y == _M_header || k < y->_M_value_field)
            return _M_header;
        return y;
    }
};

template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> class list {                          // STLport 4.6 list<T>, malloc allocator
public:
    _List_node<T>* _M_node;
    __forceinline list()
    {
        _M_node = (_List_node<T>*)malloc(sizeof(_List_node<void*>));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    __forceinline ~list()
    {
        _List_node<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
};

class CThing;
class CMiniMapDisplay;
struct CMarkerInfo {
    CIntelligentPointer<const CThing> Thing;             // +0x00
    long Icon;                                           // +0x08
    bool Auto;                                           // +0x0C
};
struct CMarkerList {
    _List_node<CMarkerInfo>* _M_node;
    _List_node<CMarkerInfo>* insert(_List_node<CMarkerInfo>* pos, const CMarkerInfo& info);   // 0x0082C920 (iterator by value)
};

class CTCQuestCard {
public:
    CCharString GetRegionName() const;                   // 0x007025A0
    bool IsCore();                                       // 0x00702280
    bool IsRouteToAppearOnMinimap();                     // 0x007022F0
};
class CTCDRegionExit {
public:
    unsigned char _pad00[0x0c];
    bool          Active;                                // +0x0C
    unsigned char _pad0d[0x29 - 0x0d];
    bool          Hidden;                                // +0x29
    const CCharString& GetNameOfRegionConnectingTo();    // 0x007792D0
};
struct CTCRegionConnection {
    unsigned char _pad00[0x20];
    CIntelligentPointer<CThing> Target;                  // +0x20
    unsigned char _pad28[0x4d - 0x28];
    bool          Disabled;                              // +0x4D
};
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15();
    virtual long GetMapNumberAtPos(const C3DVector& pos);   // slot 16 (+0x40)
    long GetLoadedRegion() const;                        // 0x004FB150
    long GetRegionNumberFromName(const CCharString& name);  // 0x004FC210
    long GetNextRegionOnRouteTo(long from, long to) const;  // 0x004FB4D0
    long GetRegionNumberMapIsIn(long map) const;         // 0x004FC190
};
struct CThingTypeFilter { };
class CThingSearchTools {
public:
    void GetAllThings(const long& type, list<CThing*>& out);              // 0x00617CA0
    void GetAllThings(const CThingTypeFilter& filter, list<CThing*>& out); // 0x00618E60
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();                    // slot 13 (+0x34)
    unsigned char _pad04[0x20 - 0x04];
    CThingSearchTools* PSearchTools;                     // +0x20
};
class CEngine { public: unsigned char _pad00[0x24]; CWorld* PWorld; };   // +0x24
extern CEngine* g_pEngine;                               // 0x013B86A0
struct CCardList { _List_node<CIntelligentPointer<CThing> >* _M_node; };
class CQuestManager { public: unsigned char _pad00[0x5c]; CCardList Cards; };   // +0x5C list<CIntelligentPointer<CThing> >
extern CQuestManager* g_pQuestManager;                   // 0x013B89FC
struct CPlayerGuiDef { unsigned char _pad00[0x958]; long* MarkerIcons; };   // +0x958 vector<long>
struct CPlayerGui { static const CPlayerGuiDef* __fastcall PeekGuiDef(); };  // 0x00438570
extern const char g_szNoRegion[];                        // 0x01257444

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBoundConst(const long* type) const;     // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20
    CInterfaceMap m_interfaces;                          // +0x44
    unsigned char _pad4c[0x5c - 0x4c];
    CTCDRegionExit* PTCRegionExit;                       // +0x5C
    unsigned char _pad60[0x74 - 0x60];
    CCharString   RegionName;                            // +0x74
    unsigned char _pad78[0x91 - 0x78];
    unsigned char m_flags91;                             // +0x91 bit 0: dead
    const C3DVector& GetPos() const;                     // 0x004C73D0
    __forceinline bool Present(long t) const { return (m_presentBits[t >> 5] >> (t & 31)) & 1; }
    template<class T> __forceinline bool GetInterface(T** out, long t) const
    {
        bool present = Present(t);
        if (present) {
            long type = t;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBoundConst(&type);
            if (it == map->last || t < it->type)
                it = map->last;
            *out = (T*)it->component;
        }
        return present;
    }
    template<class T> __forceinline bool GetInterfaceInline(T** out, long t) const
    {
        bool present = Present(t);
        if (present) {
            CInterfaceMap::SEntry* first = m_interfaces.first;
            CInterfaceMap::SEntry* last = m_interfaces.last;
            int len = int(last - first);
            while (len > 0) {
                int half = len / 2;
                if (first[half].type < t) {
                    first += half + 1;
                    len = len - half - 1;
                } else
                    len = half;
            }
            if (first == last || t < first->type)
                first = last;
            *out = (T*)first->component;
        }
        return present;
    }
};

class CMiniMapDisplay {
public:
    unsigned char _pad00[0x20];
    CMarkerList   Markers;                               // +0x20
    unsigned char _pad24[0xb8 - 0x24];
    void*         MarkerMap;                             // +0xB8
    void AddAutoMarker(const CThing* thing, long icon);  // 0x0082A500
    void InitialiseRegionExitMarkers();
};

void CMiniMapDisplay::InitialiseRegionExitMarkers()
{
    if (MarkerMap == 0)
        return;
    CWorld* world = g_pEngine->PWorld;
    CWorldMap* map = world->GetWorldMap();
    long region = map->GetLoadedRegion();
    CLongSet coreRoute;
    CLongSet minimapRoute;
    CCardList& cards = g_pQuestManager->Cards;
    for (_List_node<CIntelligentPointer<CThing> >* node = cards._M_node->_M_next; node != cards._M_node; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        if (card == 0 || (card->m_flags91 & 1))
            continue;
        CTCQuestCard* quest = 0;
        card->GetInterface(&quest, 0x6c);
        long target = map->GetRegionNumberFromName(quest->GetRegionName());
        if (target <= 0)
            continue;
        long next = map->GetNextRegionOnRouteTo(region, target);
        if (next <= 0)
            continue;
        if (quest->IsCore())
            coreRoute.insert_unique(next);
        else if (quest->IsRouteToAppearOnMinimap())
            minimapRoute.insert(next);
    }

    list<CThing*> exits;
    world->PSearchTools->GetAllThings(100, exits);
    CTCDRegionExit* exit;
    for (_List_node<CThing*>* it = exits._M_node->_M_next; it != exits._M_node; it = it->_M_next) {
        CThing* thing = it->_M_data;
        if (thing->Present(0x64))
            exit = thing->PTCRegionExit;
        if (!exit->Active || exit->Hidden)
            continue;
        long icon = 0xb;
        long connecting = map->GetRegionNumberFromName(exit->GetNameOfRegionConnectingTo());
        if (thing->RegionName != g_szNoRegion) {
            if (coreRoute.find(connecting)._M_node != coreRoute.end())
                icon = 0xc;
            else if (minimapRoute.find(connecting)._M_node != minimapRoute.end())
                icon = 0xd;
        }
        AddAutoMarker(thing, CPlayerGui::PeekGuiDef()->MarkerIcons[icon]);
    }

    list<CThing*> connections;
    world->PSearchTools->GetAllThings(CThingTypeFilter(), connections);
    CTCRegionConnection* connection;
    for (_List_node<CThing*>* ct = connections._M_node->_M_next; ct != connections._M_node; ct = ct->_M_next) {
        CThing* thing = ct->_M_data;
        thing->GetInterfaceInline(&connection, 0xc2);
        if (connection->Disabled)
            continue;
        CThing* other = connection->Target.GetPItem();
        if (other == 0 || (other->m_flags91 & 1))
            continue;
        long icon = 0xb;
        long otherRegion = map->GetRegionNumberMapIsIn(map->GetMapNumberAtPos(other->GetPos()));
        if (coreRoute.find_inline(otherRegion) != coreRoute.end())
            icon = 0xc;
        else if (minimapRoute.find(otherRegion)._M_node != minimapRoute.end())
            icon = 0xd;
        long iconId = CPlayerGui::PeekGuiDef()->MarkerIcons[icon];
        if (iconId <= 0)
            continue;
        CMarkerInfo info;
        info.Thing.SetPItem(thing);
        info.Icon = iconId;
        info.Auto = true;
        Markers.insert(Markers._M_node, info);
    }
}
