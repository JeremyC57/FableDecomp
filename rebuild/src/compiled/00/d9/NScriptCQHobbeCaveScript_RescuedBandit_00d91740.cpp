// CQ_HobbeCaveScript::RescuedBandit() -- each frame: wait for the HobbeCaveSideRoom level, gather the "HobbeRoaming" hobbes and, once they are all dead, record KilledSideRoomHobbes and stop
// Levers: ~vector<CScriptThing> __forceinline; for(;;){ if (Term) return; ...} outer loop (keeps three separate inline dtor copies, no cross-jumping); dead block `if (!Term) { if (!Term) KilledSideRoomHobbes = true; }` then one depth-0 `return;`
// Leaf: RescuedBandit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D91740, 481 bytes (496 with padding), plain `ret`; spawned as "RescuedBandit"
//   (0x012D573C) at 0x00D8E4A0.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("HobbeCaveSideRoom" 0x012D562C,
//   "HobbeRoaming" 0x012D5808), CScriptBase::IsActiveThreadTerminating 0x00CB7940, AreAllThingsInVectorDead 0x00CBED00
//   (__fastcall), the vector<CScriptThing> destructor 0x008AC970 (out of line; manifest "~vector<CConversation>" is a
//   folded twin), free 0x00BFEA14 (inline destructor copies); CScriptThing slot 0 (virtual destructor);
//   CGameScriptInterface (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded (+0x2c), 75 GetAllThingsWithScriptName (+0x12c).
// Layout (retail = Ego_r.pdb - 0x14): KilledSideRoomHobbes +0x4e; vector model as the landed CM_TauntingBandit::Main
//   0x00E4B430.
// Shape: the outer loop is `while (!terminating)` (rotated: entry test + bottom test after the frame yield).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
struct CCPPointerInfo;
struct CScriptThing {                              // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                       // slot 0; complete destructor 0x004AA840
    virtual void v001(); virtual void v002();
    virtual void v004(); virtual void v005();
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
};
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}
bool __fastcall AreAllThingsInVectorDead(const std::vector<CScriptThing>& things);   // 0x00CBED00
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual bool IsLevelLoaded(const CCharString& level) const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const;   // slot 75 (+0x12c)
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_HobbeCaveScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool                  DoneIntro;                    // +0x48
    bool                  SavedBeforeCaves;             // +0x49
    bool                  BratInCutscene;               // +0x4a
    bool                  MissionSucceeded;             // +0x4b
    bool                  MissionFailed;                // +0x4c
    bool                  AlarmTriggered;               // +0x4d
    bool                  KilledSideRoomHobbes;         // +0x4e
    void RescuedBandit();
};
} // namespace NScript
using NScript::CQ_HobbeCaveScript;

void NScript::CQ_HobbeCaveScript::RescuedBandit()
{
    std::vector<CScriptThing> hobbes;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("HobbeCaveSideRoom")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GetAllThingsWithScriptName("HobbeRoaming", hobbes);
        if (AreAllThingsInVectorDead(hobbes)) {
            if (!IsActiveThreadTerminating()) {
                if (!IsActiveThreadTerminating())
                    KilledSideRoomHobbes = true;
            }
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}