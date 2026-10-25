// CPlayerManager::CPlayerManager(CMainGameComponent&, const CGameDefinitionManager&) -- the non-copyable base, the component and definition-manager pointers, an empty player list, then the four hero-swap player script names ("hero_swap_1.tng" .. "hero_swap_4.tng")
// Leaf: ??0CPlayerManager@@QAE@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044A3B0, 382 bytes (padded to 384), `ret 8`, returns `this`; one caller
//   CMainGameComponent::InitPlayerManager 0x0041732A (landed: `new CPlayerManager(*this, CGameDefinitionManager::Get())`).
// Label: the catalog name HeroSwapState::Initialize is wrong: the vptr stored is 0x01231CD0 (.?AVCPlayerManager@@, slot 0 =
//   the deleting destructor 0x0044C030) and Ego_r.pdb has ??0CPlayerManager@@QAE@AAVCMainGameComponent@@ABVCGameDefinitionManager@@@Z;
//   the HeroSwap name comes from the member HeroSwapPlayerScriptNames.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString(const char*, long) @ 0x0099EBF0,
//   CCharString(const CCharString&) @ 0x0099EC30, vector<CCharString>::_M_insert_overflow @ 0x00433530, ~CCharString @ 0x0099EAE0.
// Layout: as the landed ~CPlayerManager 0x0044A140 (engine/CPlayerManager.h carries the 16-byte-vector PDB offsets, so the class
//   is modelled locally): Component +4, DefinitionManager +8, Players +0xc, PlayerNeutral +0x18, MainPlayer +0x1c (neither is
//   set here), HeroSwapPlayerScriptNames +0x20 (12-byte STLport vectors).
// Shape: STLport push_back inline (placement copy with its null test, `_M_finish += 1` through the CSE'd element size in ebp);
//   each name is a named local in its own scope; the overflow call gets a same-TU stand-in that never touches the
//   __false_type tag, so VC homes the tag on the name's own slot as retail does.
#include <stddef.h>

class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
inline void* operator new(size_t, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };
template <class T> class allocator { public: allocator() { } };
template <class T, class Alloc> class _STLP_alloc_proxy {
public:
    T _M_data;
    _STLP_alloc_proxy(const Alloc&, T p) : _M_data(p) { }
};
template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, Alloc> _M_end_of_storage;
    _Vector_base(const Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    vector() : _Vector_base<T, Alloc>(Alloc()) { }
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00433530
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) T(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1, true);
    }
};
// Same-TU stand-in for the out-of-line overflow (the real body is 0x00433530 in another TU): it never reads the tag.
#pragma auto_inline(off)
template <> __declspec(noinline) void vector<CCharString, allocator<CCharString> >::_M_insert_overflow(CCharString* pos,
    const CCharString& x, const __false_type&, unsigned int fillLen, bool atEnd)
{
    for (; fillLen != 0; --fillLen)
        new (pos++) CCharString(x);
    if (atEnd)
        this->_M_finish = pos;
}
#pragma auto_inline(on)
}

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CMainGameComponent;
class CGameDefinitionManager;
class CPlayer;
class CPlayerManager : public CBaseClassNonCopyable {
public:
    CMainGameComponent*           Component;             // +0x04
    const CGameDefinitionManager* DefinitionManager;     // +0x08
    std::vector<CPlayer*>        Players;               // +0x0c
    long                          PlayerNeutral;         // +0x18
    long                          MainPlayer;            // +0x1c
    std::vector<CCharString>     HeroSwapPlayerScriptNames;   // +0x20
    CPlayerManager(CMainGameComponent& component, const CGameDefinitionManager& defs);
    virtual ~CPlayerManager();                           // 0x0044A140
};

CPlayerManager::CPlayerManager(CMainGameComponent& component, const CGameDefinitionManager& defs)
    : Component(&component), DefinitionManager(&defs)
{
    {
        CCharString name("hero_swap_1.tng");
        HeroSwapPlayerScriptNames.push_back(name);
    }
    {
        CCharString name("hero_swap_2.tng");
        HeroSwapPlayerScriptNames.push_back(name);
    }
    {
        CCharString name("hero_swap_3.tng");
        HeroSwapPlayerScriptNames.push_back(name);
    }
    {
        CCharString name("hero_swap_4.tng");
        HeroSwapPlayerScriptNames.push_back(name);
    }
}