// Behaviour test for CTCBoastUI::~CTCBoastUI @ 0x0056AC80: the boast UI leaves the global TC list, deletes each UI component it
// owns (in index order), empties the component vector, then the boast-group map and the CTCBase base are destroyed.
#include <stdio.h>
static int g_order[16]; static int g_n = 0; static int g_removedFromGlobal = 0, g_baseDtor = 0, g_mapDtor = 0;
class CComponent { public: int Id; virtual ~CComponent() { g_order[g_n++] = Id; } };
template <class T> class vector { public:                   // minimal vector of pointers
    T* start; T* finish; T* end_of_storage;
    vector() : start(0), finish(0), end_of_storage(0) { }
    unsigned int size() const { return (unsigned int)(finish - start); }
    T& operator[](unsigned int n) { return start[n]; }
    void push_back(const T& v) { if (finish == end_of_storage) { unsigned int c = size() ? size() * 2 : 4; T* s = new T[c];
        for (unsigned int i = 0; i < size(); ++i) s[i] = start[i]; finish = s + size(); delete[] start; start = s; end_of_storage = s + c; }
        *finish++ = v; }
    void clear() { finish = start; }
    ~vector() { delete[] start; }
};
class BoastGroupMap { public: ~BoastGroupMap() { g_mapDtor++; } };
class CTCBase { public:
    virtual ~CTCBase() { g_baseDtor++; }
    void RemoveFromGlobalTCs() { g_removedFromGlobal++; }
    void* ParentThing;
};
class CTCBoastUI : public CTCBase { public:
    BoastGroupMap         BoastGroups;
    bool                  Active;
    vector<CComponent*>   Components;
    virtual ~CTCBoastUI();
};
CTCBoastUI::~CTCBoastUI()
{
    RemoveFromGlobalTCs();
    for (unsigned int i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    if (Components.size() != 0) g_n = -1000;
}
static int fail(int line) { printf("0056ac80_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    CTCBoastUI* ui = new CTCBoastUI;
    for (int i = 0; i < 5; ++i) { CComponent* c = new CComponent; c->Id = 10 + i; ui->Components.push_back(c); }
    CTCBase* base = ui;
    delete base;
    if (g_removedFromGlobal != 1 || g_n != 5 || g_mapDtor != 1 || g_baseDtor != 1) return fail(__LINE__);
    for (int i = 0; i < 5; ++i) if (g_order[i] != 10 + i) return fail(__LINE__);
    CTCBoastUI* empty = new CTCBoastUI;
    delete empty;
    if (g_removedFromGlobal != 2 || g_n != 5 || g_baseDtor != 2) return fail(__LINE__);
    printf("0056ac80_TEST PASS\n");
    return 0;
}