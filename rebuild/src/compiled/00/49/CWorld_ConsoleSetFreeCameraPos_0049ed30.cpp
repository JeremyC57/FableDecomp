// CWorld::ConsoleSetFreeCameraPos(CWorld&, const vector<CGenericVar>&) -- console command: move the main player's free camera to (args[0], args[1], args[2]) read as floats (static)
// Leaf: ConsoleSetFreeCameraPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049ED30, 65 bytes, plain `ret` (static __fastcall: the world in ecx, the
//   argument vector in edx -> esi). PDB name
//   ?ConsoleSetFreeCameraPos@CWorld@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z.
// Callees: 0x00449970 CPlayerManager::GetMainPlayer() (__fastcall on world.PlayerManager +0x0C, returns CPlayer&).
// Layout: Ego_r.pdb CGenericVar (28 bytes; ValFloat +0x18, so args[0..2] read +0x18 / +0x34 / +0x50 off the STLport
//   vector's _M_start), CPlayer::CurrentFreeCamera +0x40 (CCamera, WorldPosition first), CWorld::PlayerManager +0x0C.
// Shape: `C3DVector(x, y, z)` is a frame temporary (12 bytes, the three floats moved as dwords) handed by reference to
//   the inline `CCamera::MoveTo(const C3DVector&)` (`WorldPosition = pos`); the player is fetched before the arguments
//   are read and `lea` of the camera is the late `add eax,40h`. CCamera has no generated engine header (local model,
//   member names from the landed CCamera copy constructor 0x0049D380).
// Label correction: catalogued as `_global::ConsoleSetFreeCameraPos`; the PDB places it on CWorld (protected static).
#include "engine/CWorld.h"
#include "engine/CPlayer.h"

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CCamera {
public:
    C3DVector WorldPosition;                       // +0x00
    void MoveTo(const C3DVector& pos) { WorldPosition = pos; }
};

class CCharString { public: void* PStringData; };

class CGenericVar { public:
    int         _padding_;                         // +0x00
    long        Type;                              // +0x04 EGenericVarType
    CCharString ValString;                         // +0x08
    long        ValSLONG;                          // +0x0C
    unsigned long ValULONG;                        // +0x10
    bool        ValBool;                           // +0x14
    float       ValFloat;                          // +0x18
};

struct CGenericVarVector {                         // STLport vector<CGenericVar>
    CGenericVar* _M_start;
    CGenericVar* _M_finish;
    CGenericVar* _M_end_of_storage;
    const CGenericVar& operator[](unsigned int n) const { return *(_M_start + n); }
};

struct CPlayerManager_Calls {
    CPlayer& GetMainPlayer();                      // 0x00449970
};

struct CWorld_Methods : CWorld {
    static void __fastcall ConsoleSetFreeCameraPos(CWorld& world, const CGenericVarVector& args);
};

void __fastcall CWorld_Methods::ConsoleSetFreeCameraPos(CWorld& world, const CGenericVarVector& args)
{
    CPlayer& player = reinterpret_cast<CPlayerManager_Calls*>(world.PlayerManager)->GetMainPlayer();
    reinterpret_cast<CCamera&>(player.CurrentFreeCamera).MoveTo(C3DVector(args[0].ValFloat, args[1].ValFloat, args[2].ValFloat));
}