// CSFindThing(const CCharString& name, map<CCharString, CScriptGameResourceObjectScriptedThingBase>& actors) -- resolve a cutscene actor name to a script thing: "HERO" is the hero, a scripting actor in the map gives its thing, anything else is looked up by script name
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?CSFindThing@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF9DE, 218 bytes, `__fastcall` global returning CScriptThing by value
//   (hidden result pointer in ecx, name in edx, the map on the stack: `ret 4`) (PDB ?CSFindThing@@YI?AVCScriptThing@@
//   ABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@...@@@Z). 26 callers in the
//   cutscene macro code 0x00CC2F43..0x00CD01E6.
// Label: the catalog name "GetCutsceneActorScriptThing" is descriptive; the PDB name is CSFindThing.
// Callees: CScriptThing() @ 0x006E7B40, CScriptThing::operator= @ 0x008AB980 (x2), copy constructor @ 0x004ABE90,
//   ~CScriptThing @ 0x004AA840 (x2); Compare(const char*, const char*) @ 0x00411570 (__fastcall, the inlined
//   CBasicString<char>::operator==); the map's out-of-line _M_find @ 0x00CD2B86 (node pointer in eax);
//   CScriptGameResourceObjectScriptedThingBase::IsScripting @ 0x00CD23B9 (x2) and its virtual GetScriptThing (slot 12,
//   +0x30, 0x007E7490); CGameScriptInterface (global @ 0x0143E8F8) slot 70 GetHero (+0x118) and slot 72
//   GetThingWithScriptName (+0x120). Literals "HERO" @ 0x01255174, "" @ 0x0122D70E.
// Shape: frame-pointer size TU. The inline CCharString::operator==(const char*) of the landed 0x00E6A000 (both levels
//   `__forceinline`: the size TU calls them out of line otherwise): a null block branches off the strcmp intrinsic's
//   flags (`repe cmpsb` of "HERO" against the pooled ""), else the inlined block compare `Compare(...) ? false : true`
//   whose result byte is homed in the frame ([ebp-1]). The actor test is `it != end && it->second.IsScripting() &&
//   IsScriptingActor(&it->second)` with an inline `actor && actor->IsScripting()` helper (the `test esi,esi` on the
//   value's address and the second IsScripting); a lookup helper returning the address or null leaves a
//   `xor esi,esi` that retail threads away. The two by-value thing sources share the frame temporary and one
//   assignment + destructor tail; no NRVO, the local is copied into the hidden result and destroyed.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    __forceinline bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CScriptThing {                            // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing();                                     // 0x006E7B40
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
    void* m_vtable;
    void* m_pImpData;
    void* m_pImpInfo;
};
class CScriptGameResourceObjectScriptedThingBase {      // 0x10 bytes, vtable 0x0127094C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CScriptThing GetScriptThing() const;        // slot 12 (+0x30)
    bool IsScripting() const;                           // 0x00CD23B9 (the implementation pointer at +8 is set)
    void* m_pIntelligentPointer;                        // +0x04
    void* m_pImplementation;                            // +0x08
    void* m_pImplementationInfo;                        // +0x0c
};
namespace _STL {
template <class T> struct less {
    bool operator()(const T& a, const T& b) const;
};
template <class T> class allocator { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                   // +0x10
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > >
class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    struct iterator {
        _Node* _M_node;
        iterator(_Node* node) : _M_node(node) { }
        bool operator==(const iterator& other) const { return _M_node == other._M_node; }
        bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
        value_type* operator->() const { return &_M_node->_M_value_field; }
    };
    _Node*       _M_header;                             // +0x00
    unsigned int _M_node_count;                         // +0x04
    C            _M_key_compare;                        // +0x08
    _Node* _M_find(const K& key) const;                 // 0x00CD2B86 (map<CCharString, CScriptGameResourceObjectScriptedThingBase>)
    iterator find(const K& key) { return iterator(_M_find(key)); }
    iterator end() { return iterator(_M_header); }
};
}
typedef _STL::map<CCharString, CScriptGameResourceObjectScriptedThingBase> CActorMap;

struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();                                            // slot 70 (+0x118)
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const; // slot 72 (+0x120)
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

static inline bool IsScriptingActor(const CScriptGameResourceObjectScriptedThingBase* actor)
{
    return actor && actor->IsScripting();
}

CScriptThing __fastcall CSFindThing(const CCharString& name, CActorMap& actors)
{
    CScriptThing thing;
    if (name == "HERO") {
        thing = g_pGameScriptInterface->GetHero();
    } else {
        CActorMap::iterator it = actors.find(name);
        if (it != actors.end() && it->second.IsScripting() && IsScriptingActor(&it->second))
            thing = it->second.GetScriptThing();
        else
            thing = g_pGameScriptInterface->GetThingWithScriptName(name);
    }
    return thing;
}