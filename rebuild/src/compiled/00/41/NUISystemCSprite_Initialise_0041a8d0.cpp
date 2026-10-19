// NUISystem::CSprite::Initialise() -- initialise component and fetch default graphic-frame dimensions into inherited size
// Leaf: Initialise@CSpriteInitialiseView
// Retail0041A8D0,176 bytes through ret0041A97F. Actual RTTI0122F5D4 slot43 isCSprite.
// Shared CSprite donor inherited Size+dc->retailcc and GraphicIndex+1a8->178;
// native three8-byte handle/angle/dimension groups follow changing-state base15c.
// Shared FrontEndManager/quarantinedGraphicDataBank retained; views expose missing
// inherited nativefields and graphics-bank width21/height22 calls. Base0052C730,
// singleton0041E5F2, each owning graphics-bank temporary hasfastcall Data deletion callback.
#include <new>
#include "engine/CSprite.h"
#include "engine/CFrontEndManager.h"
#include "engine/_quarantine/CGraphicDataBank.h"
struct CGraphicDataBankCalls {
    virtual ~CGraphicDataBankCalls() {}
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual unsigned long GetGraphicFrameWidth(unsigned long,unsigned long) const;
    virtual unsigned long GetGraphicFrameHeight(unsigned long,unsigned long) const;
};
struct CSpriteGraphicsInfo {
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CSpriteGraphicsPointer {
    CGraphicDataBankCalls* Data;CSpriteGraphicsInfo* Info;
    CSpriteGraphicsPointer():Data(0),Info(0) {}
    CSpriteGraphicsPointer(const CSpriteGraphicsPointer& p):Data(p.Data),Info(p.Info) {if(Info) ++Info->RefCount;}
    ~CSpriteGraphicsPointer() {if(Info && --Info->RefCount==0) {Info->DeleteFunc(Info->Data);::operator delete(Info);}}
    CGraphicDataBankCalls* operator->() const {return Data;}
};
struct CFrontEndManagerGraphicsView {
    unsigned char Base[0x10];
    CSpriteGraphicsPointer PGraphicsBank;
    static CFrontEndManagerGraphicsView* __fastcall GetInstance(); //0041E5F2
    CSpriteGraphicsPointer GetGraphicsBank() const {return PGraphicsBank;}
};
namespace NUISystem {
struct CChangingStateComponentInitialiseCalls {
    void* VTable;
    unsigned char BeforeSize[0xcc-4];
    float SizeX,SizeY;
    unsigned char BeforeGraphicIndex[0x178-0xd4];
    unsigned long GraphicIndex;
    void Initialise(); //0052C730
};
struct CSpriteInitialiseView : CChangingStateComponentInitialiseCalls {void Initialise();};
void CSpriteInitialiseView::Initialise() {
    CChangingStateComponentInitialiseCalls::Initialise();
    SizeX=(float)CFrontEndManagerGraphicsView::GetInstance()->GetGraphicsBank()->GetGraphicFrameWidth(GraphicIndex,0);
    SizeY=(float)CFrontEndManagerGraphicsView::GetInstance()->GetGraphicsBank()->GetGraphicFrameHeight(GraphicIndex,0);
}
}