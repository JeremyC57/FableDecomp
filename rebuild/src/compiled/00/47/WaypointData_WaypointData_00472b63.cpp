#pragma optimize("s", on)
#pragma optimize("y", off)

// NUISystem WaypointData::~WaypointData  @ 0x00472B63  (__fastcall, ret 4)
// Name fix (NF387, T3 nf381 0x0044C0C0): the temporary's constructor call at 0x00472B6E goes to 0x0044C0C0, which is
//   CSubDefClassBase::CSubDefClassBase (??0CSubDefClassBase@@QAE@XZ, Ego_r / FableWin definition_manager.obj); the vtable stored after
//   it, 0x0123543C, is NSpeechGainManager::CDialogueLayerDef, whose retail RTTI direct base is CSubDefClassBase. Was the made-up
//   WaypointTemp::Construct. The other names here (WaypointData, Apply, Destroy, the two vtable globals) are unchanged (NAME_FIXES).
//
// Retail idiom:
//   push ebp / mov ebp,esp / sub esp,0x40    -> 0x40-byte stack temporary [ebp-0x40]
//   push esi / mov esi,ecx                    -> save this
//   lea ecx,[ebp-0x40] / call <ctor>          -> CSubDefClassBase() (masked)
//   push &temp / push [ebp+8] / mov ecx,esi
//   mov [ebp-0x40],0x123543c                  -> temp.head = &vtblApply
//   call <apply>                              -> this->Apply(arg, &temp)  (masked, fwd)
//   lea ecx,[ebp-0x40]
//   mov [ebp-0x40],0x1230ba0                  -> temp.head = &vtblDestroy
//   call <dtor>                               -> temp.Destroy()   (masked)
//   pop esi / leave / ret 4
//
// The two immediate stores are the classic MSVC inlined vptr stamps: a local
// object whose vtable pointer is set after construction (derived table) and
// re-set before teardown (base table).  Modelled as explicit head-pointer
// assignments so the compiler emits the two `mov [ebp-0x40], imm32` stores
// exactly where retail schedules them.  All three callees and the two vtable
// addresses are relocation-masked in parity, so they are declared extern.

extern void* const WaypointApplyVtbl_0123543c[];    // 0x0123543c
extern void* const WaypointDestroyVtbl_01230ba0[];  // 0x01230ba0

// 0x40-byte stack temporary (a def object: the vtable stored after the constructor, 0x0123543C, is
// NSpeechGainManager::CDialogueLayerDef).  Its constructor call is the def base constructor
// CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0; the local model of the base spans the whole
// 0x40-byte temporary, so no derived constructor is generated (VC7.1 would emit it as a COMDAT).
// WaypointTemp is the view the two callees take; head = inlined vptr slot; Destroy is a
// this-in-ecx member taking no stack args (masked callee).
class CSubDefClassBase { public:     // the def base; constructor @ 0x0044C0C0
    void* head;                 // +0x00  inlined vtable pointer slot
    char  pad[0x40 - 4];        // the rest of the 0x40-byte frame temporary
    CSubDefClassBase();         // lea ecx,[ebp-0x40]; call 0x0044C0C0
};

struct WaypointTemp {
    void* head;                 // +0x00  inlined vtable pointer slot
    char  pad[0x40 - 4];        // pad out to the full 0x40-byte frame temp
    void Destroy();             // lea ecx,[ebp-0x40]; call
};

struct WaypointData {
    // this-in-ecx member taking two stack args (arg, then &temp pushed first):
    //   push &temp; push arg; mov ecx,this; call
    void Apply(void* arg, WaypointTemp* temp);

    void Dtor(void* arg);       // the target: ~WaypointData(arg), ret 4
};

void WaypointData::Dtor(void* arg)
{
    CSubDefClassBase def;       // lea ecx,[ebp-0x40]; call CSubDefClassBase::CSubDefClassBase
    WaypointTemp& temp = reinterpret_cast<WaypointTemp&>(def);

    temp.head = (void*)WaypointApplyVtbl_0123543c;
    this->Apply(arg, &temp);
    temp.head = (void*)WaypointDestroyVtbl_01230ba0;
    temp.Destroy();
}