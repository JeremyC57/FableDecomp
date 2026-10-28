// CAIStateGroup_GoToPub::GetObjectWithScriptName(const CCharString& script_name) -- collect every object carrying the script name and return one picked with the world's random seed (null when there are none)
// Grade: DIFFER(265v242) -- twin of 0x00929FC0 (byte-identical shape): the found path's list<..>::~list is inlined (retail calls 0x00792240 out of line); GFRandom folds to xor edx / div where retail branches into ecx
// Leaf: ?GetObjectWithScriptName@CAIStateGroup_GoToPub@@IAEPAVCThingObject@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947EB0, length 242 bytes, `ret 4`. Ego_r.pdb name; donor 0x0096E1B0.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes), CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0 (the CThingFilter_HasScriptName temporary), CAIStateGroupBase::PeekThingSearchTools 0x008FD000,
//   CThingSearchTools::GetAllThings<CThingObject, list<CThingObject*>, CThingFilter_HasScriptName> 0x0094B490 (type mask
//   0x807), CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890 (GFRandom inline), list<..>::~list 0x00792240.
// Shape: as 0x00929FC0 (see differ/00929fc0.cpp for the variants tried there).
// Layout: no members of `this` are touched.
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
class CThingObject : public CThing { public: enum { ThingTypeMask = 0x807 }; };
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
// CThingSearchTools::GetAllThings<CThingObject, list<CThingObject*>, CThingFilter_HasScriptName> @ 0x0094B490,
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
class CAIStateGroup_GoToPub : public CAIStateGroupBase { protected:
    CThingObject* GetObjectWithScriptName(const CCharString& script_name);
};

CThingObject* CAIStateGroup_GoToPub::GetObjectWithScriptName(const CCharString& script_name)
{
    std::list<CThingObject*> objects;
    PeekThingSearchTools().GetAllThings<CThingObject>(CThingFilter_HasScriptName(script_name), objects);
    if (objects.size() != 0) {
        unsigned long index = GFRandom(objects.size(), GetPWorldSeed());
        for (std::_List_node<CThingObject*>* it = objects._M_node->_M_next; it != objects._M_node; it = it->_M_next) {
            if (index == 0)
                return it->_M_data;
            --index;
        }
    }
    return 0;
}
