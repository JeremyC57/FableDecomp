// CGameScriptThingManager::CGameScriptThingManager() -- an empty script-thing list and the shared null script thing, whose implementation is a fresh empty CGameScriptThing of the current world
// Leaf: ??0CGameScriptThingManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00704A80, 175 bytes.
// Callees: malloc @ 0x00BFEA0E (the list's 0x18-byte sentinel), CBaseClass::CBaseClass @ 0x0099A2D0 (NullThing's base),
//   operator new @ 0x00BFEA1A (the 0x48-byte CGameScriptThing and the 12-byte count block),
//   CGameScriptThing::CGameScriptThing(CWorld*) @ 0x008CFF60 (landed), the count block's per-type deleter @ 0x007044B0,
//   CScriptThing's by-value counted-pointer setter @ 0x007046D0 (landed under the propagated label
//   "SetAnimationMemoryPool"; it assigns +4/+8 from the argument then destroys the argument), operator delete @ 0x00BFE9BC.
// Globals: g_pMainGameComponent @ 0x013B86A0 (PWorld at +0x24, Ego_r.pdb CMainGameComponent).
// Layout (Ego_r.pdb, 0x18 bytes; retail 0x10): ScriptThings (list<CScriptThingInfo>, the 4-byte retail sentinel) +0x00,
//   NullThing (CScriptThing: vtable 0x01238C8C, PImp Data +4 / Info +8) +0x04.
// Shape: one zero register (esi) serves the list head clear, NullThing's PImp and the counted pointer's null Info;
//   `if (p) Info = new SCountBlock(p); else Info = 0;` (the null arms jump-threaded onto the zero register) keeps the temporary in ebp/esi; the
//   counted pointer is a NAMED local (a temporary would be built straight into the argument slot); its copy constructor
//   builds the by-value argument in the argument area (`sub esp,8 ; mov eax,esp`, add-ref) and the
//   temporary's destructor releases it after the call (`dec [esi] ; jne ; mov ecx,[esi+8] ; call [esi+4] ; delete`).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CWorld;
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                          // +0x24 scoped_ptr<CWorld>
    CWorld* GetWorld() const { return PWorld; }
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CGameScriptThing { public:
    unsigned char _body[0x48];
    CGameScriptThing(CWorld* world);               // 0x008CFF60
};
void __fastcall DeleteData(CGameScriptThing* object);   // 0x007044B0: the per-type deleter stored in the block

struct SCountBlock {                               // the shared count block of a CCountedPointer
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(CGameScriptThing*);   // +0x04
    CGameScriptThing* m_pObject;                   // +0x08
    SCountBlock(CGameScriptThing* object) : m_references(1), m_pfnDestroy(DeleteData), m_pObject(object) { }
};

struct CCountedPointer {                           // CCountedPointer<CGameScriptThing>: {Data, Info}
    CGameScriptThing* Data;
    SCountBlock*      Info;
    CCountedPointer(CGameScriptThing* p) : Data(p)
    {
        if (p)
            Info = new SCountBlock(p);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCountedPointerWords {                      // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:                 // vtable 0x01238C8C, 12 bytes
    CCountedPointerWords PImp;                     // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    void SetImp(CCountedPointer imp);              // 0x007046D0
};

struct SScriptThingInfoNode {                      // list<CGameScriptThingManager::CScriptThingInfo> node: 0x18 bytes
    SScriptThingInfoNode* m_pNext;
    SScriptThingInfoNode* m_pPrev;
    unsigned char         m_info[0x10];
};
struct CScriptThingInfoList {                      // retail: the sentinel pointer only
    SScriptThingInfoNode* m_pHead;
    CScriptThingInfoList()
    {
        m_pHead = 0;
        SScriptThingInfoNode* node = (SScriptThingInfoNode*)malloc(0x18);
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
};

class CGameScriptThingManager { public:
    CScriptThingInfoList ScriptThings;             // +0x00
    CScriptThing         NullThing;                // +0x04
    CGameScriptThingManager();
};

CGameScriptThingManager::CGameScriptThingManager()
{
    CCountedPointer imp(new CGameScriptThing(g_pMainGameComponent->GetWorld()));
    NullThing.SetImp(imp);
}