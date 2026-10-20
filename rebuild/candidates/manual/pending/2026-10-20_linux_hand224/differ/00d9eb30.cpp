// CMC_MinionGuard::Main() -- minion-camp guard entity script: each frame, when Me is hit and the camp says Me is a hero attacker, retarget Me onto whoever hit it unless that is already its target (the hero while it has none)
// Grade: DIFFER(ours v retail) -- register choice only: the SetTarget argument address is built in ecx and pushed before `mov ecx,esi` (retail: `lea eax ; mov ecx,esi ; push eax`)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CScriptThing {                                   // 12 bytes, vtable 0x01238C8C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& hitter) const;   // slot 21 (+0x54) 0x004AAB30
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual CScriptThing MsgWhoHitMe() const;                   // slot 26 (+0x68) 0x004ABEC0
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual bool IsNull() const;                                // slot 77 (+0x134) 0x004AB150
    virtual bool IsEqualTo(const CScriptThing* other) const;    // slot 78 (+0x138) 0x004AA8A0
    CScriptThing* PImp_Data;                                    // +0x04 CCountedPointer<CScriptThing>::Data
    void*         PImp_Info;                                    // +0x08
    ~CScriptThing();                                            // 0x004AA840
    bool operator==(const CScriptThing& other) const { return PImp_Data->IsEqualTo(other.PImp_Data); }
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
};
class CQ_MinionCampScript {
public:
    bool IsAssignedAsHeroAttacker(const CScriptThing& thing);   // 0x00D9EC50
};
class CMC_MinionGuard {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    CQ_MinionCampScript*  ParentClass;        // +0x14
    void*                 PMasterData;        // +0x18
    CScriptThing          Target;             // +0x1c
    bool IsDone();                            // 0x00F35B30
    void SetTarget(const CScriptThing& target);   // 0x00D9ECF0
    void Main();
};

void CMC_MinionGuard::Main()
{
    while (!IsDone()) {
        if (Me.MsgIsHitBy("")) {
            if (IsDone())
                return;
            if (ParentClass->IsAssignedAsHeroAttacker(Me)) {
                if (IsDone())
                    return;
                CScriptThing attacker = Me.MsgWhoHitMe();
                bool same;
                if (Target.IsNull()) {
                    if (IsDone())
                        return;
                    same = attacker == Game->GetHero();
                } else {
                    if (IsDone())
                        return;
                    same = attacker == Target;
                }
                if (!same) {
                    if (IsDone())
                        return;
                    SetTarget(attacker);
                }
            }
        }
        Game->NewScriptFrame();
    }
}
