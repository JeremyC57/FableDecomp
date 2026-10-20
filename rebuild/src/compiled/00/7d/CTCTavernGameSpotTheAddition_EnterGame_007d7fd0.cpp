// CTCTavernGameSpotTheAddition::EnterGame() -- tavern-game EnterGame, camera onto the def's camera point, timer reset, then the pointer piece is created from the game def and placed at the game position raised by the pointer height
// Leaf: ?EnterGame@CTCTavernGameSpotTheAddition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D7FD0, 234 bytes (plain `ret`); CTCTavernGameSpotTheAddition vtable
// 0x0126F814 slot 29. Ego_r.pdb `?EnterGame@CTCTavernGameSpotTheAddition@@UAEXXZ`.
// Callees: CTCTavernGame::EnterGame() 0x005EC0F0 (non-virtual base call); CTCTavernGame::GetTavernGameDef(CDefPointer*)
//          0x005EA000 (Ego_r.pdb GetDef, out of line in this TU); CDefStringTable::GetString(long) 0x009D49B0 on
//          g_DefStringTable 0x013CA828; CTCTavernGame::CameraUseCameraPoint(const CCharString&) 0x005EBFA0;
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
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet { float Up[3], Forward[3]; };
#pragma pack(pop)

struct CThing;
struct CThingObject;

struct CCharString {
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x88];
    long CameraName;                            // +0x90 CDefString
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {                            // CDefPointer<const CTavernGameDef>
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};

struct CTavernGameSpotTheAdditionDef : CDefVirtual {
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0x28];
    float CoverWidth;                           // +0x30
    float CoverLength;                          // +0x34
    unsigned char _pad_0x38[0x2c];
    long  PointerModel;                         // +0x64
    float PointerHeightOffset;                  // +0x68
    unsigned char _pad_0x6c[0x10];
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
    bool GetSubDef_007d9a80(CSpotTheAdditionDefPointer& out);   // 0x007D9A80 (GetSubDef<CTavernGameSpotTheAdditionDef>)
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
    void EnterGame();                           // 0x005EC0F0
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    void CameraUseCameraPoint(const CCharString& name);   // 0x005EBFA0
    CThingObject* CreateGamePiece(long model);  // 0x005EA9B0
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float a, float b);   // 0x005EABC0
};
struct CThingObject : CThing { };

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
    void GetDef(CSpotTheAdditionDefPointer* out) { ParentThing->PDef->GetSubDef_007d9a80(*out); }
    void EnterGame();
};

void CTCTavernGameSpotTheAddition::EnterGame()
{
    CTCTavernGame::EnterGame();
    CDefPointer gameDef;
    GetTavernGameDef(&gameDef);
    {
        CCharString camera = g_DefStringTable.GetString(gameDef->CameraName);
        CameraUseCameraPoint(camera);
    }
    CSpotTheAdditionDefPointer def;
    GetDef(&def);
    CurrentTime = 0;
    Pointer = CreateGamePiece(def->PointerModel);
    PositionMesh(Pointer, GamePositionPos, GamePositionRHSet, C3DVector(0.0f, 0.0f, def->PointerHeightOffset), 0.0f, 0.0f);
}