#include <cstdio>
#include <cstring>
class CBaseObject { public: int id; };
static int g_baseCopies;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; } void SetPItem(CBaseObject* item) { m_pItem = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); } };
class CEnginePersistentPrimitiveBase { public: virtual void v00() { } long m_references; };
class CEnginePrimitiveHandle { public: virtual void v00() { } CEnginePersistentPrimitiveBase* Data; CEnginePrimitiveHandle() : Data(0) { }
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& other) : Data(other.Data) { if (Data != 0) ++Data->m_references; } };
class CCombatActionBase { public: virtual void v00() { } unsigned char _body[0x114 - 4]; CCombatActionBase() { std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(const CCombatActionBase& o) { ++g_baseCopies; std::memcpy(_body, o._body, sizeof(_body)); } };
class CCreatureAction_DragonFireBlastBase : public CCombatActionBase { public: CIntelligentPointer PShot; CEnginePrimitiveHandle Line1; virtual void v00() { }
    CCreatureAction_DragonFireBlastBase() { }
    CCreatureAction_DragonFireBlastBase(const CCreatureAction_DragonFireBlastBase& other) : CCombatActionBase(other), PShot(other.PShot), Line1(other.Line1) { } };
int main()
{
    CBaseObject shot; CEnginePersistentPrimitiveBase prim; prim.m_references = 1;
    CCreatureAction_DragonFireBlastBase a; a.PShot.SetPItem(&shot); a.Line1.Data = &prim; a._body[7] = 3;
    CCreatureAction_DragonFireBlastBase b(a);
    if (g_baseCopies != 1 || b.PShot.GetPItem() != &shot || b.Line1.Data != &prim || prim.m_references != 2 || b._body[7] != 3
        || (char*)&b.PShot - (char*)&b != 0x114 || (char*)&b.Line1 - (char*)&b != 0x11c || sizeof(b) != 0x124) { std::printf("00953d60_TEST FAIL\n"); return 1; }
    a.Line1.Data = 0; CCreatureAction_DragonFireBlastBase c(a);
    if (c.Line1.Data != 0 || prim.m_references != 2) { std::printf("00953d60_TEST FAIL\n"); return 1; }
    std::printf("00953d60_TEST PASS\n");
    return 0;
}