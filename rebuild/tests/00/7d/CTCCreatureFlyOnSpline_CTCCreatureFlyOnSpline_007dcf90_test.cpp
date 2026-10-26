#include <cstdio>
class CThing { public: int id; };
class CCardinalSpline;
class CEnginePrimitiveHandle;
static int g_baseCalls = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCalls; }
};
struct CCountedPointer_CardinalSpline { CCardinalSpline* Object; void* CountBlock; CCountedPointer_CardinalSpline() : Object(0), CountBlock(0) { } };
struct CRetailVector_EngineHandle { CEnginePrimitiveHandle* First; CEnginePrimitiveHandle* Last; CEnginePrimitiveHandle* End;
    CRetailVector_EngineHandle() : First(0), Last(0), End(0) { } };
class CTCCreatureFlyOnSpline : public CTCBase_Calls { public:
    CCountedPointer_CardinalSpline PSpline; CRetailVector_EngineHandle EngineHandles; float Progress; float DeltaProgress; bool Looping;
    virtual void v00() { }
    CTCCreatureFlyOnSpline(CThing* thing) : CTCBase_Calls(thing), PSpline(), EngineHandles(), Progress(0.0f), DeltaProgress(0.01f), Looping(false) { }
};
int main()
{
    CThing thing = { 2 };
    CTCCreatureFlyOnSpline* f = new CTCCreatureFlyOnSpline(&thing);
    if (g_baseCalls != 1 || f->ParentThing != &thing || f->PSpline.Object != 0 || f->PSpline.CountBlock != 0 || f->EngineHandles.First != 0
        || f->EngineHandles.Last != 0 || f->EngineHandles.End != 0 || f->Progress != 0.0f || f->DeltaProgress != 0.01f || f->Looping)
    { std::printf("007dcf90_TEST FAIL\n"); return 1; }
    delete f;
    std::printf("007dcf90_TEST PASS\n");
    return 0;
}