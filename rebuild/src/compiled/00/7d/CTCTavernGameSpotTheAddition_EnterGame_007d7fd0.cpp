// CTCTavernGameSpotTheAddition::EnterGame() -- tavern-game EnterGame, camera onto the def's camera point, timer reset, then the pointer piece is created from the game def and placed at the game position raised by the pointer height
// Leaf: ?EnterGame@CTCTavernGameSpotTheAddition
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D7FD0, 234 bytes (plain `ret`); CTCTavernGameSpotTheAddition vtable
// 0x0126F814 slot 29. Ego_r.pdb `?EnterGame@CTCTavernGameSpotTheAddition@@UAEXXZ`.
// Callees: CTCTavernGame::EnterGame() 0x005EC0F0 (non-virtual base call); CTCTavernGame::GetDef(CDefPointer*)
//          0x005EA000 (Ego_r.pdb GetDef, out of line in this TU); CDefStringTable::GetString(long) 0x009D49B0 on
//          GDefStringTable 0x013CA828; CTCTavernGame::CameraUseCameraPoint(const CCharString&) 0x005EBFA0;
//          ~CCharString 0x0099EAE0; CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef> 0x007D9A80 (the inline
//          GetDef of this class); CTCTavernGame::CreateGamePiece(long) 0x005EA9B0; CTCTavernGame::PositionMesh(CThing*,
//          const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float) 0x005EABC0 (static `__fastcall`);
//          both defs' slot 1 on release.
// Layout: retail CTCTavernGameSpotTheAddition (Ego_r.pdb - 0x1c from FirstRound on): CurrentTime +0xf4, Pointer +0x104,
//         FirstRound +0x14c, GamePositionRHSet +0x14d and GamePositionPos +0x165 (unaligned in the PDB too: the vector
//         classes are `#pragma pack(1)`); CTavernGameDef::CameraName +0x90; CTavernGameSpotTheAdditionDef PointerModel
//         +0x64, PointerHeightOffset +0x68 (Ego_r.pdb - 8 after its two vectors).
// Shape: the camera name is a named CCharString in its own block, sharing its frame word with the class def pointer
//        declared after it; the class def stays in edi to the short `dec [edi+4] ; jne` release only when VC7.1 sees
//        the GetSubDef body (stand-in above under `#pragma auto_inline(off)`, as PositionCover 0x007D7F10); the game def
//        released last through its inline null-tested ReleaseTemporary(); the offset is a C3DVector temporary
//        (0, 0, height) built after the two trailing float pushes. Only EnterGame is graded.
// PositionMesh (sweep3, FINDINGS chunk 481 "PositionMesh's 5th parameter is float"): Ego_r/FableWin
//   ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z -- a static __fastcall member taking
//   (CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float); C3DVector, CRightHandedSet and CThing
//   are classes (V in every PDB signature).
// NF49 (round 7): the def classes come from the shared headers (were local CDefPointer<const CTavernGameDef> / CCharString / CDefStringTable / CRefCounted / CDefVirtual / CTavernGameDef / CTavernGameSpotTheAdditionDef / CSpotTheAdditionDefPointer / CDefinitionManager / CDefClassBase models and a same-TU
// stand-in GetSubDef body): the call at 0x007D9A80 is the member template
// ??$GetSubDef@VCTavernGameSpotTheAdditionDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameSpotTheAdditionDef@@@@@Z (Ego_r.pdb / FableWin.pdb), its body visible
// and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameSpotTheAdditionDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#pragma pack(push, 1)
#include "engine/C3DVector.h"
class CRightHandedSet { public: float Up[3], Forward[3]; };
#pragma pack(pop)

class CThing;
class CThingObject;





class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                        // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0xe8];
    void EnterGame();                           // 0x005EC0F0
    void GetDef(CDefPointer<const CTavernGameDef>* out);    // 0x005EA000
    void CameraUseCameraPoint(const CCharString& name);   // 0x005EBFA0
    CThingObject* CreateGamePiece(long model);  // 0x005EA9B0
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float tilt, float angle);   // 0x005EABC0
};
class CThingObject : public CThing { public: };

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    long          CurrentRound;                 // +0xf0
    float         CurrentTime;                  // +0xf4
    CThing*       Cover;                        // +0xf8
    float         CoverTimer;                   // +0xfc
    bool          UpdatedKnickKnacks;           // +0x100
    CThingObject* Pointer;                      // +0x104
    float         PointerScale;                 // +0x108
    float         DistFromObject;               // +0x10c
    unsigned char KnickKnacks[0xc];             // +0x110 vector<KnickKnack>
    C3DVector     BoardPos;                     // +0x11c
    CRightHandedSet BoardRHSet;                 // +0x128
    long          HeldTime;                     // +0x140
    long          Selected;                     // +0x144
    CThingObject* NewKnickKnack;                // +0x148
    bool          FirstRound;                   // +0x14c
    CRightHandedSet GamePositionRHSet;          // +0x14d
    C3DVector     GamePositionPos;              // +0x165
    void GetDef(CDefPointer<const CTavernGameSpotTheAdditionDef>* out) { ParentThing->PDef->GetSubDef(out); }
    void EnterGame();
};

void CTCTavernGameSpotTheAddition::EnterGame()
{
    CTCTavernGame::EnterGame();
    CDefPointer<const CTavernGameDef> gameDef;
    CTCTavernGame::GetDef(&gameDef);
    {
        CCharString camera = gameDef->CameraName.GetString()   /* NF49: CDefString::GetString, GDefStringTable.GetString(TablePos) in place */;
        CameraUseCameraPoint(camera);
    }
    CDefPointer<const CTavernGameSpotTheAdditionDef> def;
    GetDef(&def);
    CurrentTime = 0;
    Pointer = CreateGamePiece(def->PointerModel);
    CTCTavernGame::PositionMesh(Pointer, GamePositionPos, GamePositionRHSet, C3DVector(0.0f, 0.0f, def->PointerHeightOffset), 0.0f, 0.0f);
}