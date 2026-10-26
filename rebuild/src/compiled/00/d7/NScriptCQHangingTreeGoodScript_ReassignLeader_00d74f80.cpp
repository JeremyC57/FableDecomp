// CQ_HangingTreeGoodScript::ReassignLeader() -- refill the guard list from the "HTG_Guard" things and make the first guard the party leader if he lives, otherwise the hero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D74F80, length 213 bytes; called from DoMission (0x00D74430) and 0x00D76030.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0, CScriptThing::operator= 0x008AB980, CScriptThing's virtual destructor (slot 0, with 0) and IsAlive (slot 75),
// and through the game script interface (+0x40): GetHero (slot 70), GetAllThingsWithScriptName (slot 75).
// Layout (Ego_r.pdb - 0x14, retail 12-byte vectors): PartySuccessfullySetUp +0x49, MissionSucceeded +0x4a,
// MissionFailed +0x4b, ReassignLeaderNow +0x4d, Leader +0x68, CurrentEnemyVector +0xa0, CurrentGuardVector +0xac.
// Shape: three early-out tests; the guard vector is emptied with the inline pop_back loop (the `lea esi` reference to
// the vector is kept for the lookup and front()); the guard name is a named CCharString local destroyed right after the
// lookup; each arm re-tests the thread before assigning the leader.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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

class CBaseClass { public:                                      // polymorphic root of the script handles
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};

class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                   // out of line 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);  // 0x008AB980
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
template <class T> struct vector {                       // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    T& front() { return *_M_start; }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
class CGameScriptInterface { public:                           // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) 0x00891CA0
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString&, vector<CScriptThing>&);   // slot 75 (+0x12c) 0x008A8570
};
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

namespace NScript {
class CQ_HangingTreeGoodScript : public CQuestScriptBase {
public:
    bool LeaderSpokenTo;                                 // +0x48
    bool PartySuccessfullySetUp;                         // +0x49
    bool MissionSucceeded;                               // +0x4a
    bool MissionFailed;                                  // +0x4b
    bool SwitchingRegions;                               // +0x4c
    bool ReassignLeaderNow;                              // +0x4d
    unsigned char _pad4e[0x68 - 0x4e];
    CScriptThing Leader;                                 // +0x68
    CScriptThing Prisoner;                               // +0x74
    CScriptThing PartySpawnPoint;                        // +0x80
    CScriptThing ActiveWayPoint;                         // +0x8c
    CCharString  CurrentLevel;                           // +0x98
    CCharString  ReasonForFailing;                       // +0x9c
    vector<CScriptThing> CurrentEnemyVector;             // +0xa0
    vector<CScriptThing> CurrentGuardVector;             // +0xac
    void ReassignLeader();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

void NScript::CQ_HangingTreeGoodScript::ReassignLeader()
{
    if (MissionSucceeded || MissionFailed || IsActiveThreadTerminating())
        return;
    vector<CScriptThing>& guards = CurrentGuardVector;
    while (!guards.empty())
        guards.pop_back();
    {
        CCharString name("HTG_Guard");
        GSI->GetAllThingsWithScriptName(name, guards);
    }
    if (guards.front().IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Leader = guards.front();
        ReassignLeaderNow = false;
        PartySuccessfullySetUp = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        Leader = GSI->GetHero();
        PartySuccessfullySetUp = true;
    }
}