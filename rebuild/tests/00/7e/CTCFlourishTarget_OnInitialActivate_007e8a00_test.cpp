#include <cstdio>
class CTCBase;
class CThing { public:
    unsigned char m_type; unsigned char m_flags; CTCBase* m_registered; int m_calls; bool m_lastRegistered;
    bool IsDead() const { return (m_flags & 1) != 0; }
    void SetComponentRegistered(bool registered, CTCBase* component) { ++m_calls; m_lastRegistered = registered; m_registered = registered ? component : 0; }
};
class CTCBase { public: void* vptrSlot; CThing* ParentThing; };
class CTCFlourishTarget : public CTCBase { public:
    void OnInitialActivate() { if (ParentThing->m_type == 1) ParentThing->SetComponentRegistered(true, this); }
};
int main()
{
    CThing t; t.m_type = 2; t.m_calls = 0; t.m_registered = 0;
    CTCFlourishTarget f; f.ParentThing = &t;
    f.OnInitialActivate(); bool skipped = t.m_calls == 0;
    t.m_type = 1; f.OnInitialActivate();
    if (!skipped || t.m_calls != 1 || !t.m_lastRegistered || t.m_registered != &f) { std::printf("007e8a00_TEST FAIL\n"); return 1; }
    std::printf("007e8a00_TEST PASS\n");
    return 0;
}