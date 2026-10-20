// Behaviour test for 005bf654: src/005bf654.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NInventory::CTCInventoryBase::AddItemToInventory(CThing* item, bool silent, bool fromShop, long price, bool noTutorials)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NInventory::CTCInventoryBase::AddItemToInventory(CThing* item, bool silent, bool fromShop, long price, bool noTutorials) -- a hero-title item sets the owner's title instead; otherwise the item goes into the category matching its identifier and OnItemAdded is raised; true when added
// Retail 0x005BF654, 297 bytes, `ret 14h`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const CHeroTitleDef>*) const 0x005B1E5A (on the item's def),
// std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (inline GetInterface: the owner's hero component 0x29)
// and the const LowerBound 0x004365B0 (the item's inventory-item component 0x1A), CTCHero::SetTitle(long) 0x006CA400,
// CTCInventoryItem::GetCategoryIdentifier() const 0x005D91F0, FindCategory(first, last, identifier) 0x005C08D3
// (__fastcall), NInventory::CCategory::AddItem(CThing*, bool, long) 0x005BF5A4, this's slot 84 (+0x150, OnItemAdded(long,
// bool, bool) 0x005BE028), the defs' slot 1 on release.
// Shape: the title sub-def out pointer is a zeroed frame word; the item's def pointer is copied (inline add-ref) for the
// GetSubDef call only, and the bool result is homed in the dead `item` slot's high byte; the title path returns false;
// the inventory-item component pointer starts at 0 (the hit path takes it in ecx, the miss path re-reads the zero);
// both exits release the title sub-def through the pointee's inline Release().
// Layout: ParentThing +0x04, Categories +0x20 (12-byte elements view: first/last); retail CThing PDef +0x70, def index
// word +0x8C.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit) const;                       // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound template member (compiled in the caller's TU):
// binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound template member (compiled in the caller's TU):
// binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (DecRefToZero(RefCount)) Destroy(); }
};
struct CHeroTitleDef : CDefPointeeBase {};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CHeroTitleDef>* out) const;   // 0x005B1E5A
};
void* FindDef_s7r_0();
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_0(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CHeroTitleDef>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
    unsigned char _pad_0x74[0x8c - 0x74];
    unsigned short DefIndex;                         // +0x8c
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCHero {                                     // interface type 0x29
    void SetTitle(long titleObjectDef);              // 0x006CA400
};
struct CTCInventoryItem {                            // interface type 0x1A
    long GetCategoryIdentifier() const;              // 0x005D91F0
};
namespace NInventory {
struct CCategory {
    bool AddItem(CThing* thing, bool silent, long price);   // 0x005BF5A4
};
}
NInventory::CCategory* __fastcall FindCategory(NInventory::CCategory* first, NInventory::CCategory* last, long ident);   // 0x005C08D3
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindCategory does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindCategory instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) NInventory::CCategory* __fastcall FindCategory(NInventory::CCategory* first, NInventory::CCategory* last, long ident)
{
    long r = TmplSink_s7_1(first, ident);   // the search over [first, last) for the category with this identifier
    return r ? (NInventory::CCategory*)r : last;
}
#pragma auto_inline(on)

namespace NInventory {
struct CTCInventoryBase {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void OnItemAdded(long itemDefIndex, bool fromShop, bool noTutorials);   // slot 84 (+0x150)
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x008[0x20 - 0x08];
    CCategory* m_categoriesFirst;                    // +0x20 Categories
    CCategory* m_categoriesLast;                     // +0x24
    bool AddItemToInventory(CThing* item, bool silent, bool fromShop, long price, bool noTutorials);
};

bool CTCInventoryBase::AddItemToInventory(CThing* item, bool silent, bool fromShop, long price, bool noTutorials)
{
    CDefPointer<CHeroTitleDef> titleDef;
    bool isTitle;
    {
        CDefPointer<CDefClassBase> def(item->PDef);
        isTitle = def->GetSubDef(&titleDef);
    }
    if (isTitle) {
        CTCHero* hero;
        if (ParentThing->GetInterface(&hero, 0x29))
            hero->SetTitle(item->DefIndex);
    } else {
        const CThing* constItem = item;
        CTCInventoryItem* inventoryItem = 0;
        constItem->GetInterface(&inventoryItem, 0x1a);
        CCategory* category = FindCategory(m_categoriesFirst, m_categoriesLast, inventoryItem->GetCategoryIdentifier());
        if (category->AddItem(item, silent, price)) {
            OnItemAdded(item->DefIndex, fromShop, noTutorials);
            return true;
        }
    }
    return false;
}
}

