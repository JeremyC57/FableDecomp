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
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterface;
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

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