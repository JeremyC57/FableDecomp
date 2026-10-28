// CAIStateGroup_ApprenticeHero::GetObjectWithScriptName(const CCharString& script_name) -- collect every game object carrying the script name and return one picked with the world's random seed (null when there are none)
// Grade: DIFFER(265v242) -- the found path's list<..>::~list is inlined (retail calls 0x006FE660 out of line and keeps the result in esi, ebx shrink-wrapped around the random pick); GFRandom's `range ? r % range : 0` folds to xor edx / div where retail branches into ecx (mov ecx,edx / xor ecx,ecx)
// Leaf: ?GetObjectWithScriptName@CAIStateGroup_ApprenticeHero@@IAEPAVCThingGameObject@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929FC0, length 242 bytes, `ret 4`. Called by GetDummy 0x0092A0C0.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes), CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0 (the CThingFilter_HasScriptName temporary), CAIStateGroupBase::PeekThingSearchTools 0x008FD000,
//   CThingSearchTools::GetAllThings<CThingGameObject, list<CThingGameObject*>, CThingFilter_HasScriptName> 0x0092B2D0,
//   CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890 (GFRandom inline), list<..>::~list 0x006FE660.
// Tried: 1 grade + 3 multi-variant probes (V1-V9: early `size()==0` return, an inner `return 0`, if-form GFRandom,
//   an up-counting index, a while-advance): VC7.1 inlines ~list at either both or neither of two sites, or out-of-lines
//   the empty path; none out-of-lines only the found path. The same-TU GetAllThings body keeps the list header in esi.
// Shape: the filter temporary lives in the dead `script_name` argument slot; size() is walked twice (the test and the
//   GFRandom range); the pick walks from begin() counting the index down; the found path destroys the list out of
//   line, the empty / run-off path inline (clear + free the header node).
// Layout: no members of `this` are touched (engine/CAIStateGroup_ApprenticeHero.h has no methods to bind).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
template <class T> class allocator { public:
    void deallocate(void* p) { if (p != 0) free(p); }
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T, class A = allocator<T> > class list { public:
    typedef _List_node<T> _Node;
    _Node* _M_node;
    list()
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~list()
    {
        clear();
        A().deallocate(_M_node);
    }
    void clear()
    {
        _Node* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _Node* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    void push_back(const T& x)
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        new (&node->_M_data) T(x);
        _Node* n = _M_node;
        _Node* prev = n->_M_prev;
        node->_M_next = n;
        node->_M_prev = prev;
        prev->_M_next = node;
        n->_M_prev = node;
    }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (_Node* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};
}

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CThingFilter_HasScriptName { public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) {}
};
class CThing { public:
    unsigned char _pad_0x00[0x6c];
    unsigned long ThingTypeFlags;                       // +0x6c
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                            // +0x91
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
class CThingGameObject : public CThing { public: enum { ThingTypeMask = 7 }; };
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
class CThingSearchTools { public:
    std::list<CThing*>& PeekAllThings() const;          // 0x0049C770
    template <class T, class C, class F> void GetAllThings(const F& filter, C& things) const;
};
bool FilterAccepts(const CThingFilter_HasScriptName& filter, const CThing& thing);   // 0x006A7E30
// CThingSearchTools::GetAllThings<CThingGameObject, list<CThingGameObject*>, CThingFilter_HasScriptName> @ 0x0092B2D0,
// compiled in this TU but kept out of line as in retail (VC7.1 then sees it never rewrites the list's header pointer).
#pragma auto_inline(off)
template <class T, class C, class F> void CThingSearchTools::GetAllThings(const F& filter, C& things) const
{
    std::list<CThing*>& all = PeekAllThings();
    for (std::_List_node<CThing*>* it = all._M_node->_M_next; it != all._M_node; it = it->_M_next) {
        CThing* thing = it->_M_data;
        if (thing->IsIncludedInThingScans() && IsThingOfType<T>(thing) && FilterAccepts(filter, *thing))
            things.push_back((T*)thing);
    }
}
#pragma auto_inline(on)

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CAIStateGroupBase { protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
};
class CAIStateGroup_ApprenticeHero : public CAIStateGroupBase { protected:
    CThingGameObject* GetObjectWithScriptName(const CCharString& script_name);
};

CThingGameObject* CAIStateGroup_ApprenticeHero::GetObjectWithScriptName(const CCharString& script_name)
{
    std::list<CThingGameObject*> objects;
    PeekThingSearchTools().GetAllThings<CThingGameObject>(CThingFilter_HasScriptName(script_name), objects);
    if (objects.size() != 0) {
        unsigned long index = GFRandom(objects.size(), GetPWorldSeed());
        for (std::_List_node<CThingGameObject*>* it = objects._M_node->_M_next; it != objects._M_node; it = it->_M_next) {
            if (index == 0)
                return it->_M_data;
            --index;
        }
    }
    return 0;
}
