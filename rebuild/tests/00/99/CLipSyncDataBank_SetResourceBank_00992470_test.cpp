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
class CResourceBank;
class CLipSyncDataBank {
public:
    unsigned char                  _pad00[0x170];
    CCountedPointer<CResourceBank> ResourceBank;         // +0x170 (Ego_r.pdb +0x1E8)
    bool                           SkipUpdate;           // +0x178 (Ego_r.pdb +0x1F8)
    void*                          PhonemeBank;          // +0x17C (Ego_r.pdb +0x1FC)
    void SetResourceBank(const CCountedPointer<CResourceBank>& bank);
};
void CLipSyncDataBank::SetResourceBank(const CCountedPointer<CResourceBank>& bank)
{
    ResourceBank = bank;
}

#include <cstdio>
#include <stdlib.h>
static int g_destroyed;
static void* g_destroyed_item;
static void __fastcall DestroyItem(void* item) { ++g_destroyed; g_destroyed_item = item; }
static CCPPointerInfo* NewInfo(void* item) { CCPPointerInfo* i = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo)); i->Count = 1; i->Destroy = DestroyItem; i->Item = item; return i; }
int main()
{
    static unsigned char raw[sizeof(CLipSyncDataBank)];
    CLipSyncDataBank* s = (CLipSyncDataBank*)raw;
    int item_a = 1, item_b = 2;
    CCountedPointer<CResourceBank> a; a.Data = (CResourceBank*)&item_a; a.Info = NewInfo(&item_a);
    CCountedPointer<CResourceBank> b; b.Data = (CResourceBank*)&item_b; b.Info = NewInfo(&item_b);
    bool ok = (char*)&s->ResourceBank - (char*)s == 0x170;
    s->ResourceBank.Data = 0; s->ResourceBank.Info = 0;
    s->SetResourceBank(a);
    ok = ok && s->ResourceBank.Data == a.Data && s->ResourceBank.Info == a.Info && a.Info->Count == 2 && g_destroyed == 0;
    s->SetResourceBank(a);                                 // same block: unchanged
    ok = ok && a.Info->Count == 2;
    a.Info->Count = 1;                           // the member holds the last reference
    s->SetResourceBank(b);
    ok = ok && g_destroyed == 1 && g_destroyed_item == &item_a && s->ResourceBank.Info == b.Info && b.Info->Count == 2;
    CCountedPointer<CResourceBank> none; none.Data = 0; none.Info = 0;
    s->SetResourceBank(none);
    ok = ok && b.Info->Count == 1 && g_destroyed == 1 && s->ResourceBank.Info == 0 && s->ResourceBank.Data == 0;
    std::printf(ok ? "00992470_TEST PASS\n" : "00992470_TEST FAIL\n");
    return ok ? 0 : 1;
}
