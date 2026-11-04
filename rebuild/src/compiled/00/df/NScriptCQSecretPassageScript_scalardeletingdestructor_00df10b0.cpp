// NScript::CQ_SecretPassageScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCQ_SecretPassageScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF10B0, 30 bytes, thiscall, `ret 4`; calls 00df10d0=NScript::CQ_SecretPassageScript::~CQ_SecretPassageScript ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012ddc30 slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00decad0 (slot 2) registers 10 entity scripts whose PDB Alloc outer is CQ_SecretPassageScript; PDB ??_G/??1CQ_SecretPassageScript@NScript@@; refs=1; was CVtable_012ddc30::`scalar deleting destructor'.
//   ??_GCQ_SecretPassageScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_GCQ_SecretPassageScript@NScript@@UAEPAXI@Z; Ego_r.pdb ??_ECQ_SecretPassageScript@NScript@@UAEPAXI@Z; Ego_r.pdb / FableWin.pdb ??1CQ_SecretPassageScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012ddc30, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CQ_SecretPassageScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00DEC7F0 (rebuild/src/compiled/00/de/NScriptCQSecretPassageScript_CQSecretPassageScript_00dec7f0.cpp), its own source unchanged. The constructor body is not part of this graded body.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CGameScriptInterface;
class CGameScriptInterfaceBase;

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

class CScriptDataBase;
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterfaceBase* Game;                    // +0x40 (second argument)
    CScriptDataBase*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CScriptBase(), Game(&game), Data(data) { }
};

#include "engine/CCountedPointer.h"
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

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
class CQ_SecretPassageScript : public CQuestScriptObject {
public:   // vtable 0x012DDC30
    bool MissionSucceeded;                         // +0x48
    bool MissionFailed;                            // +0x49
    bool MissionOver;                              // +0x4a
    bool MotherFound;                              // +0x4b
    bool DoorsBlocked;                             // +0x4c
    bool DoorsBeingDone;                           // +0x4d
    bool OkayToSpawn;                              // +0x4e
    bool QuestStartScreenShown;                    // +0x4f
    long GhostSightings;                           // +0x50
    CRetailMap<0x24> GlobalMymap;                  // +0x54 map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    CRetailMap<0x18> FlagMap;                      // +0x60 map<CCharString, bool>
    CScriptGameResourceObjectScriptedThingBase minions[11];   // +0x6c
    CScriptGameResourceObjectScriptedThingBase seh_Mother;   // +0x11c
    virtual void v00();
    CQ_SecretPassageScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
} // namespace NScript
using NScript::CQ_SecretPassageScript;

NScript::CQ_SecretPassageScript::CQ_SecretPassageScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CQuestScriptObject(data, game)
{
}