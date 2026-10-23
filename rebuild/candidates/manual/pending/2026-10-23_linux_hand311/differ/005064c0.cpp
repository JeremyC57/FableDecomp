// Grade: DIFFER(536v542) -- family gap: whole-program register caching (retail caches the list head in ebp across GetAllThings, which receives &list) plus three loop-invariant push_back constant temps hoisted into separate slots
// CWorldMap::PostRegionLoad(long region) -- for every village thing in the world: populate it with villagers the first time (else refresh it), fill its stacks and add its crate tasks, bracketed by loading-progress ticks; then re-enable sound themes
// Leaf: ?PostRegionLoad@CWorldMap@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005064C0, 544 bytes, ret 4 (argument unused).
// Callees: CThingSearchTools::GetAllThings<CThing, list, CThingFilter_HasTCOfLeafType<CTCVillage>> 0x004ACD70 (World +0x20),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, __fastcall DisplayProgress 0x009E9F40,
//   CInterfaceMap::LowerBound 0x0040F020, CWorldMap::PopulateRegionWithVillagers(CThing*) 0x00502E90,
//   CTCVillage 0x00658603 (manifest "$E2"), __fastcall CTCVillage::FillStacks(long) 0x007B6B10,
//   vector<long>::_M_insert_overflow 0x00440990, CTCVillage::AddCrateTasksFromAndTo 0x00659C7E, free 0x00BFEA14.
// Layout: retail CThing interface bitset +0x20 (TC 0x22 = word +0x24 bit 2), interface map +0x44; CTCVillage
//   PopulatedOnce byte +0x7A; CWorldMap World +0x08, SoundThemesDisabled +0x4C.
#include <new>
#include <stdlib.h>
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40

struct SScalarTag { SScalarTag() { } };
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CLongVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00440990
    void push_back(const long& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) long(value);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, SScalarTag(), 1, true);
    }
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);              // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                  // +0x20
    CInterfaceMap Components;                          // +0x44
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        if (!(UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))))
            return 0;
        CInterfaceMap::SEntry* it = Components.LowerBound(interfaceType);
        if (it == Components.last || interfaceType < it->type)
            it = Components.last;
        return (T*)it->component;
    }
};
class CTCVillage {
public:
    unsigned char _pad00[0x7a];
    bool          PopulatedOnce;                       // +0x7A
    void Repopulate();                                 // 0x00658603
    void __fastcall FillStacks(long count);            // 0x007B6B10 (ecx = this, edx = count)
    void AddCrateTasksFromAndTo(const CLongVector& from, const CLongVector& to);   // 0x00659C7E
};

template<class T> struct list_node { list_node* _M_next; list_node* _M_prev; T _M_data; };
struct CThingList {
    list_node<CThing*>* _M_node;
    CThingList() { list_node<CThing*>* node = (list_node<CThing*>*)::operator new(sizeof(list_node<CThing*>)); node->_M_next = node; node->_M_prev = node; _M_node = node; }
    ~CThingList()
    {
        list_node<CThing*>* cur = _M_node->_M_next;
        while (cur != _M_node) { list_node<CThing*>* tmp = cur; cur = cur->_M_next; free(tmp); }
        _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node;
        free(_M_node);
    }
};
struct CThingFilter_HasTCOfLeafType { long Type; CThingFilter_HasTCOfLeafType(long type) : Type(type) {} };
struct CThingSearchTools {
    void GetAllThings(const CThingFilter_HasTCOfLeafType& filter, CThingList& out);   // 0x004ACD70
};
struct CWorld { unsigned char _pad[0x20]; CThingSearchTools* SearchTools; };

class CWorldMap {
public:
    virtual ~CWorldMap();
    long        f4;                                    // +0x04
    CWorld*     World;                                 // +0x08
    unsigned char _pad0c[0x40];
    bool        SoundThemesDisabled;                   // +0x4C
    void PopulateRegionWithVillagers(CThing* village); // 0x00502E90
    void PostRegionLoad(long region);
};

void CWorldMap::PostRegionLoad(long region)
{
    CThingList villages;
    World->SearchTools->GetAllThings(CThingFilter_HasTCOfLeafType(0x22), villages);
    const long villageStock = 2;
    const long noStock = 0;
    const long marketStock = 3;
    for (list_node<CThing*>* it = villages._M_node->_M_next; it != villages._M_node; it = it->_M_next)
    {
        {
            CCharString text("World Map: Post Region Load Villages");
            DisplayProgress(text, -1.0f, false, false);
        }
        CThing* thing = it->_M_data;
        CTCVillage* village = thing->GetTC<CTCVillage>(0x22);
        if (!village->PopulatedOnce)
        {
            PopulateRegionWithVillagers(thing);
            village->PopulatedOnce = true;
        }
        else
            village->Repopulate();
        village->FillStacks(2);
        CLongVector from;
        from.push_back(villageStock);
        CLongVector to;
        to.push_back(noStock);
        to.push_back(marketStock);
        village->AddCrateTasksFromAndTo(from, to);
        {
            CCharString text("World Map: Post Region Load Villages End");
            DisplayProgress(text, -1.0f, false, false);
        }
    }
    SoundThemesDisabled = false;
}
