// CAIStateGroup_JackDragon::IsTargetOnATower() -- is the enemy standing higher than the dragon's landing marker?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094FBB0, length 65 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PLandedMarker at +0x1c, then PEnemy at +0x14),
// CThing::GetPosition @ 0x004C73D0 twice (manifest "CVertexBufferWin32::DoSizeof" is propagated; it returns the
// C3DVector* whose Z is read).
//
// Shape: both things are fetched into locals first (marker in edi, enemy in eax); the comparison is written
// `enemy.Z > marker.Z + 1.5f`, so VC evaluates the enemy's position first (left operand) and the marker's
// last (its Z is used straight from the returned pointer), compares `marker.Z + 1.5f` against `enemy.Z` as
// the ordered `<` (`fcomp ; test ah,5 ; jp` to the false tail). The 1.5f is a float LITERAL (pool constant at
// 0x0125AADC, value 1.5): a literal is added after the member load (`fld [eax+8] ; fadd [pool]`), an extern
// global would be loaded first.
// `mov al,1` / `xor al,al`: bool. The engine header keeps the pointers as opaque byte arrays, so the
// layout is modelled locally with their names.
struct C3DVector { float X, Y, Z; };

struct CThing {
    C3DVector* GetPosition();                   // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem();                         // 0x00A01B50
};

struct CAIStateGroupBase {
    virtual void v00();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CBaseIntelligentPointer PLandedMarker;      // +0x1c CIntelligentPointer<CThing>
    bool IsTargetOnATower();
};

bool CAIStateGroup_JackDragon::IsTargetOnATower()
{
    CThing* pMarker = PLandedMarker.GetPItem();
    CThing* pEnemy = PEnemy.GetPItem();
    if (pEnemy->GetPosition()->Z > pMarker->GetPosition()->Z + 1.5f)
        return true;
    return false;
}