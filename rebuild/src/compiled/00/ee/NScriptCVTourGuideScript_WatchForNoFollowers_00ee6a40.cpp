// Levers: vector dtor __forceinline with the finish pointer cached in a local (`last`, edi); for(;;) with the terminating return first, both stop conditions break to one trailing terminating test; in-body `#pragma inline_depth(0)` around the all-dead terminate return keeps that vector dtor out of line (0x008AC970).
// CV_TourGuideScript::WatchForNoFollowers() -- collect the tour followers, then watch until the hero leaves the guild complex or (tour unfinished) every follower is dead, flagging AllFollowersDead in the latter case
// Leaf: ?WatchForNoFollowers@CV_TourGuideScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE6A40, 441 bytes (448 with padding), plain `ret`.
// Callees: CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   AreAllThingsInVectorDead 0x00CBED00 (`__fastcall`), vector<CScriptThing>::~vector 0x008AC970 (out of line once),
//   free 0x00BFEA14, CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame
//   (slot 7), MsgIsRegionUnloaded (slot 21), GetAllThingsWithScriptName (slot 75).
// Strings: "TourGuideFollower" 0x012F50F0, "HeroGuildComplexInside" 0x01238FCC.
// Layout (retail = Ego_r.pdb - 0x14): AllFollowersDead +0x121, TourFinished +0x123; interface pointer +0x40.
//   The follower vector is STLport's 12-byte {_M_start, _M_finish, _M_end_of_storage}, storage released with free().
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <stdlib.h>
struct CCharString {
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
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

struct CScriptThingImpl;
struct CScriptThing : CBaseClass {                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing() {}                                      // out of line 0x004AA840
};
struct CScriptThingVector {                                 // std::vector<CScriptThing> (STLport, 12 bytes)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~CScriptThingVector()                     // out of line 0x008AC970
    {
        CScriptThing* last = _M_finish;
        for (CScriptThing* p = _M_start; p != last; ++p)
            p->~CScriptThing();
        if (_M_start)
            free(_M_start);
    }
};
bool __fastcall AreAllThingsInVectorDead(const CScriptThingVector& things);   // 0x00CBED00
struct CGameScriptInterface {                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                           // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual bool MsgIsRegionUnloaded(const CCharString& region) const;             // slot 21 (+0x54)
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const; // slot 75 (+0x12c)
};

struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_TourGuideScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x120 - 0x48];                     // WaypointInfo +0x48 ..
    bool TourGuideKilled;                                   // +0x120
    bool AllFollowersDead;                                  // +0x121
    bool QuestFinished;                                     // +0x122
    bool TourFinished;                                      // +0x123
    void WatchForNoFollowers();
};
} // namespace NScript
using NScript::CV_TourGuideScript;

void NScript::CV_TourGuideScript::WatchForNoFollowers()
{
    CScriptThingVector followers;
    CCharString unused;
    Game->GetAllThingsWithScriptName("TourGuideFollower", followers);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->MsgIsRegionUnloaded("HeroGuildComplexInside")) {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        if (AreAllThingsInVectorDead(followers) && !TourFinished) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            AllFollowersDead = true;
            break;
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
}