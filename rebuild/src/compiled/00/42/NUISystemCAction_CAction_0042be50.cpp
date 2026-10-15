// NUISystem::CAction::CAction() -- ActionType = 0, the two empty STLport lists (UIActionParams, ThingActionParams), Valid = true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042BE50, length 43 bytes.
// Leaf: ??0CAction@NUISystem
// Retail bytes: push ebp ; mov ebp,esp ; push ecx ; push esi ; mov esi,ecx ; and dword ptr [esi],0 ; lea eax,[ebp-1] ; push eax ; lea ecx,[esi+4] ; call list ;
//               lea eax,[ebp-1] ; push eax ; lea ecx,[esi+8] ; call list ; mov byte ptr [esi+0Ch],1 ; mov eax,esi ; pop esi ; leave ; ret
// Callees: the STLport list constructors (allocator by const reference; `[this]=0 ; malloc(0Ch) ; next=prev=node`)
// @ 0x0042AB01 (list<CActionParam*>) and @ 0x0042AB58 (list<long>).
// The manifest label "CRumbleDef::CRumbleDef" is propagated (CRumbleDef is 72 bytes and has no lists at +4/+8): the
// callers (NUISystem::CManager::AddActionsListenIgnoreOnList 0x0041DC9D and friends) allocate 0x10 bytes, run this
// constructor and hand the object to CCountedPointer<NUISystem::CAction> (0x0042AA29). PDB layout of CAction: ActionType +0,
// UIActionParams +4, ThingActionParams (+16 in the PDB's 12-byte debug lists; +8 with retail's 4-byte lists), Valid.
// Shape: a frame-pointer, size-optimised translation unit (`optimize("y",off)` + `("s",on)`, as the neighbouring
// landed NUISystem::CObservable constructor 0x0042BE7B): `and [esi],0` for the zero, the default allocator argument of
// each list a 1-byte empty temporary homed at [ebp-1] (both lists reuse the slot).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem {

struct CActionParam;

struct SRetailAllocator {                      // std::allocator<T>: empty, user constructor keeps it uninitialised
    SRetailAllocator() { }
};

struct CActionParamList {                      // STLport list<CActionParam*>, 4 bytes (the sentinel node pointer)
    void* m_pNode;
    CActionParamList(const SRetailAllocator& allocator = SRetailAllocator());   // 0x0042AB01
};

struct CLongList {                             // STLport list<long>, 4 bytes
    void* m_pNode;
    CLongList(const SRetailAllocator& allocator = SRetailAllocator());          // 0x0042AB58
};

enum EActionType { ACTION_NONE = 0 };

struct CAction {                               // retail 0x10 bytes
    EActionType      ActionType;               // +0x00
    CActionParamList UIActionParams;           // +0x04
    CLongList        ThingActionParams;        // +0x08
    bool             Valid;                    // +0x0c
    CAction();
};

CAction::CAction()
    : ActionType(ACTION_NONE), UIActionParams(), ThingActionParams(), Valid(true)
{
}

}