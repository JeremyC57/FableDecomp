// CV_BeardyBaldyScript::WatchForBarberLadyDeath() -- find the barber nearest the hero, wait until she is alive, then until she is killed (by anyone), and record BarberLadyKilled
// Levers: ~CScriptThing __forceinline; IsAlive in if-form; depth 0 around the post-first-loop terminating return; killed block `if (!Term) { if (!Term) BarberLadyKilled = true; }` then one depth-0 `return;` (the merged out-of-line exit)
// Leaf: WatchForBarberLadyDeath
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E504C0, 400 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, ~CScriptThing @ 0x004AA840 / inline (counted release, operator delete @ 0x00BFE9BC, ~CBaseClass
//          @ 0x0099A2E0), CScriptThing::IsAlive / MsgIsKilledBy inline (game-side thing slots 75 +0x12c / 18 +0x48);
//          CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero (+0x118), 77 GetNearestWithScriptName (+0x134).
// Layout: Ego_r.pdb CV_BeardyBaldyScript - 0x14: BarberLadyKilled +0x69. CScriptThing model as the landed
//          CQ_HangingTreeGoodScript (0x00D74F80); same TU as the landed WatchForBeardyBaldyDeath (0x00E503D0).
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
    __forceinline ~CScriptThing() {}                     // out of line 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const;
    virtual bool MsgIsKilledBy(const CCharString& killer) const
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }   // slot 18 (+0x48)
    virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const; virtual void v25() const; virtual void v26() const;
    virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const;
    virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const;
    virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const; virtual void v49() const; virtual void v50() const;
    virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const;
    virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const;
    virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const; virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const                         // slot 75 (+0x12c)
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
};
class CGameScriptInterface { public:   // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual const CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const;   // slot 77 (+0x134)
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
class CV_BeardyBaldyScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x20];                          // +0x48 quest flags, phase, required hair strings
    bool BeardyBaldyKilled;                              // +0x68
    bool BarberLadyKilled;                               // +0x69
    void WatchForBarberLadyDeath();
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;

void NScript::CV_BeardyBaldyScript::WatchForBarberLadyDeath()
{
    CScriptThing barber = GSI->GetNearestWithScriptName(GSI->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
    while (!barber.IsAlive()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
#pragma inline_depth()
    while (!IsActiveThreadTerminating()) {
        if (barber.MsgIsKilledBy("")) {
            if (!IsActiveThreadTerminating()) {
                if (!IsActiveThreadTerminating())
                    BarberLadyKilled = true;
            }
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        GSI->NewScriptFrame();
    }
}