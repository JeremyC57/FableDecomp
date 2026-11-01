// StaticInitialization::DynamicInitializer_GGame() -- `dynamic initializer for 'GGame'`: construct the global CGame (CInitBaseClass base, then CGame's vtable) and register its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x01219F00, 32 bytes: mov ecx,0x013B83D0 ; call 0x0099A310 ; push 0x01228930 ;
//   mov dword ptr [0x013B83D0],0x0122E59C ; call _atexit 0x004012BC ; pop ecx ; ret.
// Identity (proven): compiler-emitted, not a library object: the only reference is its .CRT$XCU slot at 0x01374218 (the
//   C++ initializer table), it constructs an engine object (GGame 0x013B83D0, rebuild/candidates/manual/naming/
//   ESTABLISHED_SPELLINGS.tsv ?GGame@@3VCGame@@A) with the CGame vtable 0x0122E59C (RTTI .?AVCGame@@ : CInitBaseClass :
//   CBaseClass, rtti_bases.tsv) after CInitBaseClass::CInitBaseClass 0x0099A310, and registers the atexit thunk 0x01228930
//   (`mov ecx,0x013B83D0 ; jmp 0x0099A410` = ~CInitBaseClass: CGame's destructor is the compiler-generated one). Ego_r.pdb
//   and FableWin.pdb both name `dynamic initializer for 'GGame'` (and `dynamic atexit destructor for 'GGame'`), between the
//   initializers of RelaunchSaveGameName and GDebugString -- the retail .CRT$XCU order (0x01219EE0, 0x01219F00, 0x01219F20).
// Callees: CInitBaseClass::CInitBaseClass() 0x0099A310, _atexit 0x004012BC (the `_$E2` thunk 0x01228930 is emitted with it).
// Layout: Ego_r.pdb CInitBaseClass (8): vptr, Valid +0x4; CGame (0x210): CurrentGameComponent +0x8, ParameterBuffer[512]
//   +0xC, Quit +0x20C. fable_game.h's CGame is a flat non-polymorphic record and there is no engine/CGame.h or
//   engine/CInitBaseClass.h, so the chain is modelled here over engine/CBaseClass.h.
// Shape: CGame's default constructor is inline and initialises no member (only the base ctor call and the vtable store);
//   CGame declares no destructor, so the atexit thunk tail-jumps to ~CInitBaseClass.
#include "engine/CBaseClass.h"
class CGameComponent;

class CInitBaseClass : public CBaseClass {                 // vtable 0x0129A7CC
public:
    CInitBaseClass();                                      // 0x0099A310 (??0CInitBaseClass@@QAE@XZ)
    virtual ~CInitBaseClass();                             // 0x0099A410 (??1CInitBaseClass@@UAE@XZ)
    virtual void Validate() const;                         // ?Validate@CInitBaseClass@@UBEXXZ
    bool Valid;                                            // +0x04
};

class CGame : public CInitBaseClass {                      // vtable 0x0122E59C
public:
    CGame() { }                                            // FableWin ??0CGame@@QAE@XZ (inline here)
    bool Initialise();                                     // ?Initialise@CGame@@QAE_NXZ
    void Uninitialise();                                   // ?Uninitialise@CGame@@QAEXXZ
    void Play();                                           // ?Play@CGame@@QAEXXZ
    void SetQuit(bool quit);                               // ?SetQuit@CGame@@QAEX_N@Z
    CGameComponent* CurrentGameComponent;                  // +0x008
    unsigned char   ParameterBuffer[512];                  // +0x00C
    bool            Quit;                                  // +0x20C
};

CGame GGame;                                               // 0x013B83D0