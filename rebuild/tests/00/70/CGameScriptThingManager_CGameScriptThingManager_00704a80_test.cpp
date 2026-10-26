// Behaviour test for CGameScriptThingManager::CGameScriptThingManager (0x00704A80): empty list sentinel, the null script
// thing owning one counted CGameScriptThing of the current world (reference count 1 after the temporary died).
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
class CWorld;
static int g_live = 0, g_deletes = 0;
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                          // +0x24 scoped_ptr<CWorld>
    CWorld* GetWorld() const { return PWorld; }
};
CMainGameComponent* g_pMainGameComponent;

class CGameScriptThing { public:
    unsigned char _body[0x48];
    CWorld* m_world;
    CGameScriptThing(CWorld* world) : m_world(world) { ++g_live; }
    ~CGameScriptThing() { --g_live; }
};
void __fastcall DeleteData(CGameScriptThing* object) { ++g_deletes; delete object; }

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

class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00() {}
    CBaseClass() {}
};
struct CCountedPointerWords {                      // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:                 // vtable 0x01238C8C, 12 bytes
    CCountedPointerWords PImp;                     // +0x04
    virtual void v00() {}
    CScriptThing() : CBaseClass(), PImp() { }
    void SetImp(CCountedPointer imp)               // 0x007046D0 (landed body: assign then the argument dies)
    {
        SCountBlock* block = imp.Info;
        if ((SCountBlock*)PImp.Info != block) {
            SCountBlock* old = (SCountBlock*)PImp.Info;
            if (old && --old->m_references == 0) { old->m_pfnDestroy(old->m_pObject); delete old; }
            PImp.Data = imp.Data; PImp.Info = block;
            if (block) ++block->m_references;
        }
    }
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
        SScriptThingInfoNode* node = (SScriptThingInfoNode*)malloc(sizeof(SScriptThingInfoNode));
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

int main()
{
    int world = 0;
    CMainGameComponent mgc; mgc.PWorld = (CWorld*)&world; g_pMainGameComponent = &mgc;
    void* mem = malloc(sizeof(CGameScriptThingManager)); memset(mem, 0xcd, sizeof(CGameScriptThingManager));
    CGameScriptThingManager* m = new (mem) CGameScriptThingManager;
    int bad = 0;
    SScriptThingInfoNode* n = m->ScriptThings.m_pHead;
    if (!n || n->m_pNext != n || n->m_pPrev != n) bad = 1;
    CGameScriptThing* t = (CGameScriptThing*)m->NullThing.PImp.Data;
    SCountBlock* b = (SCountBlock*)m->NullThing.PImp.Info;
    if (!t || !b || t->m_world != (CWorld*)&world || b->m_pObject != t || b->m_references != 1 || b->m_pfnDestroy != DeleteData) bad = 2;
    if (g_live != 1 || g_deletes != 0) bad = 3;
    b->m_pfnDestroy(b->m_pObject);
    if (g_live != 0 || g_deletes != 1) bad = 4;
    if (bad) { std::printf("00704a80_TEST FAIL\n"); return 1; }
    std::printf("00704a80_TEST PASS\n");
    return 0;
}