#ifndef FABLE_ENGINE_CTCQUICKACCESSMENU_H
#define FABLE_ENGINE_CTCQUICKACCESSMENU_H
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE
#include "engine/ETCInterfaceType.h"
#endif
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace NUISystem { class CList;class CComponent; }
// Original PDB fields, native vector12; ctor confirms Active+c, Components+10, Quitting+1c.
class CTCQuickAccessMenu : public CTCBase {
public: CTCQuickAccessMenu(CThing&);
public: virtual ~CTCQuickAccessMenu();
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual void OnCreate();
public: virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public: virtual void GlobalUpdate(bool);
#ifdef FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE
    // Retail opt-in (chunk 890, row 0x0056AD40 CTCBoastUI::Activate): FableWin.pdb ?StatGetInterfaceType@CTCQuickAccessMenu@@SI?AW4ETCInterfaceType@@XZ
    // (0x01A93CF0; inline in retail: Activate looks up TC 246 = TCI_QUICK_ACCESS_MENU, `mov dword ptr [esp+30h],0F6h` at 0x0056AD93) and
    // ?Deactivate@CTCQuickAccessMenu@@QAEXXZ (Ego_r.pdb 0x0066A140, FableWin.pdb 0x02119DD0; out of line in retail at 0x00625920, `call`
    // at 0x0056ADB6 -- the empty 0x00625910 before it is OnCreate, Ego_r's order). Needs engine/ETCInterfaceType.h. No layout change.
public: static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_QUICK_ACCESS_MENU; }
public: void Deactivate();                                               // 0x00625920
#endif
private: void ConstructItemCategoryList(CWideString &,NUISystem::CList *,std::vector<std::pair<long,long>,std::allocator<std::pair<long,long> > > &,NUISystem::CComponent *,bool);
private: void ContructItemLists(std::vector<std::pair<long,long>,std::allocator<std::pair<long,long> > > &,NUISystem::CList *,NUISystem::CComponent *,CCountedPointer<NUISystem::CComponent> &);
private: void ContructItemLists(std::map<long,long,std::less<long>,std::allocator<std::pair<long const ,long> > > &,NUISystem::CList *,NUISystem::CComponent *,CCountedPointer<NUISystem::CComponent> &);
private: bool Active;
private: std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > Components;
private: bool Quitting;
private: double LastRenderTime;
private: bool FirstRenderFrame;
};
FABLE_STATIC_ASSERT(sizeof(CTCQuickAccessMenu)==0x30);
#endif
