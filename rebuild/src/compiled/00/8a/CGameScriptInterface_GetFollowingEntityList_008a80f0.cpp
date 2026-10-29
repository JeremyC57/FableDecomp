// CGameScriptInterface::GetFollowingEntityList(const CScriptThing& leader, std::vector<CScriptThing>& followers) const -- append the script things of the live leader's followers (TC 0xBC) that are not in the list yet; true when one was added
// Leaf: ?GetFollowingEntityList@CGameScriptInterface@@UBE_NABVCScriptThing@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A80F0, length 355 bytes (`ret 8`, bool in al).
// Callees: the script thing's slot 11 (+0x2C GetThing), the component-map LowerBound @ 0x0040F020, the TC 0xBC follower
//   query @ 0x006D5850 (fills a vector<CThing*>; manifest label propagated), CThing::GetScriptThing @ 0x004C7E50, STLport
//   __find<CScriptThing*, CScriptThing>(first, last, val, random_access_iterator_tag) @ 0x008AB5D0 (`__fastcall`),
//   vector<CScriptThing>::push_back @ 0x008ADF90, ~CScriptThing @ 0x004AA840, ~CBaseClass @ 0x0099A2E0,
//   operator delete @ 0x00BFE9BC, free @ 0x00BFEA14.
// Shape: a same-TU stand-in body for the __find instance (0x008AB5D0, emitted next door) tells VC7.1 the empty tag is never
//   written, so the iterator and the tag share the dead leader slot as in retail (advisor); the STLport tag hierarchy
//   temporary avoids a zero store; the first GetScriptThing temporary is destroyed inline, the push_back one out of line.
// Layout: CThing dead flags +0x91 bit 0, UsingComponents +0x20, component map +0x44.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CBaseClass { public:                                     // the polymorphic root (vptr at +0)
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {                              // CCountedPointer<CScriptThingImp>
    void* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
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
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
};
struct __false_type { __false_type() {} };
struct CScriptThingVector {                             // STLport vector<CScriptThing> (12-byte elements)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    void reserve(unsigned long n);                      // 0x008ACD30
    void _M_insert_overflow(CScriptThing* position, const CScriptThing& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x008AD5F0
    CScriptThing* begin() { return _M_start; }
    CScriptThing* end() { return _M_finish; }
    void push_back(const CScriptThing& x);              // 0x008ADF90 (out of line here)
};
struct CThing_Retail;
struct CThingVector {                                   // STLport vector<CThing*> (malloc allocator)
    CThing_Retail** _M_start;
    CThing_Retail** _M_finish;
    CThing_Retail** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct input_iterator_tag {};                          // the STLport tag hierarchy (non-aggregate: no zero store)
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
CScriptThing* __fastcall __find(CScriptThing* first, CScriptThing* last, const CScriptThing& val,
                                const random_access_iterator_tag&);   // 0x008AB5D0
bool __fastcall SameScriptThing_s7(const void* a, const void* b);   // stand-in: CScriptThing equality on the imp data
#pragma auto_inline(off)
// Same-TU body of the STLport random-access __find instantiation 0x008AB5D0: walks the range comparing each element
// with the value read through its const reference; the empty tag is never touched.
__declspec(noinline) CScriptThing* __fastcall __find(CScriptThing* first, CScriptThing* last, const CScriptThing& val,
                                                     const random_access_iterator_tag&)
{
    for (; first != last; ++first)
        if (SameScriptThing_s7(first->PImp.Data, val.PImp.Data))
            return first;
    return last;
}
#pragma auto_inline(on)
inline CScriptThing* find(CScriptThing* first, CScriptThing* last, const CScriptThing& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;
struct CThing_Retail {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];         // +0x20
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned char         _pad4c[0x45];
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(type);
        return ((CTCBase_Retail*)((it == map->last || interfaceType < it->first) ? map->last : it)->second);
    }
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
struct CTCFollowed_Retail { void GetFollowers(CThingVector& out); };   // 0x006D5850
struct CScriptThingRef {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
};
class CGameScriptInterface { public:
    virtual bool GetFollowingEntityList(const CScriptThingRef& leader, CScriptThingVector& followers) const;
};

bool CGameScriptInterface::GetFollowingEntityList(const CScriptThingRef& leader, CScriptThingVector& followers) const
{
    bool added = false;
    CThing_Retail* thing = leader.GetThing();
    if (thing && !thing->m_dead && thing->HasInterface(0xbc)) {
        CTCFollowed_Retail* followed = (CTCFollowed_Retail*)thing->GetInterface(0xbc);
        CThingVector found;
        followed->GetFollowers(found);
        for (CThing_Retail** it = found._M_start; it != found._M_finish; ++it) {
            CThing_Retail* follower = *it;
            if (find(followers.begin(), followers.end(), follower->GetScriptThing()) == followers.end()) {
                followers.push_back(follower->GetScriptThing());
                added = true;
            }
        }
    }
    return added;
}