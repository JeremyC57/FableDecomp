// CV_TravellingHeroesScript::DoConversation(long line) -- play one line of the Thunder conversation from the script def: the speaker says the dialogue to the hero, the named actor (Thunder/Whisper -> rival, Boy, Girl) plays its animation after the def's delay unless the Thunder scene has started, then wait out the conversation and remove it
// Leaf: ?DoConversation@CV_TravellingHeroesScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE2A80, 0x325 bytes, `ret 4`.
// Callees: CCharString(const CCharString&) 0x0099EC30 / operator!=(const char*) 0x0099E960 / ~ 0x0099EAE0, Compare 0x00411570
//   (defined here under auto_inline(off): with the body in the TU VC7.1 knows it writes no memory and keeps the actor's PData
//   in esi across the four compares, as retail; declared extern it reloads the string after every call), CBaseClass()
//   0x0099A2D0, ~CScriptThing 0x004AA840, IsActiveThreadTerminating 0x00CB7940; Game slots 7, 70 GetHero, 72
//   GetThingWithScriptName, 364/365/366 conversation add, 367 RemoveConversation, 368 IsConversationActive, 376 Pause; the
//   actor handle's slot 18 (+0x48) animation call (descriptive name).
// Data: script def 0x0143E90C TH_ThunderConv_Speaker / _Dialogue / _Actor / _Animation / _AnimDelay vectors at +0x92c..+0x95c.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, seh_Rival +0x48, seh_Girl +0x58, seh_Boy +0x68, ThunderKidsTalking +0x78,
//   ThunderSceneStarted +0x79.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
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
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    __forceinline bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator!=(const char* text) const;                // 0x0099E960
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
extern CCharString g_EmptyCharString;                       // 0x0143E8EC
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
inline CCharString CScriptThing::GetDefName() const { if (!PImp.Data) return g_EmptyCharString; return PImp.Data->GetDefName(); }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline bool CScriptThing::IsNull() const { if (!PImp.Data) return true; return PImp.Data->IsNull(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptGameResourceObjectScriptedThingBase { public:        // 0x10 bytes, vtable 0x0127094C
    virtual void h00(); virtual void h01(); virtual void h02(); virtual void h03(); virtual void h04(); virtual void h05();
    virtual void h06(); virtual void h07(); virtual void h08(); virtual void h09(); virtual void h10(); virtual void h11();
    virtual void h12(); virtual void h13(); virtual void h14(); virtual void h15(); virtual void h16(); virtual void h17();
    virtual void PlayAnimation(const CCharString& animation, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // slot 18 (+0x48), descriptive name
    unsigned char _body[0xc];
};
class CScriptDef { public:                                         // retail layout (STLport 12-byte vectors; only _M_start used)
    unsigned char _pad0[0x92c];
    CCharString* TH_ThunderConv_Speaker;   unsigned char _pad930[8];   // +0x92c
    CCharString* TH_ThunderConv_Dialogue;  unsigned char _pad93c[8];   // +0x938
    CCharString* TH_ThunderConv_Actor;     unsigned char _pad948[8];   // +0x944
    CCharString* TH_ThunderConv_Animation; unsigned char _pad954[8];   // +0x950
    float*       TH_ThunderConv_AnimDelay;                             // +0x95c
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_TravellingHeroesScript : public CQuestScriptBase {
public:
    CScriptGameResourceObjectScriptedThingBase seh_Rival;   // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_Girl;    // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Boy;     // +0x68
    bool ThunderKidsTalking;                            // +0x78
    bool ThunderSceneStarted;                           // +0x79
    void DoConversation(long line);
};
} // namespace NScript
using NScript::CV_TravellingHeroesScript;

void NScript::CV_TravellingHeroesScript::DoConversation(long line)
{
    CCharString speaker = pGlobalScriptDef->TH_ThunderConv_Speaker[line];
    CCharString dialogue = pGlobalScriptDef->TH_ThunderConv_Dialogue[line];
    CCharString actor = pGlobalScriptDef->TH_ThunderConv_Actor[line];
    CCharString animation = pGlobalScriptDef->TH_ThunderConv_Animation[line];
    float delay = pGlobalScriptDef->TH_ThunderConv_AnimDelay[line];
    CScriptThing speakerThing = Game->GetThingWithScriptName(speaker);
    CScriptThing unused;
    long conversation = Game->AddNewConversation(speakerThing, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, dialogue, false, speakerThing, Game->GetHero());
    if (actor != "NULL") {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase* actorHandle = 0;
        if (actor == "Thunder" || actor == "Whisper") {
            if (IsActiveThreadTerminating())
                return;
            actorHandle = &seh_Rival;
        } else if (actor == "Boy") {
            if (IsActiveThreadTerminating())
                return;
            actorHandle = &seh_Boy;
        } else if (actor == "Girl") {
            if (IsActiveThreadTerminating())
                return;
            actorHandle = &seh_Girl;
        }
        if (actorHandle) {
            if (IsActiveThreadTerminating())
                return;
            Game->Pause(delay);
            if (!ThunderSceneStarted) {
                if (IsActiveThreadTerminating())
                    return;
                actorHandle->PlayAnimation(animation, false, true, false, false, false, false, true);
            }
        }
    }
    while (Game->IsConversationActive(conversation) && !ThunderSceneStarted && !ThunderKidsTalking) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveConversation(conversation, false);
}