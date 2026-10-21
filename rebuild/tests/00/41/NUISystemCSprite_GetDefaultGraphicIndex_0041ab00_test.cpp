#include <cstdio>
namespace NUISystem {
class CDefPointeeBase {
public:
    virtual void _slot00();
    virtual void DestroyDef() const;                     // slot 1 (+4)
    mutable long RefCount;                               // +0x04
    void ReleaseDef() const { if (--RefCount == 0) DestroyDef(); }
};
class CUIStateDef {
public:
    unsigned char _pad00[0x3c];
    unsigned long GraphicIndex;                          // +0x3c
    unsigned char _pad40[0x80 - 0x40];
};
class CUIDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x3c - 0x08];
    long          Type;                                  // +0x3c
    CUIStateDef*  States_M_start;                        // +0x40 vector<CUIStateDef>
    CUIStateDef*  States_M_finish;                       // +0x44
    CUIStateDef*  States_M_end_of_storage;               // +0x48
};
}
template <class T> class CDefPointer {
public:
    T* Object;                                           // +0x00
    CDefPointer(const CDefPointer& x) : Object(x.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) Object->ReleaseDef(); }
    T* operator->() const { return Object; }
};
namespace NUISystem {
class CSprite {
public:
    virtual void _slot000(); virtual void _slot001(); virtual void _slot002(); virtual void _slot003();
    virtual void _slot004(); virtual void _slot005(); virtual void _slot006(); virtual void _slot007();
    virtual void _slot008(); virtual void _slot009(); virtual void _slot010(); virtual void _slot011();
    virtual void _slot012(); virtual void _slot013(); virtual void _slot014(); virtual void _slot015();
    virtual void _slot016(); virtual void _slot017(); virtual void _slot018(); virtual void _slot019();
    virtual void _slot020(); virtual void _slot021(); virtual void _slot022(); virtual void _slot023();
    virtual void _slot024(); virtual void _slot025(); virtual void _slot026(); virtual void _slot027();
    virtual void _slot028(); virtual void _slot029(); virtual void _slot030(); virtual void _slot031();
    virtual void _slot032(); virtual void _slot033(); virtual void _slot034(); virtual void _slot035();
    virtual void _slot036(); virtual void _slot037(); virtual void _slot038(); virtual void _slot039();
    virtual void _slot040(); virtual void _slot041(); virtual void _slot042(); virtual void _slot043();
    virtual void _slot044(); virtual void _slot045(); virtual void _slot046(); virtual void _slot047();
    virtual void _slot048(); virtual void _slot049(); virtual void _slot050(); virtual void _slot051();
    virtual void _slot052(); virtual void _slot053(); virtual void _slot054(); virtual void _slot055();
    virtual void _slot056(); virtual void _slot057(); virtual void _slot058(); virtual void _slot059();
    virtual void _slot060(); virtual void _slot061(); virtual void _slot062(); virtual void _slot063();
    virtual void _slot064(); virtual void _slot065(); virtual void _slot066(); virtual void _slot067();
    virtual void _slot068(); virtual void _slot069(); virtual void _slot070(); virtual void _slot071();
    virtual void _slot072(); virtual void _slot073(); virtual void _slot074(); virtual void _slot075();
    virtual void _slot076(); virtual void _slot077(); virtual void _slot078(); virtual void _slot079();
    virtual void _slot080(); virtual void _slot081(); virtual void _slot082(); virtual void _slot083();
    virtual void _slot084(); virtual void _slot085(); virtual void _slot086(); virtual void _slot087();
    virtual void _slot088(); virtual void _slot089(); virtual void _slot090(); virtual void _slot091();
    virtual void _slot092(); virtual void _slot093(); virtual void _slot094(); virtual void _slot095();
    virtual void _slot096(); virtual void _slot097(); virtual void _slot098(); virtual void _slot099();
    virtual void _slot100(); virtual void _slot101(); virtual void _slot102(); virtual void _slot103();
    virtual void _slot104(); virtual void _slot105(); virtual void _slot106(); virtual void _slot107();
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;  // slot 0x1b0
    unsigned long GetDefaultGraphicIndex();
};
unsigned long CSprite::GetDefaultGraphicIndex()
{
    return LoadAndReturnDef()->States_M_start[0].GraphicIndex;
}
}

