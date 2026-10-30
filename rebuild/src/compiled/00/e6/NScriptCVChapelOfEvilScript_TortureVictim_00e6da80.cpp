// CV_ChapelOfEvilScript::TortureVictim(CScriptThing& victim, CScriptGameResourceObjectScriptedThingBase& victimObject, CScriptGameResourceObjectScriptedThingBase& heroObject) -- record the victim's health, sex, night time, Sunday and guard/bandit/trader flags, pick the torture sound, then play CS_CHAPEL_EVIL_TORTURE with HERO/VICTIM cast and the $SOUND argument while the master data's in-progress flag is set; clear it after one frame unless terminating
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6DA80, 0x389 bytes, `ret 0Ch`.
// Callees: CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0 / operator= 0x0099EFB0, __ftol2 0x00BFEA70, malloc 0x00BFEA0E,
//   free 0x00BFEA14, operator delete 0x00BFE9BC, CBaseObject::operator= 0x0099A3B0, the cast map (operator[] 0x00CD3D2E, erase
//   0x00CD344A) and the argument map (operator[] 0x009AC700, erase 0x009AA830), both constructed and destroyed inline,
//   RunCutsceneMacro 0x00CBFB7D (__fastcall), GetVictimIsGuard 0x00E6E050 / IsBandit 0x00E6E120 / IsTrader 0x00E6E1B0 (each
//   takes the victim's GetDefName(), CScriptThing virtual slot 2, by value), GetTortureSoundEffect 0x00E6DE10,
//   IsActiveThreadTerminating 0x00CB7940; Game slots 7, 264 GetHealth, 513 EntityGetSex, 647 GetTimeOfDay, 651 GetDayOfWeek.
// Shape: the landed CQ_OrchardFarmRaidScript::DoMultiplierCutscene 0x00DD1AF0 cast-map model, with __forceinline map
//   destructors and the handle operator= (CBaseObject::operator= + counted-pointer copy through locals). CCharString needs its
//   copy constructor declared so the GetDefName() result is built straight into the by-value argument slot.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, PMasterData +0x44 (flag +0x10), VictimHealth +0x78, VictimGender +0x7c,
//   VictimNightTime +0x80, VictimSunday +0x81, VictimIsGuard +0x82, VictimIsBandit +0x83, VictimIsTrader +0x84, TortureSound +0x88.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    long RefCount;                                      // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
struct CScriptThingImp;
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();                            // 0x004AA840
    virtual void t001();
    virtual CCharString GetDefName() const;             // slot 2 (+0x08)
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x0127094C
    CCountedPointer<CScriptThingImp> PImp;              // +0x08
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
};
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (12 bytes)
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned char       _M_key_compare;
    __forceinline CCutsceneCastMap() : _M_header(0)
    {
        _M_header = (_Rb_tree_node_base*)malloc(0x24);
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    __forceinline ~CCutsceneCastMap()                   // out of line 0x00CDBFB0
    {
        if (_M_node_count) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
        if (_M_header)
            free(_M_header);
    }
    void _M_erase(_Rb_tree_node_base* root);            // 0x00CD344A
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneArgMap {                                // map<CCharString, CCharString> (12 bytes)
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned char       _M_key_compare;
    __forceinline CCutsceneArgMap() : _M_header(0)
    {
        _M_header = (_Rb_tree_node_base*)malloc(0x18);
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    __forceinline ~CCutsceneArgMap()
    {
        if (_M_node_count) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
        if (_M_header)
            free(_M_header);
    }
    void _M_erase(_Rb_tree_node_base* root);            // 0x009AA830
    CCharString& operator[](const CCharString& key);    // 0x009AC700
};
struct CCutsceneFlagMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
enum ESex { SEX_MALE, SEX_FEMALE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CChapelOfEvilMasterData {
    unsigned char _pad[0x10];
    bool TortureInProgress;                             // +0x10
};
namespace NScript {
class CV_ChapelOfEvilScript : public CScriptBase {
public:
    CGameScriptInterface*    Game;                      // +0x40
    CChapelOfEvilMasterData* PMasterData;               // +0x44
    unsigned char            _pad48[0x78 - 0x48];
    long                     VictimHealth;              // +0x78
    ESex                     VictimGender;              // +0x7c
    bool                     VictimNightTime;           // +0x80
    bool                     VictimSunday;              // +0x81
    bool                     VictimIsGuard;             // +0x82
    bool                     VictimIsBandit;            // +0x83
    bool                     VictimIsTrader;            // +0x84
    CCharString              TortureSound;              // +0x88
    void TortureVictim(CScriptThing& victim, CScriptGameResourceObjectScriptedThingBase& victimObject, CScriptGameResourceObjectScriptedThingBase& heroObject);
    bool GetVictimIsGuard(CCharString defName);         // 0x00E6E050
    bool GetVictimIsBandit(CCharString defName);        // 0x00E6E120
    bool GetVictimIsTrader(CCharString defName);        // 0x00E6E1B0
    void GetTortureSoundEffect(ESex sex);               // 0x00E6DE10
};
} // namespace NScript
using NScript::CV_ChapelOfEvilScript;

void NScript::CV_ChapelOfEvilScript::TortureVictim(CScriptThing& victim, CScriptGameResourceObjectScriptedThingBase& victimObject, CScriptGameResourceObjectScriptedThingBase& heroObject)
{
    VictimHealth = (long)Game->GetHealth(victim);
    VictimGender = Game->EntityGetSex(victim);
    VictimNightTime = Game->GetTimeOfDay() > 2159 || Game->GetTimeOfDay() < 600;
    VictimSunday = Game->GetDayOfWeek() == 6;
    VictimIsGuard = GetVictimIsGuard(victim.GetDefName());
    VictimIsBandit = GetVictimIsBandit(victim.GetDefName());
    VictimIsTrader = GetVictimIsTrader(victim.GetDefName());
    GetTortureSoundEffect(VictimGender);
    CCutsceneCastMap cast;
    cast["HERO"] = heroObject;
    cast["VICTIM"] = victimObject;
    CCutsceneArgMap args;
    args["$SOUND"] = TortureSound;
    PMasterData->TortureInProgress = true;
    RunCutsceneMacro("CS_CHAPEL_EVIL_TORTURE", cast, 0, &args, false, true);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    PMasterData->TortureInProgress = false;
}