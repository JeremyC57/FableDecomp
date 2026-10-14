#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawBase::CDrawBase(CPlayerGui*, std::vector<CDrawBase*>*) -- cache the GUI's manager/world/component/bank pointers, clear the drawing flag and the primitive-handle vector, then register this drawer in the GUI's draw list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064CC65, length 78 bytes.
// Callees: vector<CDrawBase*>::push_back(CDrawBase* const&) @ 0x00656806 (on the list argument; manifest "CDrawBase_PushBackValue"). Vtable 0x0125A52C
// (NPlayerGui::CDrawBase; not in rtti-vtables.tsv, the 10 landed forwarding constructors call this body). Frame-pointer size TU
// (`optimize("y",off)` + `("s",on)`). PDB CDrawBase (44 bytes): PlayerGui +4, PlayerManager +8, World +0xc, Component +0x10, Bank +0x14,
// Drawing +0x18, PrimitiveHandles +0x1c (retail VC6 vector: three words). Shape: PlayerGui is stored before the vtable (hoisted next to its
// load), the four cached pointers come from the GUI record (+8, +4, +0x10, +0x14), the zero stores share eax, and push_back's by-reference
// temporary (`this`) is homed in the dead first argument slot (`mov [ebp+8],esi ; lea eax,[ebp+8]`).
struct CPlayerManager;
struct CWorld;
struct CMainGameComponent;
struct CGraphicDataBank;

struct CPlayerGui {
    unsigned char       _pad00[4];
    CWorld*             m_pWorld;                  // +0x04
    CPlayerManager*     m_pPlayerManager;          // +0x08
    unsigned char       _pad0c[4];
    CMainGameComponent* m_pComponent;              // +0x10
    CGraphicDataBank*   m_pBank;                   // +0x14
};

namespace NPlayerGui {

struct CDrawBase;

struct CRetailDrawList {                           // VC6 Dinkumware vector<CDrawBase*>
    CDrawBase** m_pFirst;
    CDrawBase** m_pLast;
    CDrawBase** m_pEnd;
    void push_back(CDrawBase* const& value);       // 0x00656806
};

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
};

struct CDrawBase {                                 // vtable 0x0125A52C
    virtual void v00();
    CPlayerGui*         PlayerGui;                 // +0x04
    CPlayerManager*     PlayerManager;             // +0x08
    CWorld*             World;                     // +0x0c
    CMainGameComponent* Component;                 // +0x10
    CGraphicDataBank*   Bank;                      // +0x14
    bool                Drawing;                   // +0x18
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, CRetailDrawList* drawList);
};

CDrawBase::CDrawBase(CPlayerGui* gui, CRetailDrawList* drawList)
    : PlayerGui(gui), PlayerManager(gui->m_pPlayerManager), World(gui->m_pWorld), Component(gui->m_pComponent), Bank(gui->m_pBank),
      Drawing(false)
{
    PrimitiveHandles.m_pFirst = 0;
    PrimitiveHandles.m_pLast = 0;
    PrimitiveHandles.m_pEnd = 0;
    drawList->push_back(this);
}

}
