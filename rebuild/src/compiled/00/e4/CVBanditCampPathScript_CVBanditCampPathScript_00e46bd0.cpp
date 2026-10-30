// CV_BanditCampPathScript::CV_BanditCampPathScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, owner/interface pair, vtable 0x012E4A9C, empty Bandits vector, two empty CScriptThing members
// Leaf: ??0CV_BanditCampPathScript
// Retail: Fable.exe @ 0x00E46BD0, 93 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (manifest label "CHeroMorphDef" is propagated, see GOTCHAS),
// CBaseClass::CBaseClass @ 0x0099A2D0 (the CScriptThing root, twice); CScriptThing vtable 0x01238C8C.
// Layout: Ego_r.pdb CV_BanditCampPathScript - 0x14: OverheardBanditConversation +0x48, Bandits (STLport vector, 12 bytes)
// +0x4c, Bandit1 (CScriptThing, 12 bytes) +0x58, Bandit2 +0x64.
// Shape: the 0x00E68F70 script-object model (arguments stored by an inlined `__declspec(novtable)` intermediate base
// before the vtable); the Bandits vector is the real STLport structure (_Vector_base(const allocator&) with
// _M_start(0), _M_finish(0) and the end-of-storage word in an _STLP_alloc_proxy built from an `allocator()`
// temporary) -- a plain three-zero vector moves `mov ecx,edi` above the zero stores and swaps the two argument
// loads (the fleet's "24i" shape); its zero words share ebx with the CScriptThing members' two zero words each,
// whose inline constructor runs CBase(), stores its vtable, then the PImp member object's inline constructor zeroes
// Data/Info (a member OBJECT keeps both stores after the vtable store; plain init-list members sink it).
class CGameScriptInterface;

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing;
class CCPPointerInfo;

struct CCountedPointer_CScriptThing {                // CScriptThing::PImp
    CScriptThing* Data;                              // +0
    CCPPointerInfo* Info;                            // +4
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};

class CScriptThing : public CBaseClass { public:                        // vtable 0x01238C8C
    CCountedPointer_CScriptThing PImp;               // +0x04
    CScriptThing() { }
    virtual ~CScriptThing();
};

template <class T>
struct allocator {
    allocator() { }
};

template <class T, class A>
struct _STLP_alloc_proxy : A {                       // STLport: the end-of-storage pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};

template <class T>
struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T>
struct CRetailVector : _Vector_base<T> {             // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};

struct CV_BanditCampPathScript : CScriptObject {     // vtable 0x012E4A9C
    bool OverheardBanditConversation;                // +0x48
    CRetailVector<CScriptThing> Bandits;             // +0x4c
    CScriptThing Bandit1;                            // +0x58
    CScriptThing Bandit2;                            // +0x64
    virtual void v00();
    CV_BanditCampPathScript(void* owner, CGameScriptInterface* game);
};

CV_BanditCampPathScript::CV_BanditCampPathScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}