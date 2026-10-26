#include <cstdio>
#include <new>
class CThing { public: int id; };
class CTCBase { public: CThing* Thing; CTCBase(CThing* t) : Thing(t) { } virtual ~CTCBase() { } };
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } virtual ~CIntelligentPointer() { } };
struct CLongAllocator { CLongAllocator() { } };
struct CLongAllocProxy : CLongAllocator { long* _M_data; CLongAllocProxy(const CLongAllocator& a, long* p) : CLongAllocator(a), _M_data(p) { } };
struct CLongVectorBase {
    long* _M_start; long* _M_finish; CLongAllocProxy _M_end_of_storage;
    CLongVectorBase(const CLongAllocator& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
struct CLongVector : CLongVectorBase { explicit CLongVector(const CLongAllocator& a = CLongAllocator()) : CLongVectorBase(a) { } };
struct CTCUserActionPointBase : CTCBase {
    CLongVector FreeActionPointIndexes, BlockedActionPointIndexes;
    long NumberOfActionPoints, FrameOfNextCheck;
    CIntelligentPointer PThing;
    CTCUserActionPointBase(CThing* thing) : CTCBase(thing), NumberOfActionPoints(0), FrameOfNextCheck(0) { }
};
int main()
{
    CThing owner;
    unsigned char raw[sizeof(CTCUserActionPointBase)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CTCUserActionPointBase* p = new (raw) CTCUserActionPointBase(&owner);
    if (p->Thing != &owner || p->FreeActionPointIndexes._M_start || p->FreeActionPointIndexes._M_finish || p->FreeActionPointIndexes._M_end_of_storage._M_data
        || p->BlockedActionPointIndexes._M_start || p->BlockedActionPointIndexes._M_end_of_storage._M_data
        || p->NumberOfActionPoints != 0 || p->FrameOfNextCheck != 0 || p->PThing.m_pItem != 0) { std::printf("007fec60_TEST FAIL\n"); return 1; }
    std::printf("007fec60_TEST PASS\n");
    return 0;
}