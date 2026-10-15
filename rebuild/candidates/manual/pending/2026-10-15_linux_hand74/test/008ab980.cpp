// Behaviour test for CScriptThing::operator= (0x008AB980): release the old block (destroy at zero), adopt + count the new one, self-share is a no-op.
#include <cstdio>

struct CScriptThing;
static int g_destroyed = 0;
static void DestroyObject(void* object) { (void)object; ++g_destroyed; }

struct CCPPointerInfo {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CCountedPointer_CScriptThing {
    CScriptThing* Data;
    CCPPointerInfo* Info;
    CCountedPointer_CScriptThing& operator=(const CCountedPointer_CScriptThing& other)
    {
        CScriptThing* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};
struct CScriptThing {
    CCountedPointer_CScriptThing PImp;
    virtual ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};

int main()
{
    CScriptThing implA, implB;
    implA.PImp.Data = 0; implA.PImp.Info = 0;
    implB.PImp.Data = 0; implB.PImp.Info = 0;
    CCPPointerInfo* blockA = new CCPPointerInfo;
    blockA->m_references = 1; blockA->m_pfnDestroy = DestroyObject; blockA->m_pObject = &implA;
    CCPPointerInfo blockB = { 1, DestroyObject, &implB };
    CScriptThing x, y;
    x.PImp.Data = &implA; x.PImp.Info = blockA;
    y.PImp.Data = &implB; y.PImp.Info = &blockB;
    CScriptThing* r = &(x = y);
    if (r != &x || x.PImp.Data != &implB || x.PImp.Info != &blockB || blockB.m_references != 2 || g_destroyed != 1) { std::printf("008ab980_TEST FAIL\n"); return 1; }
    x = y;  // same block: nothing changes
    if (blockB.m_references != 2 || g_destroyed != 1) { std::printf("008ab980_TEST FAIL\n"); return 1; }
    CScriptThing z;
    z.PImp.Data = 0; z.PImp.Info = 0;
    x = z;  // drop to null: 2 -> 1, no destroy
    if (x.PImp.Info != 0 || x.PImp.Data != 0 || blockB.m_references != 1 || g_destroyed != 1) { std::printf("008ab980_TEST FAIL\n"); return 1; }
    std::printf("008ab980_TEST PASS\n");
    return 0;
}
