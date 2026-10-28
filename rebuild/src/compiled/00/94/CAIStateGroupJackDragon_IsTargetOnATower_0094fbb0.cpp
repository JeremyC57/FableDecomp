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
class C3DVector { public: float X, Y, Z; };

class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CAIStateGroupBase { public:
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
    CIntelligentPointer<CThing> PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CIntelligentPointer<CThing> PLandedMarker;      // +0x1c CIntelligentPointer<CThing>
    bool IsTargetOnATower();
};

bool CAIStateGroup_JackDragon::IsTargetOnATower()
{
    CThing* pMarker = PLandedMarker.GetPItem();
    CThing* pEnemy = PEnemy.GetPItem();
    if (pEnemy->GetPos().Z > pMarker->GetPos().Z + 1.5f)
        return true;
    return false;
}