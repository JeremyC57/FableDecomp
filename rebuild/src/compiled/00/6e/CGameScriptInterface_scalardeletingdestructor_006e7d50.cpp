// CGameScriptInterface::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CGameScriptInterface inlined (destroying ScriptTimers, HeroSThing, then ~CBaseClassNonCopyable), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCGameScriptInterface@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7D50, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: CGameScriptInterface -- proven; evidence: vtable 0x01260F0C slot 0 (RTTI .?AVCGameScriptInterface@@ : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass); the destroyed members are Ego_r.pdb ScriptTimers (map<long, long>, +0x3C; 0x004DF885 is ~map, a tail jump into the tree's destructor) and HeroSThing (CScriptThing, +0x30; 0x004AA840 stores the CScriptThing vtable 0x01238C8C); every base destructor down to CBaseClassNonCopyable is implicit and CBaseClassNonCopyable::~CBaseClassNonCopyable is out of line (0x0099A300, FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ: the CBaseClass-derived classes' destructors call 0x0099A2E0 instead, e.g. CBaseClass's own deleting destructor 0x004371F0).
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: map<long, long>::~map 0x004DF885, CScriptThing::~CScriptThing 0x004AA840, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors, and every destructor between this class
//   and CBaseClassNonCopyable is implicit (CBaseClassNonCopyable's own one stores its vtable); member destructors run in reverse declaration order.
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more.
// Model: local -- engine/CGameScriptInterface.h exists but is data-only (vptr flattened into a word), which cannot carry the virtual destructor chain.
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                       // STLport: the _Rb_tree (header proxy, node count, compare)
public:
    unsigned char _M_t[12];
    map();
    ~map();
};
}
class CBaseClass {                                // RTTI root, vtable 0x01231710
public:
    virtual ~CBaseClass();                        // 0x0099A2E0 (FableWin.pdb ??1CBaseClass@@UAE@XZ)
};
class CBaseClassNonCopyable : public CBaseClass { // vtable 0x0129A7C4
protected:
    CBaseClassNonCopyable();                      // 0x0099A2F0 (FableWin.pdb ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();             // 0x0099A300 (FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CWorld; class CMainGameComponent; class CDisplayEngine; class CGameDefinitionManager; class CPlayerManager;
class CThingSearchTools;
class CScriptThing : public CBaseClass {          // Ego_r.pdb 12 bytes
public:
    CScriptThing();
    virtual ~CScriptThing();                      // 0x004AA840
    unsigned char _data[8];
};
class CGameScriptInterfaceBase : public CBaseClassNonCopyable { };
class CGameScriptInterface : public CGameScriptInterfaceBase {
public:
    CWorld* World;                                // +0x04
    CMainGameComponent* Component;                // +0x08
    CDisplayEngine* DisplayEngine;                // +0x0c
    CGameDefinitionManager* DefinitionManager;    // +0x10
    CPlayerManager* PlayerManager;                // +0x14
    CThingSearchTools* ThingSearchTools;          // +0x18
    long CurrentPlayer;                           // +0x1c
    long CurrentLevelID;                          // +0x20
    long CurrentScriptLevelID;                    // +0x24
    long CurrentScriptID;                         // +0x28
    bool InMovieSequence;                         // +0x2c
    bool AllowScreenFadingIfAlreadyFaded;         // +0x2d
    CScriptThing HeroSThing;                      // +0x30
    std::map<long, long> ScriptTimers;            // +0x3c
    long CameraResetToViewBehindHeroCount;        // +0x48
    long CreateCreatureDelayFrames;               // +0x4c
};
void* MakeOne()
{
    return new CGameScriptInterface;
}