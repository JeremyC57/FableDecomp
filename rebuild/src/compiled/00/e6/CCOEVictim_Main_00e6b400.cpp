// CCOE_Victim::Main() -- Chapel of Evil sacrifice victim: let Me follow the hero, then make every COE_Victim (other
// than Me) and every live COE_AcolyteEvil a mutual ally of Me, the hero likewise, and idle until the thread terminates
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6B400, 842 bytes (848 with padding); entity-script vtable slot 1 (Main).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, CScriptThing::operator== 0x008AAE20, CScriptThing slots 0
//   (virtual dtor, inside the inlined ~vector) and 75 IsAlive, free 0x00BFEA14, CEntityScriptBase::IsActiveThreadTerminating
//   0x00F35B30, CBaseObject ctor 0x0099A380 / dtor 0x0099A430 (scripted-thing handle, vtable 0x0127094C, generated
//   inline dtor via CScriptGameResourceObjectBase 0x0126008C); CGameScriptInterface slots 7 NewScriptFrame,
//   70 GetHero, 75 GetAllThingsWithScriptName, 235 EntitySetAsAllowedToFollowHero, 599 EntitySetThingAsAllyOfThing.
//   Literals "COE_Victim" 0x012E8404, "COE_AcolyteEvil" 0x012E8428.
// Shape: the self test is the inline operator!= (= !operator==); the plain !(Me == x) form swaps eax/edx.
// Layout: Ego_r.pdb CCOE_Victim (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                           // slot 0 (out of line 0x004AA840)
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024();
    virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032();
    virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048();
    virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056();
    virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064();
    virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072();
    virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                      // slot 75 (+0x12c)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    bool operator==(const CScriptThing& other) const;  // 0x008AAE20
    bool operator!=(const CScriptThing& other) const { return !(*this == other); }
};
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                     // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()                                          // inline here (out-of-line COMDAT copy 0x008AC970)
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return *(_M_start + i); }
};
}
using std::vector;
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void SetPredicate(void* predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CCOE_Victim : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CV_ChapelOfEvilScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CCOE_Victim::Main()
{
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    vector<CScriptThing> victims;
    Game->GetAllThingsWithScriptName("COE_Victim", victims);
    vector<CScriptThing> acolytes;
    long acolyteCount = Game->GetAllThingsWithScriptName("COE_AcolyteEvil", acolytes);
    for (unsigned int i = 0; i < victims.size(); ++i) {
        Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
        Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
        if (Me != victims[i]) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetThingAsAllyOfThing(Me, victims[i]);
            Game->EntitySetThingAsAllyOfThing(victims[i], Me);
        }
        for (long j = 0; j < acolyteCount; ++j) {
            if (acolytes[j].IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetThingAsAllyOfThing(Me, acolytes[j]);
                Game->EntitySetThingAsAllyOfThing(acolytes[j], Me);
            }
        }
    }
    CScriptGameResourceObjectScriptedThingBase scripted;
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}