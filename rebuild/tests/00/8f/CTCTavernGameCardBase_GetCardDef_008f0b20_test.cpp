// Behaviour test for 008f0b20: body copied from src/008f0b20.cpp (CTCTavernGameCardBase::GetCardDef), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CDefPointee {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
class CTavernGameCardBaseDef : public CDefPointee { public: };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        T* old = Object;
        T* object = other.Object;
        if (old != object) {
            if (old) {
                if (--old->m_refCount == 0)
                    old->Destroy();
                Object = 0;
            }
            Object = object;
            if (object)
                ++object->m_refCount;
        }
        return *this;
    }
};
static CTavernGameCardBaseDef* g_cardDef = 0; static int g_getSubDef = 0;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CTavernGameCardBaseDef>* out);
};
bool CDefClassBase::GetSubDef(CDefPointer<CTavernGameCardBaseDef>* out)
{
    ++g_getSubDef;
    if (!g_cardDef) return false;
    ++g_cardDef->m_refCount; out->Object = g_cardDef; return true;
}
class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;
};
struct CTCTavernGameCardBase {
    void*   __vftable;
    CThing* ParentThing;
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);
};
void CTCTavernGameCardBase::GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out)
{
    CDefPointer<CTavernGameCardBaseDef> def;
    ParentThing->PDef->GetSubDef(&def);
    *out = def;
}
int main()
{
    static CTavernGameCardBaseDef cardA, cardB; static CDefClassBase classDef; static CThing thing; thing.PDef = &classDef;
    cardA.m_refCount = 1; cardB.m_refCount = 1;
    CTCTavernGameCardBase tc; tc.ParentThing = &thing;
    // 1: an empty out pointer receives the def (one reference held by the caller after the local is released)
    { CDefPointer<CTavernGameCardBaseDef> out; g_cardDef = &cardA; tc.GetCardDef(&out); CHECK(out.Object == &cardA); CHECK(cardA.m_refCount == 2); CHECK(g_getSubDef == 1); }
    CHECK(cardA.m_refCount == 1 && g_destroyed == 0);
    // 2: an out pointer already holding another def releases it
    { CDefPointer<CTavernGameCardBaseDef> out; out.Object = &cardA; ++cardA.m_refCount; g_cardDef = &cardB; tc.GetCardDef(&out); CHECK(out.Object == &cardB); CHECK(cardA.m_refCount == 1 && cardB.m_refCount == 2); }
    CHECK(cardB.m_refCount == 1);
    // 3: the same def already held: counts unchanged
    { CDefPointer<CTavernGameCardBaseDef> out; out.Object = &cardB; ++cardB.m_refCount; tc.GetCardDef(&out); CHECK(out.Object == &cardB && cardB.m_refCount == 2); }
    // 4: no sub-def: the out pointer is cleared
    { CDefPointer<CTavernGameCardBaseDef> out; out.Object = &cardB; ++cardB.m_refCount; g_cardDef = 0; tc.GetCardDef(&out); CHECK(out.Object == 0 && cardB.m_refCount == 1); }
    CHECK(g_destroyed == 0);
    if (fails == 0) printf("008f0b20_TEST PASS\n"); else printf("008f0b20_TEST FAIL\n");
    return fails;
}