// CSecretBook::Main() -- a book already read in an earlier visit is swapped for its open-book object (facing the book's
//   heading plus a quarter turn) and removed; otherwise it sparkles and waits for the hero to use it: without the diary
//   key the "diary locked" notice is shown, with it the book is marked read and handed to the warden-game script.
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDE940, 748 bytes (+ int3 padding), plain `ret`. Entity-script slot 1 (Main).
// Callees: CCharString(const char*, long) 0x0099EBF0 / operator=(const char*) 0x0099EFE0 / ~CCharString 0x0099EAE0;
//   ~CScriptThing 0x004AA840 (out-of-line copy) / ~CBaseClass 0x0099A2E0 / CScriptThing::operator= 0x008AB980; operator delete
//   0x00BFE9BC; CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30. CGameScriptInterface slots (ghidra_out/
//   cgsi_slots_aligned.tsv) 7 NewScriptFrame, 40 MsgIsGameInfoClickedPast, 98 CreateObject, 100 CreateEffect,
//   108 RemoveThing, 115 DisplayGameInfo, 474 EntitySetFacingAngle; CScriptThing slots 6 GetPos, 10 GetAngleXY,
//   20 MsgIsUsedBy. Literals "OBJECT_BOOK_OPEN_01/04/02" 0x012DCCE0 / 0x012DCCB8 / 0x012DCCCC, "" 0x0122D70E,
//   "SPARKLES_SMALL" 0x012DCC2C, "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_055_DIARY_LOCKED" 0x012DCC9C; 0.25f 0x0123078C,
//   0.1f 0x0122EE58.
// Layout: Ego_r.pdb CSecretBook (32 bytes, = retail): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_PrisonWardenGameScript, retail = PDB - 0x14: SecretDiaryOpened +0x48, HeroHasSecretDiaryKey +0x49, BookOpened
//   +0x4c, SThingBookOpened +0x50), PMasterData +0x18 (CQ_SunnyvaleMasterData: BooksPreviouslyOpened[3] +0xef), BookType
//   +0x1c. CScriptThing / CCountedPointer models as the landed CBanditAttacker::Main 0x00D1F610.
namespace NScript { class CPredicateBase; }
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
static inline C3DVector RaisedBy(const C3DVector& pos, float height)
{
    return C3DVector(pos.X, pos.Y, pos.Z + height);
}
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const char* text);          // 0x0099EFE0
};
void operator delete(void* p);                          // 0x00BFE9BC
class CCPPointerInfo { public:
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;                            // slot 6 (+0x18)
    virtual void s007() const; virtual void s008() const; virtual void s009() const;
    virtual float GetAngleXY() const;                                   // slot 10 (+0x28)
    virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const;
    virtual void s017() const; virtual void s018() const; virtual void s019() const;
    virtual bool MsgIsUsedBy(const CCharString& user) const;            // slot 20 (+0x50)
    void*           PImp_Data;                          // +0x04
    CCPPointerInfo* PImp_Info;                          // +0x08
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
    ~CScriptThing()                                     // inline here; out-of-line copy 0x004AA840
    {
        if (PImp_Info) {
            --PImp_Info->RefCount;
            if (PImp_Info->RefCount == 0) {
                PImp_Info->DeleteFunc(PImp_Info->Data);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_PrisonWardenGameScript {
    unsigned char _pad00[0x48];
    bool          SecretDiaryOpened;                    // +0x48
    bool          HeroHasSecretDiaryKey;                // +0x49
    unsigned char _pad4a[2];
    long          BookOpened;                           // +0x4c
    CScriptThing  SThingBookOpened;                     // +0x50
};
struct CQ_SunnyvaleMasterData {
    unsigned char _pad00[0xef];
    bool          BooksPreviouslyOpened[3];             // +0xef
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};
struct CSecretBook : CEntityScriptBase {
    CQ_PrisonWardenGameScript* ParentClass;             // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;             // +0x18
    long                       BookType;                // +0x1c
    void Main();
};

void CSecretBook::Main()
{
    if (PMasterData->BooksPreviouslyOpened[BookType]) {
        if (IsActiveThreadTerminating())
            return;
        CCharString objectName("OBJECT_BOOK_OPEN_01");
        switch (BookType) {
        case 0: objectName = "OBJECT_BOOK_OPEN_01"; break;
        case 1: objectName = "OBJECT_BOOK_OPEN_04"; break;
        case 2: objectName = "OBJECT_BOOK_OPEN_02"; break;
        }
        CScriptThing book = Game->CreateObject(objectName, Me.GetPos(), "");
        Game->EntitySetFacingAngle(book, Me.GetAngleXY() + 0.25f, true);
        Game->RemoveThing(Me, false, true);
        return;
    }
    Game->CreateEffect("SPARKLES_SMALL", RaisedBy(Me.GetPos(), 0.1f), "", 0.0f, false, false);
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->HeroHasSecretDiaryKey) {
                if (IsActiveThreadTerminating())
                    return;
                PMasterData->BooksPreviouslyOpened[BookType] = true;
                ParentClass->SecretDiaryOpened = true;
                ParentClass->BookOpened = BookType;
                ParentClass->SThingBookOpened = Me;
                for (;;) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_055_DIARY_LOCKED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}