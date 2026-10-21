// CFireHeartHolder::Main() -- until the fire heart has been taken: each frame, when the hero uses the holder, play the (unnamed) movie sequence, pause the world, mark the heart taken, take the fire heart object from the hero, remove the holder's minimap marker and unpause; then idle until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF2D30, length 349 bytes (`ret`).
// Callees: the entity script's IsDone @ 0x00F35B30, CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, CScriptThing slot 20
//   MsgIsUsedBy(const CCharString&) (0x004AAB10), the movie handle's root ctor/dtor @ 0x0099A380 / 0x0099A430 and its PImp
//   counted-pointer dtor @ 0x006E7AB0 (vtables 0x01260EF4 / 0x0126008C); CGameScriptInterface slots 7 NewScriptFrame,
//   125 TakeObjectFromHero, 352 MiniMapRemoveMarker, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities.
// Layout (the CEntityScriptBase prefix of the landed entity scripts): vptr, Game +4, CScriptThing Me +8, the owning quest
//   script +0x14 (fire heart taken flag +0x50).
// Shape: the pause/unpause pair goes through one named copy of Game (edi); the movie handle is a block local whose
//   implicit destructor releases PImp out of line and then runs the inline base destructor (vptr store + root dtor).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CScriptThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual bool MsgIsUsedBy(const CCharString& scriptName) const;   // slot 20 (+0x50)
    void* PImp_Data;
    void* PImp_Info;
};
struct CBaseObjectRoot {                                // the intelligent-pointer target root (8 bytes)
    virtual void v00();
    void* m_pReferences;                                // +0x04
    CBaseObjectRoot();                                  // 0x0099A380
    ~CBaseObjectRoot();                                 // 0x0099A430
};
struct CScriptGameResourceObjectBase : CBaseObjectRoot {   // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CMovieImpPointer {                               // CCountedPointer<...> {Data, Info}
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0 (out of line here)
};
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4, no declared dtor
    CMovieImpPointer PImp;                              // +0x08
};

#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CGameScriptInterface {
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1C)
    VPAD64(qa) VPAD32(qb) VPAD16(qc) VPAD4(qd) VPAD1(qe)                             // slots 8..124
    virtual void TakeObjectFromHero(const CCharString& defName) const;               // slot 125 (+0x1F4)
    VPAD128(ra) VPAD64(rb) VPAD32(rc) VPAD2(rd)                                      // slots 126..351
    virtual void MiniMapRemoveMarker(const CScriptThing& thing) const;               // slot 352 (+0x580)
    VPAD16(sa) VPAD1(sb)                                                             // slots 353..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5C8)
    VPAD8(ta)                                                                        // slots 371..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5EC)
};
struct CQuestScript_Retail {
    unsigned char _pad00[0x50];
    bool          FireHeartTaken;                       // +0x50
};

class CFireHeartHolder {
public:
    void*                   __vftable;                  // +0x00
    CGameScriptInterface*   Game;                       // +0x04
    CScriptThing            Me;                         // +0x08
    CQuestScript_Retail*    Quest;                      // +0x14
    bool IsDone();                                      // 0x00F35B30
    void Main();
};

void CFireHeartHolder::Main()
{
    while (!Quest->FireHeartTaken) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
        bool used;
        {
            CCharString hero("SCRIPT_NAME_HERO");
            used = Me.MsgIsUsedBy(hero);
        }
        if (used) {
            if (IsDone())
                return;
            CScriptGameResourceObjectMovieBase movie;
            {
                CCharString name("");
                Game->StartMovieSequence(name, movie);
            }
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Quest->FireHeartTaken = true;
            {
                CCharString fireHeart("OBJECT_LPDD_FIREHEART_01");
                Game->TakeObjectFromHero(fireHeart);
            }
            Game->MiniMapRemoveMarker(Me);
            game->PauseAllNonScriptedEntities(false);
        }
    }
    while (!IsDone())
        Game->NewScriptFrame();
}