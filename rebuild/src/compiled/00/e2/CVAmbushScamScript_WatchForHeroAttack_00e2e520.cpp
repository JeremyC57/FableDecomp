// CV_AmbushScamScript::WatchForHeroAttack(CScriptThing target) -- each time the hero hits the target: while it survives count the hit and flag an attack on the trickster (before the ambush; after it, a warning or, past two hits, the real attack) or on the buddy; a first hit that kills it flags the killing blow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2E520, length 726 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, Compare 0x00411570 (the inlined CCharString == const char*) and the empty string global 0x0143E8EC,
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line at the in-loop returns); the
//   thing implementation's slots 1 GetDataString (+0x04), 21 MsgIsHitBy (+0x54), 43
//   MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xac) behind inlined null-safe wrappers; GameCalls slots 7 NewScriptFrame,
//   264 GetHealth. Strings "SCRIPT_NAME_HERO", "AS_Trickster", "AS_Buddy"; 0.0001f 0x0129BA3C.
// Layout: Ego_r.pdb CV_AmbushScamScript minus 0x14 (retail CScriptBase): DoAmbush +0x4f, HeroAttackOnBuddy +0x64,
//   HeroAttackOnTricksterBeforeAmbush +0x65, HeroAttackOnTricksterAfterAmbush +0x66, HeroAttackOnTricksterKillingBlow
//   +0x67, HeroAttachOnTricksterWarning +0x69, HitCount +0x74. CScriptThing as the landed
//   CV_TalentlessBardScript::WatchForAttack 0x00ED9790; the string == as CV_ChapelOfEvilScript::WatchForInsideChapel.
// Shape: generated script code; `for (;;)` with the terminate return first; ~CScriptThing is __forceinline with
//   `#pragma inline_depth(0)` around the in-loop returns except the first (two inline exit copies, as retail). Compare has
//   its landed body in this TU under auto_inline(off) (as CTCCameraPointScriptedSpline::HasMessage 0x0075D2B0), so the
//   thing's data pointer stays in ebp across it. The killed-after-ambush arm calls IsActiveThreadTerminating and returns
//   either way (VC merges the two out-of-line exits).
#include <string.h>
#pragma intrinsic(strcmp)
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body: VC7.1 then knows it writes no memory)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                                // CBasicString<char>, 13 bytes
    char*         PData;                                 // +0x00
    unsigned long StringLength;                          // +0x04
    unsigned long DataLength : 31;                       // +0x08
    bool          UseFastExtend : 1;                     // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
struct CCharStringData {                                 // 17 bytes
    CBasicStringChar Data;                               // +0x00
    long             NoRefs;                             // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
extern const CCharString g_EmptyCharString;              // 0x0143E8EC
struct CBaseClass {
    virtual ~CBaseClass();                                  // 0x0099A2E0
    CBaseClass();                                           // 0x0099A2D0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
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
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data (CScriptThing vtable layout)
    virtual void i000();
    virtual const CCharString& GetDataString() const;                         // slot 1 (+0x04)
    virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;             // slot 18 (+0x48)
    virtual void i019(); virtual void i020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;              // slot 21 (+0x54)
    virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029();
    virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037();
    virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const; // slot 43 (+0xac)
    virtual void i044();
    virtual void i045();
    virtual void i046();
    virtual void i047();
    virtual void i048();
    virtual void i049();
    virtual void i050();
    virtual void i051();
    virtual void i052();
    virtual void i053();
    virtual void i054();
    virtual void i055();
    virtual void i056();
    virtual void i057();
    virtual void i058();
    virtual void i059();
    virtual void i060();
    virtual void i061();
    virtual void i062();
    virtual void i063();
    virtual void i064();
    virtual void i065();
    virtual void i066();
    virtual void i067();
    virtual void i068();
    virtual void i069();
    virtual void i070();
    virtual void i071();
    virtual void i072();
    virtual void i073();
    virtual void i074();
    virtual bool IsAlive() const;                                                      // slot 75 (+0x12c)
};
struct CScriptThing : CBaseClass {                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    __forceinline ~CScriptThing() {}                                      // out of line 0x004AA840
    const CCharString& GetDataString() const
    {
        if (!PImp.Data)
            return g_EmptyCharString;
        return PImp.Data->GetDataString();
    }
    bool IsAlive() const                                    // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
    bool MsgIsKilledBy(const CCharString& killer) const     // 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
    bool MsgIsHitBy(const CCharString& attacker) const      // 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(attacker);
    }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const   // 0x004AAD60
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
    }
};
struct GameCalls {                                       // CGameScriptInterface, retail vtable 0x01260F0C
 virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
 virtual void NewScriptFrame(void) const; //slot7 0x006e7410
 virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
 virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
 virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
 virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
 virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
 virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
 virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
 virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
 virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
 virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
 virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
 virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
 virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
 virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
 virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
 virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
 virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
 virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
 virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
 virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
 virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
 virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
 virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
 virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
 virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
 virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
 virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223();
 virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231();
 virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
 virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247();
 virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255();
 virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
 virtual float GetHealth(CScriptThing const &) const; //slot264 0x00890400
};
struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CV_AmbushScamScript : CScriptBase {
    GameCalls*    Game;                                     // +0x40
    void*         Owner;                                    // +0x44
    unsigned char _pad48[0x4f - 0x48];
    bool          DoAmbush;                                 // +0x4f
    unsigned char _pad50[0x64 - 0x50];
    bool          HeroAttackOnBuddy;                        // +0x64
    bool          HeroAttackOnTricksterBeforeAmbush;        // +0x65
    bool          HeroAttackOnTricksterAfterAmbush;         // +0x66
    bool          HeroAttackOnTricksterKillingBlow;         // +0x67
    bool          HeroTalkToTrickster;                      // +0x68
    bool          HeroAttachOnTricksterWarning;             // +0x69
    unsigned char _pad6a[0x74 - 0x6a];
    long          HitCount;                                 // +0x74
    void WatchForHeroAttack(CScriptThing target);
};

void CV_AmbushScamScript::WatchForHeroAttack(CScriptThing target)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (target.MsgIsHitBy("SCRIPT_NAME_HERO") || target.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->GetHealth(target) > 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                ++HitCount;
                if (target.GetDataString() == "AS_Trickster") {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    if (DoAmbush) {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        if (HitCount > 2) {
#pragma inline_depth(0)
                            if (IsActiveThreadTerminating())
                                return;
                            HeroAttackOnTricksterAfterAmbush = true;
                            IsActiveThreadTerminating();
                            return;
#pragma inline_depth()
                        }
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        HeroAttachOnTricksterWarning = true;
                    } else {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        HeroAttackOnTricksterBeforeAmbush = true;
                    }
                } else if (target.GetDataString() == "AS_Buddy") {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    HeroAttackOnBuddy = true;
                }
            } else if (HitCount == 0) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                HeroAttackOnTricksterKillingBlow = true;
            }
        }
        Game->NewScriptFrame();
    }
}