#include <cstdio>

class CTCEnemy { public: int tag; };

class CPlayerGui { public:
    int calls;
    int lastArg;
    void UpdateMiniMapMarkers(bool force);   // out-of-line => real emitted symbol
};

void CPlayerGui::UpdateMiniMapMarkers(bool force) { calls++; lastArg = force; }

static CPlayerGui g_obj = {0,-1};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; T& operator*() const { return *Data; } };
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the source reads the private static PClass through Get()
    static CCountedPointer<T> PClass;
public:
    static T& __fastcall Get() { return *PClass; }
    static void __fastcall Set(T* p) { PClass.Data = p; PClass.Info = 0; }   // ?Set@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIXPAVCPlayerGui@@@Z
};
template <class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CPlayerGui>;
static int g_setPClass = (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&g_obj), 0);

static int g_baseCalled = 0;
static CTCEnemy* g_baseSelf = 0;

void __fastcall CTCEnemy_base_OnInitialActivate(CTCEnemy* enemy)
{
    g_baseCalled++;
    g_baseSelf = enemy;
}

void __fastcall CTCEnemy_OnInitialActivate(CTCEnemy* enemy);

int main()
{
    CTCEnemy e; e.tag = 42;
    CTCEnemy_OnInitialActivate(&e);
    if (g_obj.calls == 1 && g_obj.lastArg == 0 &&
        g_baseCalled == 1 && g_baseSelf == &e)
        printf("0076c830_TEST PASS\n");
    else
        printf("FAIL c=%d a=%d b=%d\n", g_obj.calls, g_obj.lastArg, g_baseCalled);
    return 0;
}