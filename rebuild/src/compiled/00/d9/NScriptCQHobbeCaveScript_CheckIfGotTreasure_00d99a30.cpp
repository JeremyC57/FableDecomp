// NScript::CQ_HobbeCaveScript::CheckIfGotTreasure() -- the Hobbe Cave quest's "CheckIfGotTreasure" spawned thread: each time the main chamber region is loaded, look up the two treasure chests HC_Treasure1 / HC_Treasure2 and, while the region stays loaded, count every chest the hero uses (GotTreasure) and have the sacrificial bandit comment on it (TEXT_QST_036_BANDIT_GOT_TREASURE_ONE / _TWO); runs until the thread is terminated
// Leaf: ?CheckIfGotTreasure@CQ_HobbeCaveScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D99A30, 911 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb ?CheckIfGotTreasure@CQ_HobbeCaveScript@NScript@@QAEXXZ (0x00DB72C0); the HobbeCave spawner stores
//   0x00D99A30 at +0x34 of a CSpawnedFunc<CQ_HobbeCaveScript> (vtable 0x012D567C) it names "CheckIfGotTreasure"
//   (0x012D6488) at 0x00D9806F; `ret` with no arguments = QAEXXZ. Manifest label "Script_Bandit_Sacrifice" is wrong.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   CQ_HobbeCaveScript::MakeBanditComment 0x00D905C0, ~CScriptThing 0x004AA840 (the two named chests), CBaseClass dtor
//   0x0099A2E0 and operator delete 0x00BFE9BC (the inline ~CScriptThing of the bandit temporary); Game slots 7
//   NewScriptFrame, 12 IsRegionLoaded, 72 GetThingWithScriptName (ghidra_out/cgsi_slots_aligned.tsv); the chests'
//   CScriptThing slot 20 MsgIsUsedBy (inline wrapper, out of line 0x004AAB10).
// Data: "HobbeCaveMainChamber" 0x012D55EC, "HC_Treasure1" 0x012D65BC, "HC_Treasure2" 0x012D65AC, "SCRIPT_NAME_HERO"
//   0x0125D1C8, "BanditSacrifice" 0x012D5788, "TEXT_QST_036_BANDIT_GOT_TREASURE_ONE" 0x012D6584, "..._TWO" 0x012D655C.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, GotTreasure +0x58 (Ego_r +0x6C), BanditComment +0x54.
// Shape: ~CScriptThing is __forceinline (the GetThingWithScriptName temporary expands it); the two named chests are
//   destroyed out of line at every exit and at the end of their block, written under `#pragma inline_depth(0)`.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                          // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline ~CCountedPointer()
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
class CBaseClass { public:                              // polymorphic root of the script handles
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline virtual ~CScriptThing() {}            // 0x004AA840 where it stays out of line
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011();
    virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019();
    virtual bool MsgIsUsedBy(const CCharString& user) const   // slot 20 (+0x50) 0x004AAB10
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsUsedBy(user);
    }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterfaceBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30)
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
};
class CScriptBase { public:                             // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
class CTimer { public:                                  // 4 bytes: an index into the global script timers
    long TimerIndex;
};
namespace NScript {
class CQ_HobbeCaveScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*                     PMasterData;              // +0x44
    bool DoneIntro;                                     // +0x48
    bool SavedBeforeCaves;                              // +0x49
    bool BratInCutscene;                                // +0x4a
    bool MissionSucceeded;                              // +0x4b
    bool MissionFailed;                                 // +0x4c
    bool AlarmTriggered;                                // +0x4d
    bool KilledSideRoomHobbes;                          // +0x4e
    bool ReleaseBrat;                                   // +0x4f
    bool BratReturned;                                  // +0x50
    CTimer BanditComment;                               // +0x54
    long GotTreasure;                                   // +0x58
    long BanditHitCount;                                // +0x5c
    bool MakeBanditComment(const CCharString& line, const CScriptThing& bandit);   // 0x00D905C0
    void CheckIfGotTreasure();
};
}

void NScript::CQ_HobbeCaveScript::CheckIfGotTreasure()
{
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsRegionLoaded("HobbeCaveMainChamber")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing treasure1 = Game->GetThingWithScriptName("HC_Treasure1");
            CScriptThing treasure2 = Game->GetThingWithScriptName("HC_Treasure2");
            while (Game->IsRegionLoaded("HobbeCaveMainChamber")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                if (treasure1.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    ++GotTreasure;
                    MakeBanditComment("TEXT_QST_036_BANDIT_GOT_TREASURE_ONE", Game->GetThingWithScriptName("BanditSacrifice"));
                }
                if (treasure2.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    ++GotTreasure;
                    MakeBanditComment("TEXT_QST_036_BANDIT_GOT_TREASURE_TWO", Game->GetThingWithScriptName("BanditSacrifice"));
                }
            }
            if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                return;
        }
#pragma inline_depth()
        Game->NewScriptFrame();
    }
}