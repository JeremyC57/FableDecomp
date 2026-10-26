// CBS_Teacher::LookForBook() -- the book-collecting teacher checks the hero's pack: find the first book the hero carries that was not the last one requested and is not yet donated (marking each newly found one as owned), stopping early at a wanted/accepted/comment index once one is found; then say "none found", a comment line, a request line (and ask for the book), or "not found"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E57530, length 1200 bytes, plain `ret`.
// Callees: IsActiveThreadTerminating 0x00F35B30, AskForBook 0x00E55CE0, GFIntToCharString 0x0099F830 and
//   operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`, hidden result), CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0 / operator const char* 0x0099E4C0, ~CScriptThing 0x004AA840. The member handle seh_me is called
//   through its vtable (0x0127094C): slots 12 GetScriptThing, 13 Speak, 26 IsPerformingScriptTask.
//   CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 184 IsObjectInThingsPossession, 264 GetHealth.
// Literals: TEXT_QST_B16_BOOK_NONE_FOUND 0x012E658C, _COMMENT_ 0x012E6570, _REQUEST_ 0x012E6554, _REFUSED 0x012E6538,
//   _NOT_FOUND 0x012E651C.
// Layout: Ego_r.pdb CBS_Teacher (68 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
//   +0x18, BadBooksForHat +0x1c, GoodBooksForHat +0x20, BooksWanted +0x24, BooksAccepted +0x28, BooksComment +0x2c,
//   MoralityReward +0x30, seh_me +0x34. CV_BookCollectingScript is retail = PDB - 0x18 here (0x14 for the base, 4 for
//   the 12-byte retail STLport vectors): LastBookRequested +0x8c, BooksInGame +0x90, BookDef +0x94, BookDonated +0xa0,
//   BookOwned +0xac (the bool vectors are read as plain byte arrays).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    operator const char*() const;                      // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
class CScriptThing { public:
    void* vptr;
    void* PImp[2];
    ~CScriptThing();                                   // 0x004AA840
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_0 };
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
class CScriptGameResourceObjectScriptedThingBase { public:    // vtable 0x0127094C, 16 bytes
    VPAD8(p0) VPAD4(p1)                                // slots 0..11
    virtual CScriptThing GetScriptThing() const;       // slot 12 (+0x30)
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34)
    VPAD8(p2) VPAD4(p3)                                // slots 14..25
    virtual bool IsPerformingScriptTask() const;       // slot 26 (+0x68)
    void* _body[3];
};
class CGameScriptInterface { public:
    VPAD4(p0) VPAD2(p1) VPAD1(p2)                      // slots 0..6
    virtual void NewScriptFrame() const;               // slot 7 (+0x1c)
    VPAD32(q0) VPAD16(q1) VPAD8(q2) VPAD4(q3) VPAD2(q4)   // slots 8..69
    virtual const CScriptThing& GetHero() const;       // slot 70 (+0x118)
    VPAD64(r0) VPAD32(r1) VPAD16(r2) VPAD1(r3)            // slots 71..183
    virtual bool IsObjectInThingsPossession(const CCharString& def, const CScriptThing& thing) const;   // slot 184 (+0x2e0)
    VPAD64(s0) VPAD8(s1) VPAD4(s2) VPAD2(s3) VPAD1(s4) // slots 185..263
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
};
struct CV_BookCollectingScript {
    unsigned char _pad[0x8c];
    long         LastBookRequested;                    // +0x8c
    long         BooksInGame;                          // +0x90
    CCharString* BookDef;                              // +0x94 vector<CCharString> _M_start
    void*        BookDefFinish[2];
    bool*        BookDonated;                          // +0xa0
    void*        BookDonatedFinish[2];
    bool*        BookOwned;                            // +0xac
};
struct CQ_SunnyvaleMasterData;
class CBS_Teacher {
public:
    void*                    __vftable;                // +0x00
    CGameScriptInterface*    Game;                     // +0x04
    CScriptThing             Me;                       // +0x08
    CV_BookCollectingScript* ParentClass;              // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;              // +0x18
    long BadBooksForHat;                               // +0x1c
    long GoodBooksForHat;                              // +0x20
    long BooksWanted;                                  // +0x24
    long BooksAccepted;                                // +0x28
    long BooksComment;                                 // +0x2c
    long MoralityReward;                               // +0x30
    CScriptGameResourceObjectScriptedThingBase seh_me; // +0x34
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    bool AskForBook(long book, CCharString refusal);   // 0x00E55CE0
    void LookForBook();
};

void CBS_Teacher::LookForBook()
{
    bool newlyOwned = false;
    long found = -1;
    for (long i = 0; i < ParentClass->BooksInGame; ++i) {
        if ((i == BooksWanted || i == BooksAccepted || i == BooksComment) && found >= 0) {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        CCharString* defs = ParentClass->BookDef;
        if (Game->IsObjectInThingsPossession(defs[i], Game->GetHero())) {
            if (IsActiveThreadTerminating())
                return;
            if (i != ParentClass->LastBookRequested && !ParentClass->BookDonated[i]) {
                if (IsActiveThreadTerminating())
                    return;
                if (!newlyOwned) {
                    if (IsActiveThreadTerminating())
                        return;
                    found = i;
                }
                if (!ParentClass->BookOwned[i]) {
                    if (IsActiveThreadTerminating())
                        return;
                    ParentClass->BookOwned[i] = true;
                    newlyOwned = true;
                }
            } else if (found == -1) {
                if (IsActiveThreadTerminating())
                    return;
                found = -2;
            }
        }
    }
    if (found == -2 || found >= BooksComment) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), "TEXT_QST_B16_BOOK_NOT_FOUND", (ETextGroupSelectionMethod)2, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    } else if (found == -1) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), "TEXT_QST_B16_BOOK_NONE_FOUND", (ETextGroupSelectionMethod)2, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    } else if (found >= BooksAccepted) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), "TEXT_QST_B16_BOOK_COMMENT_" + GFIntToCharString(found - BooksAccepted), (ETextGroupSelectionMethod)0, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        CCharString request = "TEXT_QST_B16_BOOK_REQUEST_" + GFIntToCharString(found);
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), request, (ETextGroupSelectionMethod)0, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        AskForBook(found, "TEXT_QST_B16_BOOK_REFUSED");
    }
}