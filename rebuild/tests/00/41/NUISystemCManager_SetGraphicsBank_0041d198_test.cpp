// behaviour test for 0x0041D198
#include <stdio.h>
class CGraphicDataBank { public: int x; };
struct CCPPointerInfo { int refs; };
static int g_dtors = 0;
template <class T> class CCountedPointer_0042918c {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer_0042918c() : Data(0), Info(0) {}
    CCountedPointer_0042918c(const CCountedPointer_0042918c& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->refs; }
    ~CCountedPointer_0042918c() { ++g_dtors; if (Info) --Info->refs; }
    void ShareData(T* data, CCPPointerInfo* info) { if (Info) --Info->refs; Data = data; Info = info; if (Info) ++Info->refs; }
    CCountedPointer_0042918c& operator=(const CCountedPointer_0042918c& o) { ShareData(o.Data, o.Info); return *this; }
};
namespace NUISystem {
class CManager {
public:
    void*                             __vftable;
    unsigned char                     _pad04[0x14];
    CCountedPointer_0042918c<CGraphicDataBank> PGraphicsBank;
    void SetGraphicsBank(CCountedPointer_0042918c<CGraphicDataBank> bank);
};
void CManager::SetGraphicsBank(CCountedPointer_0042918c<CGraphicDataBank> bank)
{
    PGraphicsBank = bank;
}
}
int main()
{
    CGraphicDataBank bank; CCPPointerInfo info = { 1 };
    int ok;
    {
        NUISystem::CManager m;
        CCountedPointer_0042918c<CGraphicDataBank> p; p.Data = &bank; p.Info = &info;
        m.SetGraphicsBank(p);
        ok = m.PGraphicsBank.Data == &bank && m.PGraphicsBank.Info == &info && info.refs == 2 && g_dtors == 1
          && (char*)&m.PGraphicsBank - (char*)&m == 0x18;
        p.Info = 0;
    }
    printf(ok ? "0041d198_TEST PASS\n" : "0041d198_TEST FAIL\n");
    return ok ? 0 : 1;
}