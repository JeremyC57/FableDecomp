#include <cstdio>
#include <stdlib.h>
class CThing { public: int id; };
static int g_globalAdds = 0;
class CTCBase { public:
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase() { }
    void AddToGlobalUpdateTCs() { if (!IsInGlobalUpdateTCs) { IsInGlobalUpdateTCs = true; ++g_globalAdds; } }
};
struct CIntelligentPointer {
    void* PData;
    CIntelligentPointer() : PData(0) { }
    virtual ~CIntelligentPointer() { }
};
template<class T>
struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
    ~CRetailList() { free(head); }
};
struct SCountedActionPointer { void* m_pObject; void* m_pBlock; };
struct SAttitudeTag { long m_tag; long m_attitude; };
class CTCScriptedControl : public CTCBase { public:
    CIntelligentPointer PScriptHandleImp; CRetailList<SCountedActionPointer> PActionList; bool Locked; long ScriptAIPriority;
    CRetailList<SAttitudeTag> AttitudeTags;
    CTCScriptedControl(CThing* thing) : CTCBase(thing), PScriptHandleImp(), PActionList(), Locked(false), ScriptAIPriority(0), AttitudeTags() { }
};
int main()
{
    CThing t; t.id = 5;
    CTCScriptedControl c(&t);
    if (c.ParentThing != &t || c.PScriptHandleImp.PData || c.Locked || c.ScriptAIPriority != 0 ||
        !c.PActionList.head || c.PActionList.head->next != c.PActionList.head || c.PActionList.head->prev != c.PActionList.head ||
        !c.AttitudeTags.head || c.AttitudeTags.head->next != c.AttitudeTags.head || c.AttitudeTags.head->prev != c.AttitudeTags.head ||
        (void*)c.AttitudeTags.head == (void*)c.PActionList.head) { std::printf("007156b0_TEST FAIL\n"); return 1; }
    std::printf("007156b0_TEST PASS\n");
    return 0;
}