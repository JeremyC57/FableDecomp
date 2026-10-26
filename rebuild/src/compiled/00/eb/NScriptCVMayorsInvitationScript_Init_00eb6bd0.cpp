// CV_MayorsInvitationScript::Init() -- clears the quest flags and counters and looks up the Oakvale hero spawn point ("OakvaleHSP") into destinationHSP
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB6BD0, 222 bytes (padded to 224), plain `ret`. Slot 3 of vtable 0x012F0528.
// Callees: CGameScriptInterface slot 72 GetThingWithScriptName(const CCharString&) (+0x120, 0x008A7D60, CScriptThing by
//   value through the hidden pointer), CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   ~CBaseClass 0x0099A2E0 (the temporary's base), operator delete 0x00BFE9BC (the counted block). String "OakvaleHSP"
//   0x012F053C.
// Shape: field clears in source order with one zero register; the literal is an implicit conversion temporary; the
//   CScriptThing assignment is the inline counted-pointer operator= (source Info/Data read into locals first, release
//   of the old block re-reading the member, add-ref of the new); the returned temporary's inline destructor re-stores
//   the CScriptThing vtable, releases and zeroes its block and calls the base destructor.
// Layout (retail = Ego_r.pdb - 0x14): flags +0x48..+0x50, SecretsLearned +0x54, WhichQuest +0x58,
//   MonitorThunderHeroDuelRunning +0x5c, destinationHSP +0x60 (PImp Data +0x64, Info +0x68).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                     // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                            // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
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
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
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
};
class CBaseClass { public:                                // polymorphic root of the script handles
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:                 // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;            // +0x04 Data, +0x08 Info
    ~CScriptThing() { }
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name);   // slot 72 (+0x120)
};
namespace NScript {
class CV_MayorsInvitationScript {
public:
    unsigned char _base[0x40];                     // CScriptBase
    CGameScriptInterface* Game;                    // +0x40
    void* Data;                                    // +0x44
    bool MissionSucceeded;                         // +0x48
    bool MissionFailed;                            // +0x49
    bool MissionOver;                              // +0x4a
    bool HeroAbortedQuest;                         // +0x4b
    bool HeroLearnedSecret;                        // +0x4c
    bool HeroReturnedFromParanoidQuest;            // +0x4d
    bool HeroHasDefeatedThunder;                   // +0x4e
    bool HeroHasLostToThunder;                     // +0x4f
    bool QuestCardGiven;                           // +0x50
    long SecretsLearned;                           // +0x54
    long WhichQuest;                               // +0x58
    long MonitorThunderHeroDuelRunning;            // +0x5c
    CScriptThing destinationHSP;                   // +0x60
    void Init();
};
} // namespace NScript
using NScript::CV_MayorsInvitationScript;
void NScript::CV_MayorsInvitationScript::Init()
{
    MissionSucceeded = false;
    MissionFailed = false;
    MissionOver = false;
    HeroAbortedQuest = false;
    SecretsLearned = 0;
    MonitorThunderHeroDuelRunning = 0;
    HeroLearnedSecret = false;
    WhichQuest = 0;
    HeroHasDefeatedThunder = false;
    HeroHasLostToThunder = false;
    HeroReturnedFromParanoidQuest = false;
    QuestCardGiven = false;
    destinationHSP = Game->GetThingWithScriptName("OakvaleHSP");
}