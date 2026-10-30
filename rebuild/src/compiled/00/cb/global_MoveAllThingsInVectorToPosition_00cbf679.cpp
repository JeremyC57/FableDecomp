// MoveAllThingsInVectorToPosition(const vector<CScriptThing>& things, const C3DVector& position) -- take every living script thing in the vector under scripted control and send it to the position
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?MoveAllThingsInVectorToPosition@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF679, 99 bytes, `__fastcall` global (vector in ecx, position in edx),
//   plain `ret` (PDB ?MoveAllThingsInVectorToPosition@@YIXABV?$vector@VCScriptThing@@...@@ABVC3DVector@@@Z). No callers
//   in the retail image.
// Label: the catalog name "KillAllThingsInVector" is propagated from 0x00CBED82; this body starts scripting each thing
//   and moves it, and it is the PDB's MoveAllThingsInVectorToPosition (the only CScriptThing-vector global left in
//   the name set with a vector and a C3DVector).
// Callees: CScriptGameResourceObjectScriptedThingBase() @ 0x007E72A0, its MoveToPosition(pos, speed, type, bool, bool)
//   @ 0x007E72F0 (virtual slot 4, called directly on the local), its destructor @ 0x007E74D0; CScriptThing slot 75
//   IsAlive (+0x12c); CGameScriptInterface (global @ 0x0143E8F8) slot 8 StartScriptingEntity (+0x20, 0x0089B5B0) with
//   priority 4.
// Shape: frame-pointer size TU; one resource-object local for the whole loop (built before begin is read, destroyed
//   after it), begin read once into esi, end re-read every pass, the position kept in ebx; the speed is the float 0
//   (`fldz ; push ecx ; fstp [esp]`), the move type 0, the flags false / true.
class C3DVector { public:
    float X, Y, Z;
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}
enum EScriptEntityMoveType { MOVE_TYPE_WALK = 0 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
class CScriptGameResourceObjectScriptedThingBase {      // 0x10 bytes, vtable 0x0127094C
public:
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    void MoveToPosition(const C3DVector& position, float speed, EScriptEntityMoveType type, bool a, bool b);   // 0x007E72F0
    unsigned char _body[0x10];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

void __fastcall MoveAllThingsInVectorToPosition(const std::vector<CScriptThing>& things, const C3DVector& position)
{
    CScriptGameResourceObjectScriptedThingBase object;
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it) {
        if (it->IsAlive()) {
            g_pGameScriptInterface->StartScriptingEntity(*it, object, SCRIPT_AI_PRIORITY_4);
            object.MoveToPosition(position, 0.0f, MOVE_TYPE_WALK, false, true);
        }
    }
}