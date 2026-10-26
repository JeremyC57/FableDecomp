// CQ_TraderEscortScript::WatchForPickpocketing(CScriptThing trader) -- watch thread: frame by frame, when the hero picks a pocket and the victim is this trader, count it (more than two marks the master data's pickpocketed-all-traders state as 2); when the trader is killed, reset that state to 0
// Leaf: WatchForPickpocketing
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E04F10, 531 bytes (544 with int3 padding), `ret 0Ch` (the 12-byte CScriptThing
//   is passed by value and destroyed here).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, ~CScriptThing 0x004AA840 (out of line on the
//   count path), CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, operator delete 0x00BFE9BC; the thing implementation's slots 18 MsgIsKilledBy (+0x48)
//   and 78 IsEqualTo (+0x138) through the inline CScriptThing wrappers; through the quest's interface (+0x40):
//   NewScriptFrame (slot 7), MsgOnHeroPickedPocket (slot 53, +0xd4).
// Layout (retail = Ego_r PDB -0x14): TraderPickpocketedCount +0x68; master data (+0x44) CQ_SunnyvaleMasterData::
//   DarkwoodPickpocketedAllTraders +0x120 (PDB offset).
// Shape: the landed CheckVictimHit 0x00DE81B0 by-value CScriptThing model plus a default-constructed local; for(;;)
//   with the terminating return first; ~CScriptThing __forceinline, with the count block between
//   `#pragma inline_depth(0)` / `#pragma inline_depth()` so its three returns call 0x004AA840 out of line; the inline
//   MsgIsKilledBy tests the null arm first (`if (!PImp.Data) return false;`) to get retail's `jne ; xor bl,bl ; jmp`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

class CBaseClass { public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};

class CScriptThing;
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointerScriptThing() : Data(0), Info(0) {}
    ~CCountedPointerScriptThing()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    CScriptThing() {}
    virtual __forceinline ~CScriptThing() {}            // slot 0; out of line: 0x004AA840
    virtual void v001();    virtual void v002();    virtual void v003();    virtual void v004();    virtual void v005();    virtual void v006();    virtual void v007();    virtual void v008();
    virtual void v009();    virtual void v010();    virtual void v011();    virtual void v012();    virtual void v013();    virtual void v014();    virtual void v015();    virtual void v016();
    virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const    // slot 18 (+0x48)
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
    virtual void v019();    virtual void v020();    virtual void v021();    virtual void v022();    virtual void v023();    virtual void v024();    virtual void v025();    virtual void v026();
    virtual void v027();    virtual void v028();    virtual void v029();    virtual void v030();    virtual void v031();    virtual void v032();    virtual void v033();    virtual void v034();
    virtual void v035();    virtual void v036();    virtual void v037();    virtual void v038();    virtual void v039();    virtual void v040();    virtual void v041();    virtual void v042();
    virtual void v043();    virtual void v044();    virtual void v045();    virtual void v046();    virtual void v047();    virtual void v048();    virtual void v049();    virtual void v050();
    virtual void v051();    virtual void v052();    virtual void v053();    virtual void v054();    virtual void v055();    virtual void v056();    virtual void v057();    virtual void v058();
    virtual void v059();    virtual void v060();    virtual void v061();    virtual void v062();    virtual void v063();    virtual void v064();    virtual void v065();    virtual void v066();
    virtual void v067();    virtual void v068();    virtual void v069();    virtual void v070();    virtual void v071();    virtual void v072();    virtual void v073();    virtual void v074();
    virtual void v075();    virtual void v076();    virtual void v077();
    virtual bool IsEqualTo(const CScriptThing* other) const;       // slot 78 (+0x138)
    bool operator==(const CScriptThing& other) const { return PImp.Data->IsEqualTo(other.PImp.Data); }
};

class CGameScriptInterface { public:                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual bool MsgOnHeroPickedPocket(CScriptThing& victim) const;   // slot 53 (+0xd4)
};

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0x120];
    long          DarkwoodPickpocketedAllTraders;       // +0x120
};

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQ_TraderEscortScript : public CScriptBase {
public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    unsigned char           _pad48[0x20];               // CommentTimer +0x48 .. ShownEarthTroll +0x71 start below
    long                    TraderPickpocketedCount;    // +0x68
    void WatchForPickpocketing(CScriptThing trader);
};

void CQ_TraderEscortScript::WatchForPickpocketing(CScriptThing trader)
{
    CScriptThing victim;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (GSI->MsgOnHeroPickedPocket(victim)) {
            if (IsActiveThreadTerminating())
                return;
            if (victim == trader) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
                ++TraderPickpocketedCount;
                if (IsActiveThreadTerminating())
                    return;
                if (TraderPickpocketedCount > 2) {
                    if (IsActiveThreadTerminating())
                        return;
                    PMasterData->DarkwoodPickpocketedAllTraders = 2;
                }
                return;
#pragma inline_depth()
            }
        }
        if (trader.MsgIsKilledBy("")) {
            if (IsActiveThreadTerminating())
                return;
            PMasterData->DarkwoodPickpocketedAllTraders = 0;
            return;
        }
        GSI->NewScriptFrame();
    }
}
}