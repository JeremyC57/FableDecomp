#include <cstdio>
struct CCPPointerInfo {
    long  Count;                                         // +0x00
    void (__fastcall *Destroy)(void* item);              // +0x04
    void* Item;                                          // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* data = o.Data;
        CCPPointerInfo* info = o.Info;
        if (Info != info)
        {
            if (Info)
            {
                if (--Info->Count == 0)
                {
                    Info->Destroy(Info->Item);
                    ::operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->Count;
        }
        return *this;
    }
};
namespace NUISystem {
class CAction;
class CFrontEndScreen {
public:
    unsigned char             _pad00[0x7c];
    CCountedPointer<CAction>  PLeftClickAction;          // +0x7c
    void AddActionOnLeftClicked(CCountedPointer<CAction>& action);
};
void CFrontEndScreen::AddActionOnLeftClicked(CCountedPointer<CAction>& action)
{
    PLeftClickAction = action;
}
}

#include <stdlib.h>
static int g_destroyed;
static void* g_destroyed_item;
static void __fastcall DestroyItem(void* item) { ++g_destroyed; g_destroyed_item = item; }
static CCPPointerInfo* NewInfo(void* item) { CCPPointerInfo* i = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo)); i->Count = 1; i->Destroy = DestroyItem; i->Item = item; return i; }
int main()
{
    using namespace NUISystem;
    static unsigned char raw[sizeof(CFrontEndScreen)];
    CFrontEndScreen* s = (CFrontEndScreen*)raw;
    int item_a = 1, item_b = 2;
    CCountedPointer<CAction> a; a.Data = (CAction*)&item_a; a.Info = NewInfo(&item_a);
    CCountedPointer<CAction> b; b.Data = (CAction*)&item_b; b.Info = NewInfo(&item_b);
    s->PLeftClickAction.Data = 0; s->PLeftClickAction.Info = 0;
    s->AddActionOnLeftClicked(a);
    bool ok = s->PLeftClickAction.Data == a.Data && s->PLeftClickAction.Info == a.Info && a.Info->Count == 2 && g_destroyed == 0;
    s->AddActionOnLeftClicked(a);                                 // same block: unchanged
    ok = ok && a.Info->Count == 2;
    a.Info->Count = 1;                                            // the member holds the last reference
    s->AddActionOnLeftClicked(b);
    ok = ok && g_destroyed == 1 && g_destroyed_item == &item_a && s->PLeftClickAction.Info == b.Info && b.Info->Count == 2;
    CCountedPointer<CAction> none; none.Data = 0; none.Info = 0;
    s->AddActionOnLeftClicked(none);
    ok = ok && b.Info->Count == 1 && g_destroyed == 1 && s->PLeftClickAction.Info == 0 && s->PLeftClickAction.Data == 0;
    std::printf(ok ? "00669280_TEST PASS\n" : "00669280_TEST FAIL\n");
    return ok ? 0 : 1;
}