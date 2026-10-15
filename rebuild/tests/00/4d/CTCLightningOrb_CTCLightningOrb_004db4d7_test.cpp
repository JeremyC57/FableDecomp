#include <cstdio>
struct CThing { int id; };
struct CTCBase { CThing* m_pThing; long m_baseWord; CTCBase(CThing* o) : m_pThing(o), m_baseWord(0) { } virtual ~CTCBase() { } };
struct CBaseIntelligentPointer { void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } virtual ~CBaseIntelligentPointer() { } };
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() { } };
struct CLightningOrbDefPointer { const void* m_pDef; CLightningOrbDefPointer() : m_pDef(0) { } };
struct CSplinePointer { void* m_pObject; void* m_pBlock; CSplinePointer() : m_pObject(0), m_pBlock(0) { } };
struct CTCLightningOrb : CTCBase {
    CIntelligentPointer<CThing> PParent, PTarget; CLightningOrbDefPointer PDef; long Timer, LifeTime, NumFramesForSplineTraversal; CSplinePointer PSpline;
    CTCLightningOrb(CThing* owner) : CTCBase(owner), Timer(-1), LifeTime(-1), NumFramesForSplineTraversal(1) { }
};
int main()
{
    CThing t; t.id = 2;
    CTCLightningOrb* o = new CTCLightningOrb(&t);
    if (o->m_pThing != &t || o->PParent.m_pItem || o->PTarget.m_pItem || o->PDef.m_pDef || o->Timer != -1 || o->LifeTime != -1
        || o->NumFramesForSplineTraversal != 1 || o->PSpline.m_pObject || o->PSpline.m_pBlock) { std::printf("004db4d7_TEST FAIL\n"); return 1; }
    delete o;
    std::printf("004db4d7_TEST PASS\n");
    return 0;
}