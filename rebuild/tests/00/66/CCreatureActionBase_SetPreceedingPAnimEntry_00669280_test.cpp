class CCPPointerInfo { public:
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
class CAnimationEntry;
class CCreatureActionBase {
public:
    unsigned char                    _pad00[0x74];
    CCountedPointer<CAnimationEntry> PAnimEntry;             // +0x74
    CCountedPointer<CAnimationEntry> PPreceedingPAnimEntry;  // +0x7C
    unsigned char                    _pad84[0xb4 - 0x84];
    void SetPreceedingPAnimEntry(const CCountedPointer<CAnimationEntry>& entry);
};
void CCreatureActionBase::SetPreceedingPAnimEntry(const CCountedPointer<CAnimationEntry>& entry)
{
    PPreceedingPAnimEntry = entry;
}

#include <cstdio>
#include <stdlib.h>
static int g_destroyed;
static void* g_destroyed_item;
static void __fastcall DestroyItem(void* item) { ++g_destroyed; g_destroyed_item = item; }
static CCPPointerInfo* NewInfo(void* item) { CCPPointerInfo* i = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo)); i->Count = 1; i->Destroy = DestroyItem; i->Item = item; return i; }
int main()
{
    static unsigned char raw[sizeof(CCreatureActionBase)];
    CCreatureActionBase* s = (CCreatureActionBase*)raw;
    int item_a = 1, item_b = 2;
    CCountedPointer<CAnimationEntry> a; a.Data = (CAnimationEntry*)&item_a; a.Info = NewInfo(&item_a);
    CCountedPointer<CAnimationEntry> b; b.Data = (CAnimationEntry*)&item_b; b.Info = NewInfo(&item_b);
    bool ok = (char*)&s->PPreceedingPAnimEntry - (char*)s == 0x7c;
    s->PPreceedingPAnimEntry.Data = 0; s->PPreceedingPAnimEntry.Info = 0;
    s->SetPreceedingPAnimEntry(a);
    ok = ok && s->PPreceedingPAnimEntry.Data == a.Data && s->PPreceedingPAnimEntry.Info == a.Info && a.Info->Count == 2 && g_destroyed == 0;
    s->SetPreceedingPAnimEntry(a);                                 // same block: unchanged
    ok = ok && a.Info->Count == 2;
    a.Info->Count = 1;                           // the member holds the last reference
    s->SetPreceedingPAnimEntry(b);
    ok = ok && g_destroyed == 1 && g_destroyed_item == &item_a && s->PPreceedingPAnimEntry.Info == b.Info && b.Info->Count == 2;
    CCountedPointer<CAnimationEntry> none; none.Data = 0; none.Info = 0;
    s->SetPreceedingPAnimEntry(none);
    ok = ok && b.Info->Count == 1 && g_destroyed == 1 && s->PPreceedingPAnimEntry.Info == 0 && s->PPreceedingPAnimEntry.Data == 0;
    std::printf(ok ? "00669280_TEST PASS\n" : "00669280_TEST FAIL\n");
    return ok ? 0 : 1;
}
