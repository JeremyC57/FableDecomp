// Behaviour test for 008fa280: body copied from src/008fa280.cpp (CTCGift::IsGiftOfType), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
enum EGiftType { GIFT_TYPE_0, GIFT_TYPE_1, GIFT_TYPE_2 };
static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
};
struct CGiftDef : CDefBase {
    unsigned char _pad_0x08[0x20];
    EGiftType     Type;
};
struct CDefPointer_CGiftDef {
    CGiftDef* m_pDef;
    CDefPointer_CGiftDef() : m_pDef(0) { }
    ~CDefPointer_CGiftDef()
    {
        if (m_pDef) {
            if (--m_pDef->m_count == 0)
                m_pDef->Destroy();
        }
    }
    CGiftDef* operator->() const { return m_pDef; }
};
static CGiftDef* g_gift = 0; static int g_getSubDef = 0;
struct CThingBaseDef {
    bool GetSubDef(CDefPointer_CGiftDef* subDef) { ++g_getSubDef; if (!g_gift) return false; ++g_gift->m_count; subDef->m_pDef = g_gift; return true; }
};
struct CThingBaseDefPointer {
    CThingBaseDef* Object;
    CThingBaseDef* operator->() const { return Object; }
};
struct CThing {
    unsigned char  _pad_0x00[0x70];
    CThingBaseDefPointer PBaseDef;
    const CThingBaseDefPointer& GetPDef() const { return PBaseDef; }
};
struct CTCGift {
    unsigned char _pad_0x00[0x04];
    CThing*       ParentThing;
    bool IsGiftOfType(EGiftType type);
};
bool CTCGift::IsGiftOfType(EGiftType type)
{
    CDefPointer_CGiftDef def;
    if (ParentThing->GetPDef()->GetSubDef(&def))
        return def->Type == type;
    return false;
}
int main()
{
    static CThing thing; static CThingBaseDef baseDef; static CGiftDef gift; gift.m_count = 1; thing.PBaseDef.Object = &baseDef;
    CTCGift g; g.ParentThing = &thing;
    // no gift def: false
    g_gift = 0; CHECK(!g.IsGiftOfType(GIFT_TYPE_1)); CHECK(g_getSubDef == 1 && g_destroyed == 0);
    // gift def of type 1: only that type matches; the def is released each time
    g_gift = &gift; gift.Type = GIFT_TYPE_1;
    CHECK(g.IsGiftOfType(GIFT_TYPE_1)); CHECK(!g.IsGiftOfType(GIFT_TYPE_0)); CHECK(!g.IsGiftOfType(GIFT_TYPE_2)); CHECK(gift.m_count == 1 && g_getSubDef == 4);
    // a def held only by the lookup is destroyed
    gift.m_count = 0; CHECK(g.IsGiftOfType(GIFT_TYPE_1)); CHECK(g_destroyed == 1);
    if (fails == 0) printf("008fa280_TEST PASS\n"); else printf("008fa280_TEST FAIL\n");
    return fails;
}