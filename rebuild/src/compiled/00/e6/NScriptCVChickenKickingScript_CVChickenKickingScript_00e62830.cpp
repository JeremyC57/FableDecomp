// CV_ChickenKickingScript::CV_ChickenKickingScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012E7318, the empty cut-scene argument map and the Chicken/ChickenMaster entity handles
// Leaf: ??0CV_ChickenKickingScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E62830, length 126 bytes (padded to 128), `ret 8`.
// Catalog label "CV_BanditCampPathScript" is wrong: the factory 0x00E68E60 (`push 90h`, this ctor) is registered at
// 0x00CD8700 under "S_VCK" / "V_ChickenKicking", and the PDB CV_ChickenKickingScript layout fits the stores.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, malloc @ 0x00BFEA0E (the map's 0x18-byte header),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (x2).
// Layout (retail = PDB -0x14, then -0x18 after the 12-byte STLport map): ChickenLanded +0x48 .. InfoDisplayed +0x60
// (not initialised), csargs +0x64, seh_Chicken +0x70, seh_ChickenMaster +0x80 (vtable 0x0127094C); size 0x90.
// Shape: the landed CQ_HeroSoulsBriarScript (0x00D80C30) model.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
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
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};
struct _Rb_tree_node_base {
    char                _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template <unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map/_Rb_tree (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
namespace NScript {
class CV_ChickenKickingScript : public CQuestScriptObject {
public:   // vtable 0x012E7318
    bool ChickenLanded;                            // +0x48
    long FinalSector;                              // +0x4c
    long DistanceBand;                             // +0x50
    bool KnowGhostHasGone;                         // +0x54
    bool SpectatorsCreated;                        // +0x55
    bool RanOff;                                   // +0x56
    bool TalkedTo;                                 // +0x57
    bool SpectatorsUnderAttack;                    // +0x58
    unsigned long PrizesWon;                       // +0x5c
    bool InfoDisplayed;                            // +0x60
    CRetailMap<0x18> csargs;                       // +0x64
    CScriptGameResourceObjectScriptedThingBase seh_Chicken;         // +0x70
    CScriptGameResourceObjectScriptedThingBase seh_ChickenMaster;   // +0x80
    virtual void v00();
    CV_ChickenKickingScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CV_ChickenKickingScript;
NScript::CV_ChickenKickingScript::CV_ChickenKickingScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}