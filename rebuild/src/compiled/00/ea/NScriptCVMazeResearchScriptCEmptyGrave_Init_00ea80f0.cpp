// NScript::CV_MazeResearchScript::CEmptyGrave::Init() -- clear the maze research script's UNLIMBO cutscene flag
// Leaf: ?Init@CEmptyGrave@CV_MazeResearchScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA80F0, 51 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012EE4C0, which the landed
//   NScript::CV_MazeResearchScript::CEmptyGrave::Alloc 0x00EA7990 stores (site 0x00EA79C6); Ego_r.pdb has ?Init@CEmptyGrave@CV_MazeResearchScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CEmptyGrave (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("UNLIMBO" 0x012EE5D4); std::map<CCharString, bool>::operator[] 0x008ADF10
//   (catalog std::map::operator[]).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class T> struct less { };
template <class T1, class T2> struct pair { };
template <class T> class allocator { };
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:                                                 // STLport 4.6 map: the _Rb_tree (header +0, node count +4, compare +8)
    void*         _M_header;
    unsigned long _M_node_count;
    C             _M_key_compare;
    T& operator[](const K& key);                        // 0x008ADF10 for map<CCharString, bool>
};
}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CGameScriptInterface;
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MazeResearchScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool         SwordTaken;                            // +0x48
    bool         BookRead;                              // +0x49
    unsigned char Sword[0xc];                           // +0x4c CScriptThing
    std::map<CCharString, bool> cs_flags;               // +0x58
    class CEmptyGrave;
};
class CV_MazeResearchScript::CEmptyGrave : public CEntityScriptBase {
public:
    CV_MazeResearchScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Init();
};

void CV_MazeResearchScript::CEmptyGrave::Init()
{
    ParentClass->cs_flags["UNLIMBO"] = false;
}
}