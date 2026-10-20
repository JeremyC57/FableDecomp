// Behaviour test for 008f0b80: body copied from src/008f0b80.cpp (CTCTavernGameCardBase::GetPackPosition), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
static int g_destroyed = 0;
struct CDefPointee {
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
struct CTavernGameCardBaseDef : CDefPointee {
    unsigned char _pad_0x08[0x24];
    float         CardWidth;
    float         CardHeight;
    unsigned char _pad_0x34[0x30];
    C2DVector     PackOffset;
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* object = Object;
        long count = object->m_refCount - 1;
        object->m_refCount = count;
        if (count == 0)
            object->Destroy();
    }
    T* operator->() const { return Object; }
};
static CTavernGameCardBaseDef g_def; static int g_getCardDef = 0;
struct CTCTavernGameCardBase {
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out) { ++g_getCardDef; ++g_def.m_refCount; out->Object = &g_def; }
    C3DVector GetPackPosition();
};
C3DVector CTCTavernGameCardBase::GetPackPosition()
{
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    return C3DVector(def->CardWidth * def->PackOffset.X, def->CardHeight * def->PackOffset.Y, 0.0f);
}
int main()
{
    g_def.m_refCount = 1; g_def.CardWidth = 2.0f; g_def.CardHeight = 3.0f; g_def.PackOffset.X = 4.0f; g_def.PackOffset.Y = -1.5f;
    CTCTavernGameCardBase c;
    C3DVector p = c.GetPackPosition();
    CHECK(g_getCardDef == 1 && p.X == 8.0f && p.Y == -4.5f && p.Z == 0.0f); CHECK(g_def.m_refCount == 1 && g_destroyed == 0);
    // a def held only by the lookup is destroyed after the read
    g_def.m_refCount = 0; C3DVector q = c.GetPackPosition(); CHECK(q.X == 8.0f && g_destroyed == 1);
    if (fails == 0) printf("008f0b80_TEST PASS\n"); else printf("008f0b80_TEST FAIL\n");
    return fails;
}