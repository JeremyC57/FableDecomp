// Behaviour test for NPlayerGui::CDrawQuestInfo::AddBarHealth (0x0064F885): an existing live bar for the same thing UID
// is reused (its ID returned); otherwise a new bar with the symbol's sprite is appended and its ID returned.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* m_pData; };
class CRGBColour { public: unsigned char R, G, B, A; };
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CThingPhysical { public: bool alive; u64 UID; };
struct CIntelligentPointer { const CThingPhysical* m_pItem; const CThingPhysical* GetPItem() const { return m_pItem; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { return p.m_pItem && p.m_pItem->alive; }
static long g_nextID = 100;
struct CQuestInfoElement {
    long ID; long type;
    CQuestInfoElement(long t) : ID(g_nextID++), type(t) {}
    virtual ~CQuestInfoElement() {}
    virtual long GetType() const { return type; }
};
struct CQuestInfoElementBar : CQuestInfoElement {
    long Graphic; float Scale; CIntelligentPointer PThing;
    CQuestInfoElementBar(const CThingPhysical* thing, const CRGBColour&, long graphic, float scale) : CQuestInfoElement(1), Graphic(graphic), Scale(scale) { PThing.m_pItem = thing; }
};
template <class T> class CCountedPointer { public:
    T* m_pObject;
    explicit CCountedPointer(T* o) : m_pObject(o) {}
    template <class U> CCountedPointer(const CCountedPointer<U>& o) : m_pObject(o.m_pObject) {}
    T* operator->() const { return m_pObject; }
};
struct SElementNode { SElementNode* Next; SElementNode* Prev; CCountedPointer<CQuestInfoElement> Data; SElementNode() : Data(0) {} };
struct CDrawQuestInfo {
    SElementNode* Elements; SElementNode nodes[8]; int used;
    long GetScriptSpriteBySymbol(const CCharString& s) { return (long)std::strlen(s.m_pData); }
    void AddElement(CCountedPointer<CQuestInfoElement> e)
    {
        SElementNode* n = &nodes[used++]; n->Data = e;
        n->Next = Elements; n->Prev = Elements->Prev; Elements->Prev->Next = n; Elements->Prev = n;
    }
    long AddBarHealth(const CThingPhysical* thing, const CRGBColour& colour, const CCharString& symbol, float scale);
};
long CDrawQuestInfo::AddBarHealth(const CThingPhysical* thing, const CRGBColour& colour, const CCharString& symbol, float scale)
{
    for (SElementNode* node = Elements->Next; node != Elements; node = node->Next) {
        CQuestInfoElement* element = node->Data.m_pObject;
        if (element->GetType() == 1) {
            CQuestInfoElementBar* bar = static_cast<CQuestInfoElementBar*>(element);
            if (IS_THING_ALIVE(bar->PThing) && bar->PThing.GetPItem()->UID == thing->UID)
                return element->ID;
        }
    }
    long graphic = GetScriptSpriteBySymbol(symbol);
    CCountedPointer<CQuestInfoElementBar> element(new CQuestInfoElementBar(thing, colour, graphic, scale));
    AddElement(element);
    return element->ID;
}
int main()
{
    SElementNode header; header.Next = header.Prev = &header;
    CDrawQuestInfo q; q.Elements = &header; q.used = 0;
    CQuestInfoElement other(2); q.AddElement(CCountedPointer<CQuestInfoElement>(&other));
    CThingPhysical troll = { true, 77 }, troll2 = { true, 77 }, bandit = { true, 5 };
    CRGBColour red = { 255, 0, 0, 255 }; CCharString sym = { "HEALTH" };
    int bad = 0;
    long id1 = q.AddBarHealth(&troll, red, sym, 1.0f);
    if (q.used != 2 || id1 != 101) bad = 1;
    if (q.AddBarHealth(&troll2, red, sym, 1.0f) != id1 || q.used != 2) bad = 2;   // same UID reuses the bar
    long id2 = q.AddBarHealth(&bandit, red, sym, 0.5f);
    if (id2 == id1 || q.used != 3) bad = 3;
    troll.alive = false;
    if (q.AddBarHealth(&troll2, red, sym, 1.0f) == id1 || q.used != 4) bad = 4;   // dead tracked thing: new bar
    if (static_cast<CQuestInfoElementBar*>(q.nodes[1].Data.m_pObject)->Graphic != 6) bad = 5;
    if (bad) { std::printf("0064f885_TEST FAIL\n"); return 1; }
    std::printf("0064f885_TEST PASS\n");
    return 0;
}