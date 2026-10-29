// NUISystem::CViewport::Initialise() -- initialise component and fetch default graphic-frame dimensions into inherited size
// Leaf: Initialise@CViewportInitialiseView
// Retail0054E640,176 bytes through ret0054E6EF. Propagated CSprite owner is wrong:
// actual RTTI01249A5C slot43 identifies CViewport. PDB CViewport Handle18c/FakeChildren194
// (vector16)/GraphicIndex1a4 become retail15c/164(vector12)/170. Inherited Size donor+dc->cc.
// No CViewport header exists. Shared FrontEndManager and quarantined GraphicDataBank retained;
// method/data views model inherited retail fields absent from flattened shared headers.
// Base0052C730, singleton0041E5F2, graphic-bank virtual width21/height22 frame0.
// Info DeleteFunc uses ECX data (fastcall), then operator delete frees the info record.
#include <new>
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
namespace NUISystem { class CManager { public:
    unsigned char Base[0x10];
    CSpriteGraphicsPointer PGraphicsBank;
    static CManager* __fastcall GetInstance(); //0041E5F2
    CSpriteGraphicsPointer GetGraphicsBank() const {return PGraphicsBank;}
}; }
namespace NUISystem {
struct CChangingStateComponentInitialiseCalls {
    void* VTable;
    unsigned char BeforeSize[0xcc-4];
    float SizeX,SizeY;
    unsigned char BeforeGraphicIndex[0x170-0xd4];
    unsigned long GraphicIndex;
    void Initialise(); //0052C730
};
struct CViewportInitialiseView : CChangingStateComponentInitialiseCalls {void Initialise();};
void CViewportInitialiseView::Initialise() {
    CChangingStateComponentInitialiseCalls::Initialise();
    SizeX=(float)CManager::GetInstance()->GetGraphicsBank()->GetGraphicFrameWidth(GraphicIndex,0);
    SizeY=(float)CManager::GetInstance()->GetGraphicsBank()->GetGraphicFrameHeight(GraphicIndex,0);
}
}