#ifndef FABLE_ENGINE_CTCQUICKACCESSMENU_H
#define FABLE_ENGINE_CTCQUICKACCESSMENU_H
#include "engine/CTCBase.h"
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
