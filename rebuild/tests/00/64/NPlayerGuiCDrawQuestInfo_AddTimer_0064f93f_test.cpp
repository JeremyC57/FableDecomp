#include <cstdio>
// Behaviour test for 0064f93f.

// NPlayerGui::CDrawQuestInfo::AddTimer: the sprite is looked up from the symbol, a new element is created and appended through
// AddElement (which assigns the next ID), and the element's new ID is returned.
class CCharString { public: const char* m_pData; };
struct CQuestInfoElement { long ID; long ScriptID; float Scale; virtual ~CQuestInfoElement() { } };
struct CElement : CQuestInfoElement { long a; long graphic; long graphic2; float scale; };
template <class T> class CCountedPointer { public:
    T* m_pObject;
    explicit CCountedPointer(T* object) : m_pObject(object) { }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : m_pObject(other.m_pObject) { }
    T* operator->() const { return m_pObject; }
};
struct CDrawQuestInfo {
    long nextId; CQuestInfoElement* last;
    long GetScriptSpriteBySymbol(const CCharString& symbol) { return symbol.m_pData[0] == 'a' ? 100 : 200; }
    void AddElement(CCountedPointer<CQuestInfoElement> element) { element->ID = ++nextId; last = element.m_pObject; }
    long Add(long a, const CCharString& symbol, float scale)
    {
        long graphic = GetScriptSpriteBySymbol(symbol);
        CElement* e = new CElement; e->a = a; e->graphic = graphic; e->scale = scale;
        CCountedPointer<CElement> element(e);
        AddElement(element);
        return element->ID;
    }
};
int main()
{
    CDrawQuestInfo q; q.nextId = 4; q.last = 0;
    CCharString s; s.m_pData = "abc";
    int ok = 1;
    long id = q.Add(7, s, 0.5f);
    CElement* e = static_cast<CElement*>(q.last);
    if (id != 5 || e == 0 || e->ID != 5 || e->a != 7 || e->graphic != 100 || e->scale != 0.5f) ok = 0;
    s.m_pData = "z";
    if (q.Add(1, s, 1.0f) != 6 || static_cast<CElement*>(q.last)->graphic != 200) ok = 0;
    delete e; delete q.last;
    if (!ok) { std::printf("0064f93f_TEST FAIL\n"); return 1; }
    std::printf("0064f93f_TEST PASS\n");
    return 0;
}