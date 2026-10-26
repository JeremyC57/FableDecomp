// CGameScriptInterface::CleanupActions() -- ends the script camera, unfreezes the player's controls, closes the player GUI, ends the camera again, ends the letter box and the screen fade-out, and unfreezes the controls once more
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7480, length 136 bytes.
// Callees: CAIGameCameraBase virtual slot 14 (+0x38, ScriptEnd) on CWorld+0x34 (PCurrentGameCamera) twice, the inlined UnfreezeControls body twice
// (CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetControlledCreature @ 0x00487DD0, +0x91 bit 0, the unfreeze @ 0x00449A50 with CurrentPlayer +0x1c),
// CPlayerGui::Close @ 0x004374B0 on g_pPlayerGui @ 0x013B8790, CGameScriptInterface virtual slot 378 (+0x5e8, EndLetterBox), CDisplayEngine::EndScreenFadeOut @ 0x00434C90.
// Shape: UnfreezeControls (0x006E73E0) is called qualified, so its body is inlined.
// Leaf: CleanupActions
#include <string.h>
struct CThing_Retail;
class CScriptThing;
// CCharString (Ego_r.pdb layout, as the landed CCharString::operator!= 0x0099E960): a 4-byte handle on a 17-byte block.
#pragma pack(push, 1)
struct CBasicStringChar {                   // CBasicString<char>, 13 bytes
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
    unsigned long DataLength : 31;          // +0x08
    bool          UseFastExtend : 1;        // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                    // 17 bytes
    CBasicStringChar Data;                  // +0x00
    long             NoRefs;                // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;           // +0x00
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool IsEmpty() const { return PStringData == 0 || PStringData->Data.StringLength == 0; }
};
struct CThing_Retail;
struct CBase {
    CBase();                                        // 0x0099A2D0
    virtual ~CBase();
};
class CScriptThing : public CBase { public:                       // 12 bytes: vptr (0x01238C8C) + the thing's intelligent pointer words
     virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009(); virtual void s010();
    virtual CThing_Retail* GetThing() const;   // slot 11 (+0x2c)
    virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019();
    virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027();
    virtual void s028(); virtual void s029(); virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035();
    virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040(); virtual void s041(); virtual void s042(); virtual void s043();
    virtual void s044(); virtual void s045(); virtual void s046(); virtual void s047(); virtual void s048(); virtual void s049(); virtual void s050(); virtual void s051();
    virtual void s052(); virtual void s053(); virtual void s054(); virtual void s055(); virtual void s056(); virtual void s057(); virtual void s058(); virtual void s059();
    virtual void s060(); virtual void s061(); virtual void s062(); virtual void s063(); virtual void s064(); virtual void s065(); virtual void s066(); virtual void s067();
    virtual void s068(); virtual void s069(); virtual void s070(); virtual void s071(); virtual void s072(); virtual void s073(); virtual void s074();
    virtual bool IsValid() const;   // slot 75 (+0x12c)
    void* m_pData;                                  // +0x04
    void* m_pItem;                                  // +0x08
    CScriptThing() : m_pData(0), m_pItem(0) { }
};
struct CTCBase_Retail;
struct CInterfaceMap_Retail {                       // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x004365B0 / 0x0040F020 (two copies of the same body)
};
struct CThing_Retail {
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031();
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055();
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063();
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071();
    virtual void t072(); virtual void t073(); virtual void t074(); virtual void t075(); virtual void t076();
    virtual bool IsPlayer();   // slot 77 (+0x134)
    unsigned char         _pad04[0xc];              // +0x04 .. +0x10
    unsigned char         m_type;                   // +0x10
    unsigned char         _pad11[0xf];
    unsigned long         m_presentBits[9];         // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail  m_interfaces;             // +0x44 the sorted {type, component} map
    unsigned char         _pad4c[0x20];
    unsigned char         m_flags;                  // +0x6c (bit 2 = game object, bit 3 = creature)
    unsigned char         _pad6d[0x24];
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned char         _pad92[0x2a];
    unsigned char         m_bc_0 : 1;               // +0xbc
    unsigned char         m_givesHeroStatChangesOnHit : 1;   // bit 1
    unsigned char         m_bc_2 : 6;
    unsigned char         _padbd[0x10c];
    unsigned char         m_1c9_0 : 3;              // +0x1c9
    unsigned char         m_marryable : 1;          //        bit 3
    unsigned char         m_marryableToHero : 1;    //        bit 4
    unsigned char         m_1c9_5 : 3;
    CScriptThing GetScriptThing();                  // 0x004C7E50 (by value through the hidden result pointer)
    CTCBase_Retail* AddTC(const CCharString& name, long a, long b);   // 0x004C9D60
    void RemoveTC(long interfaceType);              // 0x004C9840 (the label "CBaseObject::OnPropertyChanged" is propagated)
    void FinishCurrentAction();                     // 0x00662650 (CThingCreatureBase)
    void SheatheAllWeapons();                       // 0x006647F0 (CThingCreatureBase)
    void UnsheatheAllWeapons();                     // 0x00666830 (CThingCreatureBase)
    void UnsheatheMeleeWeapon();                    // 0x00666820 (CThingCreatureBase)
    void SetAsGivingHeroStatChangesOnBeingHit(bool giving);   // 0x008389C0 (CThingGameObject)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct CPlayer_Retail {
    CThing_Retail* GetHeroThing();                  // 0x00487DC0 (label propagated)
    CThing_Retail* GetControlledThing();            // 0x00487DD0 (label propagated)
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetPlayer();                    // 0x00449970 (labelled "Catch@0040d959", propagated)
};
struct CAIGameCameraBase_Retail {
    virtual void c000(); virtual void c001(); virtual void c002(); virtual void c003(); virtual void c004(); virtual void c005(); virtual void c006(); virtual void c007();
    virtual void c008(); virtual void c009(); virtual void c010(); virtual void c011(); virtual void c012(); virtual void c013();
    virtual void ScriptEnd();   // slot 14 (+0x38)
};
class CWorld { public:
    unsigned char             _pad00[0x34];
    CAIGameCameraBase_Retail* PCurrentGameCamera;   // +0x34 (engine/CWorld.h)
};
class CPlayer { public:
    CThing_Retail* GetControlledCreature();         // 0x00487DD0
};
class CPlayerManager { public:
    void UnfreezePlayerControls(long player);       // 0x00449A50
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CPlayerGui { public:
public:
    void Open();   // 0x004374B0
};
extern CPlayerGui* g_pPlayerGui;             // 0x013B8790
class CDisplayEngine { public:
public:
    void EndScreenFadeOut();   // 0x00434C90
};
class CGameScriptInterface { public:
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006(); virtual void g007();
    virtual void g008(); virtual void g009(); virtual void g010(); virtual void g011(); virtual void g012(); virtual void g013(); virtual void g014(); virtual void g015();
    virtual void g016(); virtual void g017(); virtual void g018(); virtual void g019(); virtual void g020(); virtual void g021(); virtual void g022(); virtual void g023();
    virtual void g024(); virtual void g025(); virtual void g026(); virtual void g027(); virtual void g028(); virtual void g029(); virtual void g030(); virtual void g031();
    virtual void g032(); virtual void g033(); virtual void g034(); virtual void g035(); virtual void g036(); virtual void g037(); virtual void g038(); virtual void g039();
    virtual void g040(); virtual void g041(); virtual void g042(); virtual void g043(); virtual void g044(); virtual void g045(); virtual void g046(); virtual void g047();
    virtual void g048(); virtual void g049(); virtual void g050(); virtual void g051(); virtual void g052(); virtual void g053(); virtual void g054(); virtual void g055();
    virtual void g056(); virtual void g057(); virtual void g058(); virtual void g059(); virtual void g060(); virtual void g061(); virtual void g062(); virtual void g063();
    virtual void g064(); virtual void g065(); virtual void g066(); virtual void g067(); virtual void g068(); virtual void g069(); virtual void g070(); virtual void g071();
    virtual void g072(); virtual void g073(); virtual void g074(); virtual void g075(); virtual void g076(); virtual void g077(); virtual void g078(); virtual void g079();
    virtual void g080(); virtual void g081(); virtual void g082(); virtual void g083(); virtual void g084(); virtual void g085(); virtual void g086(); virtual void g087();
    virtual void g088(); virtual void g089(); virtual void g090(); virtual void g091(); virtual void g092(); virtual void g093(); virtual void g094(); virtual void g095();
    virtual void g096(); virtual void g097(); virtual void g098(); virtual void g099(); virtual void g100(); virtual void g101(); virtual void g102(); virtual void g103();
    virtual void g104(); virtual void g105(); virtual void g106(); virtual void g107(); virtual void g108(); virtual void g109(); virtual void g110(); virtual void g111();
    virtual void g112(); virtual void g113(); virtual void g114(); virtual void g115(); virtual void g116(); virtual void g117(); virtual void g118(); virtual void g119();
    virtual void g120(); virtual void g121(); virtual void g122(); virtual void g123(); virtual void g124(); virtual void g125(); virtual void g126(); virtual void g127();
    virtual void g128(); virtual void g129(); virtual void g130(); virtual void g131(); virtual void g132(); virtual void g133(); virtual void g134(); virtual void g135();
    virtual void g136(); virtual void g137(); virtual void g138(); virtual void g139(); virtual void g140(); virtual void g141(); virtual void g142(); virtual void g143();
    virtual void g144(); virtual void g145(); virtual void g146(); virtual void g147(); virtual void g148(); virtual void g149(); virtual void g150(); virtual void g151();
    virtual void g152(); virtual void g153(); virtual void g154(); virtual void g155(); virtual void g156(); virtual void g157(); virtual void g158(); virtual void g159();
    virtual void g160(); virtual void g161(); virtual void g162(); virtual void g163(); virtual void g164(); virtual void g165(); virtual void g166(); virtual void g167();
    virtual void g168(); virtual void g169(); virtual void g170(); virtual void g171(); virtual void g172(); virtual void g173(); virtual void g174(); virtual void g175();
    virtual void g176(); virtual void g177(); virtual void g178(); virtual void g179(); virtual void g180(); virtual void g181(); virtual void g182(); virtual void g183();
    virtual void g184(); virtual void g185(); virtual void g186(); virtual void g187(); virtual void g188(); virtual void g189(); virtual void g190(); virtual void g191();
    virtual void g192(); virtual void g193(); virtual void g194(); virtual void g195(); virtual void g196(); virtual void g197(); virtual void g198(); virtual void g199();
    virtual void g200(); virtual void g201(); virtual void g202(); virtual void g203(); virtual void g204(); virtual void g205(); virtual void g206(); virtual void g207();
    virtual void g208(); virtual void g209(); virtual void g210(); virtual void g211(); virtual void g212(); virtual void g213(); virtual void g214(); virtual void g215();
    virtual void g216(); virtual void g217(); virtual void g218(); virtual void g219(); virtual void g220(); virtual void g221(); virtual void g222(); virtual void g223();
    virtual void g224(); virtual void g225(); virtual void g226(); virtual void g227(); virtual void g228(); virtual void g229(); virtual void g230(); virtual void g231();
    virtual void g232(); virtual void g233(); virtual void g234(); virtual void g235(); virtual void g236(); virtual void g237(); virtual void g238(); virtual void g239();
    virtual void g240(); virtual void g241(); virtual void g242(); virtual void g243(); virtual void g244(); virtual void g245(); virtual void g246(); virtual void g247();
    virtual void g248(); virtual void g249(); virtual void g250(); virtual void g251(); virtual void g252(); virtual void g253(); virtual void g254(); virtual void g255();
    virtual void g256(); virtual void g257(); virtual void g258(); virtual void g259(); virtual void g260(); virtual void g261(); virtual void g262(); virtual void g263();
    virtual void g264(); virtual void g265(); virtual void g266(); virtual void g267(); virtual void g268(); virtual void g269(); virtual void g270(); virtual void g271();
    virtual void g272(); virtual void g273(); virtual void g274(); virtual void g275(); virtual void g276(); virtual void g277(); virtual void g278(); virtual void g279();
    virtual void g280(); virtual void g281(); virtual void g282(); virtual void g283(); virtual void g284(); virtual void g285(); virtual void g286(); virtual void g287();
    virtual void g288(); virtual void g289(); virtual void g290(); virtual void g291(); virtual void g292(); virtual void g293(); virtual void g294(); virtual void g295();
    virtual void g296(); virtual void g297(); virtual void g298(); virtual void g299(); virtual void g300(); virtual void g301(); virtual void g302(); virtual void g303();
    virtual void g304(); virtual void g305(); virtual void g306(); virtual void g307(); virtual void g308(); virtual void g309(); virtual void g310(); virtual void g311();
    virtual void g312(); virtual void g313(); virtual void g314(); virtual void g315(); virtual void g316(); virtual void g317(); virtual void g318(); virtual void g319();
    virtual void g320(); virtual void g321(); virtual void g322(); virtual void g323(); virtual void g324(); virtual void g325(); virtual void g326(); virtual void g327();
    virtual void g328(); virtual void g329(); virtual void g330(); virtual void g331(); virtual void g332(); virtual void g333(); virtual void g334(); virtual void g335();
    virtual void g336(); virtual void g337(); virtual void g338(); virtual void g339(); virtual void g340(); virtual void g341(); virtual void g342(); virtual void g343();
    virtual void g344(); virtual void g345(); virtual void g346(); virtual void g347(); virtual void g348(); virtual void g349(); virtual void g350(); virtual void g351();
    virtual void g352(); virtual void g353(); virtual void g354(); virtual void g355(); virtual void g356(); virtual void g357(); virtual void g358(); virtual void g359();
    virtual void g360(); virtual void g361(); virtual void g362(); virtual void g363(); virtual void g364(); virtual void g365(); virtual void g366(); virtual void g367();
    virtual void g368(); virtual void g369(); virtual void g370(); virtual void g371(); virtual void g372(); virtual void g373(); virtual void g374(); virtual void g375();
    virtual void g376(); virtual void g377();
    virtual void EndLetterBox();   // slot 378 (+0x5e8)
    CWorld*                  World;                 // +0x04 (engine/CGameScriptInterface.h)
    void*                    Component;             // +0x08
    CDisplayEngine*   DisplayEngine;         // +0x0c
    void*                    DefinitionManager;     // +0x10
    CPlayerManager* PlayerManager;         // +0x14
    void*                    ThingSearchTools;      // +0x18
    long                     CurrentPlayer;         // +0x1c
    void UnfreezeControls()                         // 0x006E73E0 (inlined at the qualified call)
    {
        CThing_Retail* creature = PlayerManager->GetMainPlayer().GetControlledCreature();
        if (creature && !creature->m_dead)
            PlayerManager->UnfreezePlayerControls(CurrentPlayer);
    }
    void CleanupActions();
};

void CGameScriptInterface::CleanupActions()
{
    World->PCurrentGameCamera->ScriptEnd();
    CGameScriptInterface::UnfreezeControls();
    g_pPlayerGui->Open();
    World->PCurrentGameCamera->ScriptEnd();
    EndLetterBox();
    DisplayEngine->EndScreenFadeOut();
    CGameScriptInterface::UnfreezeControls();
}