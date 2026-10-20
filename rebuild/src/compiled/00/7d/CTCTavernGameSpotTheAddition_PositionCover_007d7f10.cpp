// CTCTavernGameSpotTheAddition::PositionCover(float fraction) -- swings the cover over the board along an eased arc: the eased fraction picks the angle, the cover sits (1 - cos) along and sin up from its board position
// Leaf: PositionCover
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D7F10, 187 bytes (`ret 4`). Ego_r.pdb
// `?PositionCover@CTCTavernGameSpotTheAddition@@AAEXM@Z`.
// Callees: CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef> 0x007D9A80 through the inline GetDef (ParentThing's
//          def at +0x70); CTCTavernGame::PositionMesh(CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&,
//          float, float) 0x005EABC0 (Ego_r.pdb: static `__fastcall`; the landed callers 0x007D4E30 / 0x007D60C0 /
//          0x008EF2F0 declare it as a global with three different trailing types); the def's slot 1 on release.
//          Constants: 0.5f 0x0122F59C, (double)TWO_PI 0x0124F2B8, 1.0 0x01237F00, 0.5 0x012316F0.
// Layout: retail CTCTavernGameSpotTheAddition (Ego_r.pdb - 0x18, CTCTavernGame is 0xf0 here): Cover +0xf8, BoardPos +0x11c,
//         BoardRHSet +0x128; CTavernGameSpotTheAdditionDef: CoverLength +0x34 (PDB offset), LengthMovementScale +0x7c and
//         HeightMovementScale +0x80 (Ego_r.pdb - 8: two 12-byte vectors before them).
// Shape: the eased fraction `(1 - cos(pi * f)) * 0.5`, the arc angle and the two offsets are float locals that VC7.1
//        keeps unrounded on the x87 stack (no /Op): `fraction * 0.5f * (double)TWO_PI` keeps both multiplies, the float
//        `eased * 0.5f` multiplies by the dword 0.5f before the double 2*pi, the angle is duplicated for cos and sin; the
//        offset is a C3DVector temporary built after the two trailing float pushes; the def stays in esi to the short `dec [esi+4] ; jne` release only when VC7.1 sees the
//        GetSubDef body (a stand-in compiled above under `#pragma auto_inline(off)`); only PositionCover is graded.
#include <math.h>
#pragma intrinsic(cos, sin)

const float TWO_PI = 6.2831855f;

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet { float Up[3], Forward[3]; };
struct CThing;

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameSpotTheAdditionDef : CDefVirtual {
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0x28];
    float CoverWidth;                           // +0x30
    float CoverLength;                          // +0x34
    unsigned char _pad_0x38[0x44];
    float LengthMovementScale;                  // +0x7c
    float HeightMovementScale;                  // +0x80
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CSpotTheAdditionDefPointer {             // CDefPointer<const CTavernGameSpotTheAdditionDef>
    CTavernGameSpotTheAdditionDef* Object;
    CSpotTheAdditionDefPointer() : Object(0) {}
    ~CSpotTheAdditionDefPointer() { if (Object) Object->Release(); }
    CTavernGameSpotTheAdditionDef* operator->() const { return Object; }
};
struct CDefinitionManager {
    CTavernGameSpotTheAdditionDef* PeekDefFromGlobalIndex(long index);   // stand-in only
};
struct CDefClassBase {
    virtual long* GetSubDefIndex(const char* name);       // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef_007d9a80(CSpotTheAdditionDefPointer& out);  // 0x007D9A80 (GetSubDef<CTavernGameSpotTheAdditionDef>)
};

// Stand-in for the same-module GetSubDef<CTavernGameSpotTheAdditionDef> body (0x007D9A80; not graded here): compiled
// above the caller without inlining so VC7.1 sees, as retail's whole-program build did, that the def pointer it fills
// does not escape.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef_007d9a80(CSpotTheAdditionDefPointer& out)
{
    long* index = GetSubDefIndex("CTavernGameSpotTheAdditionDef");
    if (index == 0)
        return false;
    CTavernGameSpotTheAdditionDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
    if (out.Object != def) {
        if (out.Object)
            out.Object->Release();
        out.Object = def;
        if (def)
            ++def->m_count;
    }
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                        // +0x70
};

struct CTCTavernGame {
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0xe8];
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float a, float b);   // 0x005EABC0
};
struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    long          CurrentRound;                 // +0xf0
    float         CurrentTime;                  // +0xf4
    CThing*       Cover;                        // +0xf8
    float         CoverTimer;                   // +0xfc
    bool          UpdatedKnickKnacks;           // +0x100
    void*         Pointer;                      // +0x104 CThingObject*
    float         PointerScale;                 // +0x108
    float         DistFromObject;               // +0x10c
    unsigned char KnickKnacks[0xc];             // +0x110 vector<KnickKnack>
    C3DVector     BoardPos;                     // +0x11c
    CRightHandedSet BoardRHSet;                 // +0x128
    void GetDef(CSpotTheAdditionDefPointer* out) { ParentThing->PDef->GetSubDef_007d9a80(*out); }
    void PositionCover(float fraction);
};

void CTCTavernGameSpotTheAddition::PositionCover(float fraction)
{
    if (Cover) {
        CSpotTheAdditionDefPointer def;
        GetDef(&def);
        float eased = (float)((1.0 - cos(fraction * 0.5f * (double)TWO_PI)) * 0.5);
        float angle = (float)(eased * 0.5f * (double)TWO_PI);
        float along = (float)((1.0 - cos(angle)) * def->LengthMovementScale * def->CoverLength);
        float up = (float)(sin(angle) * def->HeightMovementScale * def->CoverLength);
        PositionMesh(Cover, BoardPos, BoardRHSet, C3DVector(0.0f, along, up), 0.0f, 0.0f);
    }
}