#include <new>
void NUISystem::CSprite::_slot000() {}
void NUISystem::CSprite::_slot001() {}
void NUISystem::CSprite::_slot002() {}
void NUISystem::CSprite::_slot003() {}
void NUISystem::CSprite::_slot004() {}
void NUISystem::CSprite::_slot005() {}
void NUISystem::CSprite::_slot006() {}
void NUISystem::CSprite::_slot007() {}
void NUISystem::CSprite::_slot008() {}
void NUISystem::CSprite::_slot009() {}
void NUISystem::CSprite::_slot010() {}
void NUISystem::CSprite::_slot011() {}
void NUISystem::CSprite::_slot012() {}
void NUISystem::CSprite::_slot013() {}
void NUISystem::CSprite::_slot014() {}
void NUISystem::CSprite::_slot015() {}
void NUISystem::CSprite::_slot016() {}
void NUISystem::CSprite::_slot017() {}
void NUISystem::CSprite::_slot018() {}
void NUISystem::CSprite::_slot019() {}
void NUISystem::CSprite::_slot020() {}
void NUISystem::CSprite::_slot021() {}
void NUISystem::CSprite::_slot022() {}
void NUISystem::CSprite::_slot023() {}
void NUISystem::CSprite::_slot024() {}
void NUISystem::CSprite::_slot025() {}
void NUISystem::CSprite::_slot026() {}
void NUISystem::CSprite::_slot027() {}
void NUISystem::CSprite::_slot028() {}
void NUISystem::CSprite::_slot029() {}
void NUISystem::CSprite::_slot030() {}
void NUISystem::CSprite::_slot031() {}
void NUISystem::CSprite::_slot032() {}
void NUISystem::CSprite::_slot033() {}
void NUISystem::CSprite::_slot034() {}
void NUISystem::CSprite::_slot035() {}
void NUISystem::CSprite::_slot036() {}
void NUISystem::CSprite::_slot037() {}
void NUISystem::CSprite::_slot038() {}
void NUISystem::CSprite::_slot039() {}
void NUISystem::CSprite::_slot040() {}
void NUISystem::CSprite::_slot041() {}
void NUISystem::CSprite::_slot042() {}
void NUISystem::CSprite::_slot043() {}
void NUISystem::CSprite::_slot044() {}
void NUISystem::CSprite::_slot045() {}
void NUISystem::CSprite::_slot046() {}
void NUISystem::CSprite::_slot047() {}
void NUISystem::CSprite::_slot048() {}
void NUISystem::CSprite::_slot049() {}
void NUISystem::CSprite::_slot050() {}
void NUISystem::CSprite::_slot051() {}
void NUISystem::CSprite::_slot052() {}
void NUISystem::CSprite::_slot053() {}
void NUISystem::CSprite::_slot054() {}
void NUISystem::CSprite::_slot055() {}
void NUISystem::CSprite::_slot056() {}
void NUISystem::CSprite::_slot057() {}
void NUISystem::CSprite::_slot058() {}
void NUISystem::CSprite::_slot059() {}
void NUISystem::CSprite::_slot060() {}
void NUISystem::CSprite::_slot061() {}
void NUISystem::CSprite::_slot062() {}
void NUISystem::CSprite::_slot063() {}
void NUISystem::CSprite::_slot064() {}
void NUISystem::CSprite::_slot065() {}
void NUISystem::CSprite::_slot066() {}
void NUISystem::CSprite::_slot067() {}
void NUISystem::CSprite::_slot068() {}
void NUISystem::CSprite::_slot069() {}
void NUISystem::CSprite::_slot070() {}
void NUISystem::CSprite::_slot071() {}
void NUISystem::CSprite::_slot072() {}
void NUISystem::CSprite::_slot073() {}
void NUISystem::CSprite::_slot074() {}
void NUISystem::CSprite::_slot075() {}
void NUISystem::CSprite::_slot076() {}
void NUISystem::CSprite::_slot077() {}
void NUISystem::CSprite::_slot078() {}
void NUISystem::CSprite::_slot079() {}
void NUISystem::CSprite::_slot080() {}
void NUISystem::CSprite::_slot081() {}
void NUISystem::CSprite::_slot082() {}
void NUISystem::CSprite::_slot083() {}
void NUISystem::CSprite::_slot084() {}
void NUISystem::CSprite::_slot085() {}
void NUISystem::CSprite::_slot086() {}
void NUISystem::CSprite::_slot087() {}
void NUISystem::CSprite::_slot088() {}
void NUISystem::CSprite::_slot089() {}
void NUISystem::CSprite::_slot090() {}
void NUISystem::CSprite::_slot091() {}
void NUISystem::CSprite::_slot092() {}
void NUISystem::CSprite::_slot093() {}
void NUISystem::CSprite::_slot094() {}
void NUISystem::CSprite::_slot095() {}
void NUISystem::CSprite::_slot096() {}
void NUISystem::CSprite::_slot097() {}
void NUISystem::CSprite::_slot098() {}
void NUISystem::CSprite::_slot099() {}
void NUISystem::CSprite::_slot100() {}
void NUISystem::CSprite::_slot101() {}
void NUISystem::CSprite::_slot102() {}
void NUISystem::CSprite::_slot103() {}
void NUISystem::CSprite::_slot104() {}
void NUISystem::CSprite::_slot105() {}
void NUISystem::CSprite::_slot106() {}
void NUISystem::CSprite::_slot107() {}
static int g_destroyed;
void NUISystem::CDefPointeeBase::_slot00() {}
void NUISystem::CDefPointeeBase::DestroyDef() const { ++g_destroyed; }
static NUISystem::CUIDef* g_def;
CDefPointer<const NUISystem::CUIDef> NUISystem::CSprite::LoadAndReturnDef() const
{
    CDefPointer<const NUISystem::CUIDef> p(*(const CDefPointer<const NUISystem::CUIDef>*)&g_def);
    return p;
}
int main()
{
    static NUISystem::CUIStateDef states[2];
    states[0].GraphicIndex = 42; states[1].GraphicIndex = 7;
    static double defmem[0x60 / 8];
    NUISystem::CUIDef* def = new (defmem) NUISystem::CUIDef;
    def->RefCount = 1; def->States_M_start = states; def->States_M_finish = states + 2;
    g_def = def;
    NUISystem::CSprite sprite;
    bool ok = sprite.GetDefaultGraphicIndex() == 42 && def->RefCount == 1 && g_destroyed == 0;
    def->RefCount = 0;
    ok = ok && sprite.GetDefaultGraphicIndex() == 42 && def->RefCount == 0 && g_destroyed == 1;
    std::printf(ok ? "0041ab00_TEST PASS\n" : "FAIL\n");
    return 0;
}