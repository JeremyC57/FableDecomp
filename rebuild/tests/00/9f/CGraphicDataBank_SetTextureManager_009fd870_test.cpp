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
class CTextureManager;
class CGraphicDataBank {
public:
    unsigned char                    _pad00[0x280];
    CCountedPointer<CTextureManager> TextureManager;     // retail +0x280 (Ego_r.pdb +0x2FC)
    void SetTextureManager(const CCountedPointer<CTextureManager>& manager);
};
void CGraphicDataBank::SetTextureManager(const CCountedPointer<CTextureManager>& manager)
{
    TextureManager = manager;
}

#include <cstdio>
#include <stdlib.h>
static int g_destroyed;
static void* g_destroyed_item;
static void __fastcall DestroyItem(void* item) { ++g_destroyed; g_destroyed_item = item; }
static CCPPointerInfo* NewInfo(void* item) { CCPPointerInfo* i = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo)); i->Count = 1; i->Destroy = DestroyItem; i->Item = item; return i; }
int main()
{
    static unsigned char raw[sizeof(CGraphicDataBank)];
    CGraphicDataBank* s = (CGraphicDataBank*)raw;
    int item_a = 1, item_b = 2;
    CCountedPointer<CTextureManager> a; a.Data = (CTextureManager*)&item_a; a.Info = NewInfo(&item_a);
    CCountedPointer<CTextureManager> b; b.Data = (CTextureManager*)&item_b; b.Info = NewInfo(&item_b);
    bool ok = (char*)&s->TextureManager - (char*)s == 0x280;
    s->TextureManager.Data = 0; s->TextureManager.Info = 0;
    s->SetTextureManager(a);
    ok = ok && s->TextureManager.Data == a.Data && s->TextureManager.Info == a.Info && a.Info->Count == 2 && g_destroyed == 0;
    s->SetTextureManager(a);                                 // same block: unchanged
    ok = ok && a.Info->Count == 2;
    a.Info->Count = 1;                           // the member holds the last reference
    s->SetTextureManager(b);
    ok = ok && g_destroyed == 1 && g_destroyed_item == &item_a && s->TextureManager.Info == b.Info && b.Info->Count == 2;
    CCountedPointer<CTextureManager> none; none.Data = 0; none.Info = 0;
    s->SetTextureManager(none);
    ok = ok && b.Info->Count == 1 && g_destroyed == 1 && s->TextureManager.Info == 0 && s->TextureManager.Data == 0;
    std::printf(ok ? "009fd870_TEST PASS\n" : "009fd870_TEST FAIL\n");
    return ok ? 0 : 1;
}
