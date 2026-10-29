// Behaviour test for CTCRandomAppearanceMorph::~CTCRandomAppearanceMorph @ 0x00727b40: a live parent unregisters the
// appearance component (the CAGraphicAppearanceComponent subobject), a dead parent does not; the counted mesh morph set is
// released (deleted on the last reference) and the primitive handle block freed.
#include <stdio.h>
#include <stdlib.h>
static int g_unregistered = 0, g_morphDeleted = 0, g_infoDeleted = 0;
static void* g_unregisteredComponent = 0;
class CAGraphicAppearanceComponent { public:
    virtual void OnAppearanceClear() = 0;
    ~CAGraphicAppearanceComponent() { }
};
class CTCGraphicAppearance { public:
    void UnregisterComponent(CAGraphicAppearanceComponent& c) { g_unregistered++; g_unregisteredComponent = &c; }
};
class CThing { public:
    CTCGraphicAppearance* PTCGraphicAppearance;
    unsigned char Dead : 1;
};
class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteMorphSet(void* p) { g_morphDeleted++; free(p); }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); g_infoDeleted++; delete Info; } }
        Data = 0; Info = 0;
    }
};
class CTCBase { public:
    CThing* ParentThing;
    virtual ~CTCBase() { }
};
class CTCRandomAppearanceMorph : public CTCBase, public CAGraphicAppearanceComponent { public:
    CCountedPointer<int> PMeshMorphSet;
    unsigned int Seed;
    void** PrimHandles;
    virtual void OnAppearanceClear() { }
    virtual ~CTCRandomAppearanceMorph()
    {
        if (!ParentThing->Dead)
            ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);
        free(PrimHandles);
    }
};
static CTCRandomAppearanceMorph* make(CThing* parent, CCPPointerInfo* info)
{
    CTCRandomAppearanceMorph* m = new CTCRandomAppearanceMorph;
    m->ParentThing = parent; m->PMeshMorphSet.Data = (int*)info->Data; m->PMeshMorphSet.Info = info;
    m->PrimHandles = (void**)malloc(4 * sizeof(void*));
    return m;
}
int main()
{
    CTCGraphicAppearance appearance; CThing thing; thing.PTCGraphicAppearance = &appearance; thing.Dead = 0;
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteMorphSet; info->Data = malloc(8);
    CTCRandomAppearanceMorph* m = make(&thing, info);
    CAGraphicAppearanceComponent* sub = m;
    CTCBase* b = m; delete b;
    if (g_unregistered != 1 || g_unregisteredComponent != (void*)sub || g_morphDeleted != 0 || info->RefCount != 1) { printf("00727b40_TEST FAIL\n"); return 1; }
    thing.Dead = 1;
    m = make(&thing, info);
    b = m; delete b;
    if (g_unregistered != 1 || g_morphDeleted != 1 || g_infoDeleted != 1) { printf("00727b40_TEST FAIL\n"); return 1; }
    printf("00727b40_TEST PASS\n");
    return 0;
}