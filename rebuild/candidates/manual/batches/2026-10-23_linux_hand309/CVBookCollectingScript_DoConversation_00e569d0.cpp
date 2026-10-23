// CV_BookCollectingScript::DoConversation(long book, long line) -- play one line of a book's reaction conversation: the speaker says it to the hero, the boy or girl speaking plays the line's animation (and its loop), then wait for the line to finish
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E569D0, length 823 bytes, `ret 8`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const CCharString&) 0x0099EC30 / ~CCharString
//   0x0099EAE0 / operator!=(const char*) 0x0099E960; Compare 0x00411570 (the inline operator==(const char*)); ~CScriptThing
//   0x004AA840; the scripted-thing handle's virtuals 18 PlayAnimation (+0x48), 20 PlayLoopingAnimation (+0x50), 26
//   IsPerformingScriptTask (+0x68) (vtable 0x0127094C); GameCalls slots 7 NewScriptFrame, 70 GetHero, 72
//   GetThingWithScriptName (by value), 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation, 367
//   RemoveConversation, 368 IsConversationActive. Global bool 0x01375748 (an animation default). Strings "NULL", "boy0", "girl0".
// Layout: Ego_r.pdb CV_BookCollectingScript minus 0x14 (retail CScriptBase): ReadingBook +0x48, BookReactions +0x4c
//   (vector<CConversation>, 12 bytes retail), seh_Boy +0x58, seh_Girl +0x68. CConversation (PDB 108, retail 0x5c with
//   12-byte vectors): Lines +0x28, Speaker +0x2c, Dialogue +0x38, Animation +0x44, AnimLoop +0x50.
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
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    bool operator!=(const char* text) const;             // 0x0099E960
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CScriptGameResourceObjectScriptedThingBase;
struct CScriptThing {                                    // 12 bytes, vtable 0x01238C8C {vptr, PImp_Data, PImp_Info}
    virtual void v00();
    void* PImp_Data;
    void* PImp_Info;
    ~CScriptThing();                                     // 0x004AA840
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
 virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
 virtual CScriptThing & GetHero(void); //slot70 0x00891ca0
 virtual void v071();
 virtual CScriptThing GetThingWithScriptName(CCharString const &) const; //slot72 0x008a7d60
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
 virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
 virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192();
 virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200();
 virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208();
 virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216();
 virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224();
 virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232();
 virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240();
 virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248();
 virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256();
 virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264();
 virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272();
 virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280();
 virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288();
 virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296();
 virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
 virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312();
 virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
 virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328();
 virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336();
 virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344();
 virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352();
 virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360();
 virtual void v361(); virtual void v362(); virtual void v363();
 virtual long AddNewConversation(CScriptThing const &, bool, bool) const; //slot364 0x008906c0
 virtual void AddPersonToConversation(long, CScriptThing const &) const; //slot365 0x00890710
 virtual void AddLineToConversation(long, CCharString const &, bool, CScriptThing const &, CScriptThing const &) const; //slot366 0x00890750
 virtual void RemoveConversation(long, bool) const; //slot367 0x008907c0
 virtual bool IsConversationActive(long) const; //slot368 0x008907d0
};
struct CScriptGameResourceObjectScriptedThingBase {      // vtable 0x0127094C, 16 bytes
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04();
    virtual void s05(); virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09();
    virtual void s10(); virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14();
    virtual void s15(); virtual void s16(); virtual void s17();
    virtual void PlayAnimation(const CCharString& name, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // slot 18 (+0x48)
    virtual void s19();
    virtual void PlayLoopingAnimation(const CCharString& name, long count, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // slot 20 (+0x50)
    virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25();
    virtual bool IsPerformingScriptTask() const;         // slot 26 (+0x68) 0x007E7450
    void* m_pIntelligentPointer;
    void* PImp_Data;
    void* PImp_Info;
};
extern bool g_bScriptAnimationFlag;                      // 0x01375748 (a global bool the script headers pass as an animation default)
template <class T> struct vector {                       // STLport vector: {_M_start, _M_finish, _M_end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int i) { return *(begin() + i); }
};
struct CConversation {                                   // retail 0x5c bytes
    unsigned char _pad00[0x28];
    long Lines;                                          // +0x28
    vector<CCharString> Speaker;                         // +0x2c
    vector<CCharString> Dialogue;                        // +0x38
    vector<CCharString> Animation;                       // +0x44
    vector<CCharString> AnimLoop;                        // +0x50
};
struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};
struct CV_BookCollectingScript : CScriptBase {
    GameCalls* Game;                                     // +0x40
    void* Owner;                                         // +0x44
    bool ReadingBook;                                    // +0x48
    vector<CConversation> BookReactions;                 // +0x4c
    CScriptGameResourceObjectScriptedThingBase seh_Boy;  // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Girl; // +0x68
    void DoConversation(long book, long line);
};

void CV_BookCollectingScript::DoConversation(long book, long line)
{
    CCharString speaker = BookReactions[book].Speaker[line];
    CCharString dialogue = BookReactions[book].Dialogue[line];
    CCharString animation = BookReactions[book].Animation[line];
    CCharString animLoop = BookReactions[book].AnimLoop[line];
    CScriptThing speakerThing = Game->GetThingWithScriptName(speaker);
    long conversation = -1;
    if (dialogue != "NULL") {
        if (IsActiveThreadTerminating())
            return;
        conversation = Game->AddNewConversation(speakerThing, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, dialogue, false, speakerThing, Game->GetHero());
    }
    CScriptGameResourceObjectScriptedThingBase* handle = 0;
    if (speaker == "boy0") {
        if (IsActiveThreadTerminating())
            return;
        handle = &seh_Boy;
    } else if (speaker == "girl0") {
        if (IsActiveThreadTerminating())
            return;
        handle = &seh_Girl;
    }
    if (handle) {
        if (IsActiveThreadTerminating())
            return;
        if (animLoop == "NULL") {
            if (IsActiveThreadTerminating())
                return;
            handle->PlayAnimation(animation, false, true, false, true, g_bScriptAnimationFlag, false, false);
        } else {
            if (IsActiveThreadTerminating())
                return;
            handle->PlayAnimation(animation, false, true, false, false, g_bScriptAnimationFlag, false, false);
            handle->PlayLoopingAnimation(animLoop, -1, false, false, true, false, g_bScriptAnimationFlag, false, false);
        }
    }
    while ((Game->IsConversationActive(conversation) || handle->IsPerformingScriptTask()) && !ReadingBook) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveConversation(conversation, false);
}