// ---- stubs and driver ----
void NInventory::CTCInventoryBase::v000() {}
void NInventory::CTCInventoryBase::v001() {}
void NInventory::CTCInventoryBase::v002() {}
void NInventory::CTCInventoryBase::v003() {}
void NInventory::CTCInventoryBase::v004() {}
void NInventory::CTCInventoryBase::v005() {}
void NInventory::CTCInventoryBase::v006() {}
void NInventory::CTCInventoryBase::v007() {}
void NInventory::CTCInventoryBase::v008() {}
void NInventory::CTCInventoryBase::v009() {}
void NInventory::CTCInventoryBase::v010() {}
void NInventory::CTCInventoryBase::v011() {}
void NInventory::CTCInventoryBase::v012() {}
void NInventory::CTCInventoryBase::v013() {}
void NInventory::CTCInventoryBase::v014() {}
void NInventory::CTCInventoryBase::v015() {}
void NInventory::CTCInventoryBase::v016() {}
void NInventory::CTCInventoryBase::v017() {}
void NInventory::CTCInventoryBase::v018() {}
void NInventory::CTCInventoryBase::v019() {}
void NInventory::CTCInventoryBase::v020() {}
void NInventory::CTCInventoryBase::v021() {}
void NInventory::CTCInventoryBase::v022() {}
void NInventory::CTCInventoryBase::v023() {}
void NInventory::CTCInventoryBase::v024() {}
void NInventory::CTCInventoryBase::v025() {}
void NInventory::CTCInventoryBase::v026() {}
void NInventory::CTCInventoryBase::v027() {}
void NInventory::CTCInventoryBase::v028() {}
void NInventory::CTCInventoryBase::v029() {}
void NInventory::CTCInventoryBase::v030() {}
void NInventory::CTCInventoryBase::v031() {}
void NInventory::CTCInventoryBase::v032() {}
void NInventory::CTCInventoryBase::v033() {}
void NInventory::CTCInventoryBase::v034() {}
void NInventory::CTCInventoryBase::v035() {}
void NInventory::CTCInventoryBase::v036() {}
void NInventory::CTCInventoryBase::v037() {}
void NInventory::CTCInventoryBase::v038() {}
void NInventory::CTCInventoryBase::v039() {}
void NInventory::CTCInventoryBase::v040() {}
void NInventory::CTCInventoryBase::v041() {}
void NInventory::CTCInventoryBase::v042() {}
void NInventory::CTCInventoryBase::v043() {}
void NInventory::CTCInventoryBase::v044() {}
void NInventory::CTCInventoryBase::v045() {}
void NInventory::CTCInventoryBase::v046() {}
void NInventory::CTCInventoryBase::v047() {}
void NInventory::CTCInventoryBase::v048() {}
void NInventory::CTCInventoryBase::v049() {}
void NInventory::CTCInventoryBase::v050() {}
void NInventory::CTCInventoryBase::v051() {}
void NInventory::CTCInventoryBase::v052() {}
void NInventory::CTCInventoryBase::v053() {}
void NInventory::CTCInventoryBase::v054() {}
void NInventory::CTCInventoryBase::v055() {}
void NInventory::CTCInventoryBase::v056() {}
void NInventory::CTCInventoryBase::v057() {}
void NInventory::CTCInventoryBase::v058() {}
void NInventory::CTCInventoryBase::v059() {}
void NInventory::CTCInventoryBase::v060() {}
void NInventory::CTCInventoryBase::v061() {}
void NInventory::CTCInventoryBase::v062() {}
void NInventory::CTCInventoryBase::v063() {}
void NInventory::CTCInventoryBase::v064() {}
void NInventory::CTCInventoryBase::v065() {}
void NInventory::CTCInventoryBase::v066() {}
void NInventory::CTCInventoryBase::v067() {}
void NInventory::CTCInventoryBase::v068() {}
void NInventory::CTCInventoryBase::v069() {}
void NInventory::CTCInventoryBase::v070() {}
void NInventory::CTCInventoryBase::v071() {}
void NInventory::CTCInventoryBase::v072() {}
void NInventory::CTCInventoryBase::v073() {}
void NInventory::CTCInventoryBase::v074() {}
void NInventory::CTCInventoryBase::v075() {}
void NInventory::CTCInventoryBase::v076() {}
void NInventory::CTCInventoryBase::v077() {}
void NInventory::CTCInventoryBase::v078() {}
void NInventory::CTCInventoryBase::v079() {}
void NInventory::CTCInventoryBase::v080() {}
void NInventory::CTCInventoryBase::v081() {}
void NInventory::CTCInventoryBase::v082() {}
void NInventory::CTCInventoryBase::v083() {}
using namespace NInventory;
static void* g_title = 0; static int g_setTitle = 0, g_add = 0, g_added = 0, g_release = 0, g_destroy = 0; static long g_titleIndex = -1, g_seenIdent = -1, g_addPrice = 0;
static bool g_addResult = true; static CCategory g_cats[3]; static long g_catResult = 0;
long TmplSink_s7_0(const void*, long) { return (long)g_title; }
long TmplSink_s7_1(const void* first, long ident) { CHECK(first == g_cats); g_seenIdent = ident; return g_catResult; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroy; }
static const void* g_hasBits[4]; static long g_hasBit[4];
bool CPresenceBits::test(long bit) const { for (int i = 0; i < 4; ++i) if (g_hasBits[i] == this && g_hasBit[i] == bit) return true; return false; }
void CTCHero::SetTitle(long title) { ++g_setTitle; g_titleIndex = title; }
long CTCInventoryItem::GetCategoryIdentifier() const { return *(const long*)this; }
bool CCategory::AddItem(CThing*, bool, long price) { ++g_add; g_addPrice = price; return g_addResult; }
static long g_onAdded = -1; static bool g_onShop = false, g_onNoTut = false;
void CTCInventoryBase::OnItemAdded(long index, bool fromShop, bool noTutorials) { ++g_added; g_onAdded = index; g_onShop = fromShop; g_onNoTut = noTutorials; }
static void addTC(CThing& t, CInterfaceMap::SEntry* e, int n, int slot) { t.m_interfaces.first = e; t.m_interfaces.last = e + n; g_hasBits[slot] = &t.m_present; g_hasBit[slot] = e[0].type; }
int main()
{
    static CThing owner, item; static CTCHero hero; static long ident = 4; static CDefClassBase itemDef; static CHeroTitleDef title;
    itemDef.RefCount = 5; title.RefCount = 5;
    CInterfaceMap::SEntry oe[1] = { { 0x29, &hero } }; addTC(owner, oe, 1, 0);
    CInterfaceMap::SEntry ie[1] = { { 0x1a, &ident } }; addTC(item, ie, 1, 1);
    item.PDef.Object = &itemDef; item.DefIndex = 77;
    static CTCInventoryBase inv; inv.ParentThing = &owner; inv.m_categoriesFirst = g_cats; inv.m_categoriesLast = g_cats + 3;
    // a title item: the hero's title is set, nothing added
    g_title = &title;
    CHECK(inv.AddItemToInventory(&item, false, true, 10, false) == false); CHECK(g_setTitle == 1 && g_titleIndex == 77 && g_add == 0 && itemDef.RefCount == 5 && title.RefCount == 4);
    // an ordinary item: found category accepts it
    g_title = 0; g_catResult = (long)&g_cats[1];
    CHECK(inv.AddItemToInventory(&item, true, true, 25, true) == true); CHECK(g_seenIdent == 4 && g_add == 1 && g_addPrice == 25 && g_added == 1 && g_onAdded == 77 && g_onShop && g_onNoTut);
    g_addResult = false; CHECK(inv.AddItemToInventory(&item, true, false, 3, false) == false && g_added == 1);
    CHECK(itemDef.RefCount == 5 && g_release == 0 && g_destroy == 0);
    item.PDef.Object = 0;
    if (fails == 0) printf("005bf654_TEST PASS\n"); else printf("005bf654_TEST FAIL\n");
    return fails;
}