// CSFindThing(const CCharString& name, map<CCharString, CScriptGameResourceObjectScriptedThingBase>& actors) -- resolve a cutscene actor name to a script thing: "HERO" is the hero, a scripting actor in the map gives its thing, anything else is looked up by script name
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?CSFindThing@@YI?AVCScriptThing@@ABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@@Z
// Naming wave 30 (family cst): the local non-virtual CScriptThing stand-in (??1CScriptThing@@QAE@XZ) is replaced by engine/CScriptThing.h's retail
//   class: ~CScriptThing 0x004AA840 is virtual (RTTI CScriptThing : CBaseClass; Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ)
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF9DE, 218 bytes, `__fastcall` global returning CScriptThing by value
//   (hidden result pointer in ecx, name in edx, the map on the stack: `ret 4`) (PDB ?CSFindThing@@YI?AVCScriptThing@@
//   ABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@...@@@Z). 26 callers in the
//   cutscene macro code 0x00CC2F43..0x00CD01E6.
// Label: the catalog name "GetCutsceneActorScriptThing" is descriptive; the PDB name is CSFindThing.
// Callees: CScriptThing() @ 0x006E7B40, CScriptThing::operator= @ 0x008AB980 (x2), copy constructor @ 0x004ABE90,
//   ~CScriptThing @ 0x004AA840 (x2); Compare(const char*, const char*) @ 0x00411570 (__fastcall, the inlined
//   CBasicString<char>::operator==); the map's out-of-line _M_find @ 0x00CD2B86 (node pointer in eax);
//   CScriptGameResourceObjectScriptedThingBase::IsValid @ 0x00CD23B9 (x2) and its virtual GetScriptThing (slot 12,
//   +0x30, 0x007E7490); CGameScriptInterface (global @ 0x0143E8F8) slot 70 GetHero (+0x118) and slot 72
//   GetThingWithScriptName (+0x120). Literals "HERO" @ 0x01255174, "" @ 0x0122D70E.
// Shape: frame-pointer size TU. The inline CCharString::operator==(const char*) of the landed 0x00E6A000 (both levels
//   `__forceinline`: the size TU calls them out of line otherwise): a null block branches off the strcmp intrinsic's
//   flags (`repe cmpsb` of "HERO" against the pooled ""), else the inlined block compare `Compare(...) ? false : true`
//   whose result byte is homed in the frame ([ebp-1]). The actor test is `it != end && it->second.IsValid() &&
//   IsScriptingActor(&it->second)` with an inline `actor && actor->IsValid()` helper (the `test esi,esi` on the
//   value's address and the second IsValid); a lookup helper returning the address or null leaves a
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
class CCharStringData { public:                        // 17 bytes
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
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                         // : CBaseClass, vtable 0x01238C8C; virtual ~CScriptThing 0x004AA840
class CScriptGameResourceObjectScriptedThingBase {      // 0x10 bytes, vtable 0x0127094C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CScriptThing GetScriptThing() const;        // slot 12 (+0x30)
    bool IsValid() const;                           // 0x00CD23B9 (the implementation pointer at +8 is set)
    void* m_pIntelligentPointer;                        // +0x04
    void* m_pImplementation;                            // +0x08
    void* m_pImplementationInfo;                        // +0x0c
};
namespace std {
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
template <class P> struct _Select1st { };              // STLport 4.6 _function.h (map's KeyOfValue)
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class K, class V, class KoV, class C, class A>
class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    struct iterator {
        _Node* _M_node;
        iterator(_Node* node) : _M_node(node) { }
        bool operator==(const iterator& other) const { return _M_node == other._M_node; }
        bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
        V* operator->() const { return &_M_node->_M_value_field; }
    };
    _Node*       _M_header;                             // +0x00
    unsigned int _M_node_count;                         // +0x04
    C            _M_key_compare;                        // +0x08
    template <class _KT> iterator find(const _KT& key) { return iterator(_M_find(key)); }
    iterator end() { return iterator(_M_header); }
private:
    template <class _KT> _Node* _M_find(const _KT& key) const;   // 0x00CD2B86 (map<CCharString, CScriptGameResourceObjectScriptedThingBase>)
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > >
class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const K& key) { return _M_t.find(key); }
    iterator end() { return _M_t.end(); }
};
}
typedef std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> CActorMap;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

static inline bool IsScriptingActor(const CScriptGameResourceObjectScriptedThingBase* actor)
{
    return actor && actor->IsValid();
}

CScriptThing __fastcall CSFindThing(const CCharString& name, CActorMap& actors)
{
    CScriptThing thing;
    if (name == "HERO") {
        thing = pDSTGame->GetHero();
    } else {
        CActorMap::iterator it = actors.find(name);
        if (it != actors.end() && it->second.IsValid() && IsScriptingActor(&it->second))
            thing = it->second.GetScriptThing();
        else
            thing = pDSTGame->GetThingWithScriptName(name);
    }
    return thing;
}