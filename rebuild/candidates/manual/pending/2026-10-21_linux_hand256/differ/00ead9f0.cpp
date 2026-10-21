// Grade: DIFFER(356v356) -- one register: retail reloads the counted-pointer Info into eax for operator delete, the lane picks ecx (release shapes, operator delete/char* delete/local copy/--x==0/QFE all give ecx)
// CV_MurderTwistScript::GemsFoundProcess() -- wait until the hero carries a diamond, count the find, and drop the
// buried gem from the magic-rune rumble list
// Leaf: GemsFoundProcess
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAD9F0, 356 bytes (padded to 368).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, vector<CScriptThing>::erase(iterator) @ 0x00CD3152 (catalog "std::vector::pop_back"), the
//          inline counted release (operator delete @ 0x00BFE9BC, ~CBaseClass @ 0x0099A2E0); CScriptThing::IsEqualTo
//          inline (game-side slot 78, +0x138, taking the other's implementation pointer); CGameScriptInterface slots 7
//          NewScriptFrame (+0x1c), 72 GetThingWithScriptName (+0x120), 183 GetNumberOfItemsOfTypeInInventory (+0x2dc).
// Layout (retail = Ego_r.pdb CV_MurderTwistScript - 0x14): MurderTwistItemsFound +0x48, MagicRuneRumbleList +0x54
//          (STLport vector<CScriptThing>, 12-byte elements).
struct CCharString {                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> struct CCountedPointer {
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

struct CBaseClass {                                      // polymorphic root of the script handles
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                   // out of line 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const; virtual void v75() const; virtual void v76() const; virtual void v77() const;
    virtual bool IsEqualTo(const CScriptThing* other) const;   // slot 78 (+0x138), the implementation's compare
    bool IsEqualTo(const CScriptThing& other) const { return PImp.Data->IsEqualTo(other.PImp.Data); }
};
struct CGameScriptInterface {   // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152();
    virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168();
    virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176();
    virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182();
    virtual long GetNumberOfItemsOfTypeInInventory(const CCharString& type) const;   // slot 183 (+0x2dc)
};
struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};
struct CScriptThingVector {                              // STLport vector<CScriptThing> (12 bytes in retail)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThing* begin() { return _M_start; }
    CScriptThing* end() { return _M_finish; }
    CScriptThing* erase(CScriptThing* position);         // 0x00CD3152
};
struct CV_MurderTwistScript : CQuestScriptBase {
    long               MurderTwistItemsFound;            // +0x48
    long               MurderTwistState;                 // +0x4c
    long               MurderTwistEvent;                 // +0x50
    CScriptThingVector MagicRuneRumbleList;              // +0x54
    void GemsFoundProcess();
};

void CV_MurderTwistScript::GemsFoundProcess()
{
    while (GSI->GetNumberOfItemsOfTypeInInventory("OBJECT_GEMSTONE_DIAMOND") <= 0) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ++MurderTwistItemsFound;
    for (CScriptThing* it = MagicRuneRumbleList.begin(); it != MagicRuneRumbleList.end(); ++it) {
        if (GSI->GetThingWithScriptName("MurderTwistBuriedGem").IsEqualTo(*it)) {
            MagicRuneRumbleList.erase(it);
            return;
        }
    }
}
