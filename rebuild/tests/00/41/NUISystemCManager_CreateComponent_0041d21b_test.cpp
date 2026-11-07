// Self-contained C++03 fixture; complete selected target body pasted unchanged.
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
static std::vector<std::string> Events;
class CCharString {public:std::string Text;CCharString(){}CCharString(const std::string&s):Text(s){}};
class CDefString {public:std::string Text;CCharString GetString()const{Events.push_back("string");return CCharString(Text);}};
namespace NUISystem {enum EType {
    UI_TYPE_SPRITE=0,
    UI_TYPE_MORPHING_SPRITE=1,
    UI_TYPE_TABLE=2,
    UI_TYPE_MESH=3,
    UI_TYPE_COMPOSITE=4,
    UI_TYPE_CHANGING_STATE_COMPOSITE=5,
    UI_TYPE_TEXT=6,
    UI_TYPE_MENU_ENTRY=7,
    UI_TYPE_LIST=8,
    UI_TYPE_VIEWPORT=9,
    UI_TYPE_FRONTEND_SCREEN=10,
    UI_TYPE_FRONTEND_BUTTON=11,
    UI_TYPE_FRONTEND_LIST=12,
    UI_TYPE_SCROLLING_VIEWPORT=13,
    UI_TYPE_LIST_ARROW=14,
    UI_TYPE_SLIDER=15,
    UI_TYPE_TEXT_SLIDER=16,
    UI_TYPE_MOVIE=17,
    UI_TYPE_SWAPPING_STATE_COMPOSITE=18,
    UI_TYPE_SCROLLING_COMPOSITE=19,
    UI_TYPE_TEXT_CONTAINER=20,
    UI_TYPE_ZOOMING_COMPOSITE=21,
    UI_TYPE_COMPONENT_CONTAINER=22,
    UI_TYPE_SPELL_CONTAINER=23,
    UI_TYPE_SPELL_CONTAINER_LIST=24,
    UI_TYPE_YESNO=25,
    UI_TYPE_OK=26,
    UI_TYPE_PARTICLE_EFFECT=27,
    UI_TYPE_CONTROLLERDISCONNECT=28,
    UI_TYPE_DIRTYDISC=29,
    UI_TYPE_ICON_TEXT=30,
    UI_TYPE_DYNAMIC_LIST=31,
    UI_TYPE_MOUSE_POINTER=32,
    UI_TYPE_HOVERABLE=33,
    UI_TYPE_CLICKABLE=34,
    UI_TYPE_DRAGGABLE=35,
    UI_TYPE_DRAGGABLE_INTO=36,
    UI_TYPE_EDIT_BOX=37,
    UI_TYPE_NAVIGATION_BUTTON=38,
    UI_TYPE_KEY_REDEFINER=39,
    UI_TYPE_REDEFINER_LIST=40,
    UI_TYPE_SCROLLBAR=41,
    UI_TYPE_SCROLLBAR_OUTSIDE=42,
    UI_TYPE_SCROLLABLE_LIST=43
};class CUIDef {public:EType Type;mutable long RefCount;mutable unsigned Deleted;void AddRef()const{++RefCount;}void Release()const{assert(RefCount>0);if(--RefCount==0){++Deleted;Events.push_back("delete_def");}}};}
template<class T>class CDefPointer {public:T*Object;CDefPointer():Object(0){}~CDefPointer(){if(Object)Object->Release();}T*operator->()const{return Object;}void Reset(T*p){assert(!Object);Object=p;Object->AddRef();}};
class CDefinitionManager {public:mutable NUISystem::CUIDef Def;mutable unsigned DefReads,NameReads;long ExpectedDefinition;std::string Name;
 CDefinitionManager():DefReads(0),NameReads(0),ExpectedDefinition(0){Def.RefCount=1;Def.Deleted=0;}
 template<class T>bool GetDef(long index,CDefPointer<const T>*out)const{assert(index==ExpectedDefinition);++DefReads;Events.push_back("def:"+Name);out->Reset(&Def);return true;}
 CDefString GetDefNameFromGlobalIndex(long index)const{assert(index==ExpectedDefinition);++NameReads;Events.push_back("name:"+Name);CDefString result;result.Text=Name;return result;}
};
class CGameDefinitionManager:public CDefinitionManager {public:static CGameDefinitionManager*Instance;static CGameDefinitionManager&Get(){Events.push_back("global");assert(Instance);return *Instance;}};
CGameDefinitionManager*CGameDefinitionManager::Instance=0;
class CIEngine {public:int Marker;};
class CDisplayEngine {public:CIEngine*Engine3D;CIEngine&Get3DEngine(){Events.push_back("engine");return *Engine3D;}};
class CMainGameComponent {public:static CMainGameComponent*PMainGameComponent;CDisplayEngine*PDisplayEngine;static CMainGameComponent*Get(){Events.push_back("main");return PMainGameComponent;}CDisplayEngine&GetDisplayEngine(){Events.push_back("display");return *PDisplayEngine;}};
CMainGameComponent*CMainGameComponent::PMainGameComponent=0;
static CIEngine*ExpectedEngine=0;
namespace NUISystem {
class CComponent {public:long Definition;int ConstructedType;std::string Name;unsigned NameWrites;CComponent(long n,int type=4):Definition(n),ConstructedType(type),NameWrites(0){Events.push_back("construct");}virtual ~CComponent(){}virtual void SetName(const CCharString&n){++NameWrites;Name=n.Text;Events.push_back("setname");}};
class CManager {public:CDefinitionManager*m_pDefinitionManager;CComponent*CreateComponent(long);};
class CSprite:public CComponent {public:CSprite(long n):CComponent(n,0){}};
class CMorphingSprite:public CComponent {public:CMorphingSprite(long n):CComponent(n,1){}};
class CTable:public CComponent {public:CTable(long n):CComponent(n,2){}};
class CMesh:public CComponent {public:CMesh(long n):CComponent(n,3){}};
class CChangingStateComponent:public CComponent {public:CChangingStateComponent(long n):CComponent(n,5){}};
class CText:public CComponent {public:CText(long n):CComponent(n,6){}};
class CMenuEntry:public CComponent {public:CMenuEntry(long n):CComponent(n,7){}};
class CList:public CComponent {public:CList(long n):CComponent(n,8){}};
class CViewport:public CComponent {public:CViewport(long n):CComponent(n,9){}};
class CFrontEndScreen:public CComponent {public:CFrontEndScreen(long n):CComponent(n,10){}};
class CFrontEndButton:public CComponent {public:CFrontEndButton(long n):CComponent(n,11){}};
class CFrontEndList:public CComponent {public:CFrontEndList(long n):CComponent(n,12){}};
class CScrollingViewport:public CComponent {public:CScrollingViewport(long n):CComponent(n,13){}};
class CListArrow:public CComponent {public:CListArrow(long n):CComponent(n,14){}};
class CSlider:public CComponent {public:CSlider(long n):CComponent(n,15){}};
class CTextSlider:public CComponent {public:CTextSlider(long n):CComponent(n,16){}};
class CMovie:public CComponent {public:CMovie(CIEngine&engine,long n):CComponent(n,17){assert(&engine==ExpectedEngine);}};
class CSwappingStateComponent:public CComponent {public:CSwappingStateComponent(long n):CComponent(n,18){}};
class CScrollingComponent:public CComponent {public:CScrollingComponent(long n):CComponent(n,19){}};
class CTextContainer:public CComponent {public:CTextContainer(long n):CComponent(n,20){}};
class CZoomingComponent:public CComponent {public:CZoomingComponent(long n):CComponent(n,21){}};
class CComponentContainer:public CComponent {public:CComponentContainer(long n):CComponent(n,22){}};
class CSpellContainer:public CComponent {public:CSpellContainer(long n):CComponent(n,23){}};
class CSpellContainerList:public CComponent {public:CSpellContainerList(long n):CComponent(n,24){}};
class CYesNo:public CComponent {public:CYesNo(long n):CComponent(n,25){}};
class COk:public CComponent {public:COk(long n):CComponent(n,26){}};
class CParticleEffect:public CComponent {public:CParticleEffect(long n):CComponent(n,27){}};
class CControllerDisconnect:public CComponent {public:CControllerDisconnect(long n):CComponent(n,28){}};
class CIconText:public CComponent {public:CIconText(long n):CComponent(n,30){}};
class CDynamicList:public CComponent {public:CDynamicList(long n):CComponent(n,31){}};
class CMouseCursor:public CComponent {public:CMouseCursor(long n):CComponent(n,32){}};
class CHoverable:public CComponent {public:CHoverable(long n):CComponent(n,33){}};
class CClickable:public CComponent {public:CClickable(long n):CComponent(n,34){}};
class CDraggable:public CComponent {public:CDraggable(long n):CComponent(n,35){}};
class CDraggableInto:public CComponent {public:CDraggableInto(long n):CComponent(n,36){}};
class CEditBox:public CComponent {public:CEditBox(long n):CComponent(n,37){}};
class CNavButton:public CComponent {public:CNavButton(long n):CComponent(n,38){}};
class CKeyRedefiner:public CComponent {public:CKeyRedefiner(long n):CComponent(n,39){}};
class CRedefinerList:public CComponent {public:CRedefinerList(long n):CComponent(n,40){}};
class CScrollBar:public CComponent {public:CScrollBar(long n):CComponent(n,41){}};
class CScrollBarOutside:public CComponent {public:CScrollBarOutside(long n):CComponent(n,42){}};
class CScrollableList:public CComponent {public:CScrollableList(long n):CComponent(n,43){}};
}

NUISystem::CComponent* NUISystem::CManager::CreateComponent(long definition)
{
    CDefPointer<const CUIDef> pdef;
    CComponent* presult=0;
    if(m_pDefinitionManager)m_pDefinitionManager->GetDef(definition,&pdef);
    else CGameDefinitionManager::Get().GetDef(definition,&pdef);
    switch(pdef->Type) {
    case UI_TYPE_SPRITE:presult=new CSprite(definition);break;
    case UI_TYPE_MOUSE_POINTER:presult=new CMouseCursor(definition);break;
    case UI_TYPE_HOVERABLE:presult=new CHoverable(definition);break;
    case UI_TYPE_CLICKABLE:presult=new CClickable(definition);break;
    case UI_TYPE_DRAGGABLE:presult=new CDraggable(definition);break;
    case UI_TYPE_SCROLLBAR:presult=new CScrollBar(definition);break;
    case UI_TYPE_SCROLLBAR_OUTSIDE:presult=new CScrollBarOutside(definition);break;
    case UI_TYPE_DRAGGABLE_INTO:presult=new CDraggableInto(definition);break;
    case UI_TYPE_NAVIGATION_BUTTON:presult=new CNavButton(definition);break;
    case UI_TYPE_KEY_REDEFINER:presult=new CKeyRedefiner(definition);break;
    case UI_TYPE_REDEFINER_LIST:presult=new CRedefinerList(definition);break;
    case UI_TYPE_SCROLLABLE_LIST:presult=new CScrollableList(definition);break;
    case UI_TYPE_MORPHING_SPRITE:presult=new CMorphingSprite(definition);break;
    case UI_TYPE_COMPOSITE:presult=new CComponent(definition);break;
    case UI_TYPE_TABLE:presult=new CTable(definition);break;
    case UI_TYPE_MESH:presult=new CMesh(definition);break;
    case UI_TYPE_TEXT:presult=new CText(definition);break;
    case UI_TYPE_MENU_ENTRY:presult=new CMenuEntry(definition);break;
    case UI_TYPE_LIST:presult=new CList(definition);break;
    case UI_TYPE_CHANGING_STATE_COMPOSITE:presult=new CChangingStateComponent(definition);break;
    case UI_TYPE_VIEWPORT:presult=new CViewport(definition);break;
    case UI_TYPE_FRONTEND_SCREEN:presult=new CFrontEndScreen(definition);break;
    case UI_TYPE_FRONTEND_BUTTON:presult=new CFrontEndButton(definition);break;
    case UI_TYPE_FRONTEND_LIST:presult=new CFrontEndList(definition);break;
    case UI_TYPE_SCROLLING_VIEWPORT:presult=new CScrollingViewport(definition);break;
    case UI_TYPE_LIST_ARROW:presult=new CListArrow(definition);break;
    case UI_TYPE_SLIDER:presult=new CSlider(definition);break;
    case UI_TYPE_TEXT_SLIDER:presult=new CTextSlider(definition);break;
    case UI_TYPE_MOVIE:presult=new CMovie(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(),definition);break;
    case UI_TYPE_SWAPPING_STATE_COMPOSITE:presult=new CSwappingStateComponent(definition);break;
    case UI_TYPE_SCROLLING_COMPOSITE:presult=new CScrollingComponent(definition);break;
    case UI_TYPE_TEXT_CONTAINER:presult=new CTextContainer(definition);break;
    case UI_TYPE_ZOOMING_COMPOSITE:presult=new CZoomingComponent(definition);break;
    case UI_TYPE_COMPONENT_CONTAINER:presult=new CComponentContainer(definition);break;
    case UI_TYPE_SPELL_CONTAINER:presult=new CSpellContainer(definition);break;
    case UI_TYPE_SPELL_CONTAINER_LIST:presult=new CSpellContainerList(definition);break;
    case UI_TYPE_YESNO:presult=new CYesNo(definition);break;
    case UI_TYPE_PARTICLE_EFFECT:presult=new CParticleEffect(definition);break;
    case UI_TYPE_OK:presult=new COk(definition);break;
    case UI_TYPE_CONTROLLERDISCONNECT:presult=new CControllerDisconnect(definition);break;
    case UI_TYPE_ICON_TEXT:presult=new CIconText(definition);break;
    case UI_TYPE_DYNAMIC_LIST:presult=new CDynamicList(definition);break;
    case UI_TYPE_EDIT_BOX:presult=new CEditBox(definition);break;
    }
    if(m_pDefinitionManager){
        CCharString name=m_pDefinitionManager->GetDefNameFromGlobalIndex(definition).GetString();
        presult->SetName(name);
        return presult;
    }else{
        CCharString name=CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(definition).GetString();
        presult->SetName(name);
        return presult;
    }
}


int main(){unsigned cases=0;const long ids[]={1,17,4194303};const long refs[]={0,1,4};
 for(unsigned type=0;type<44;++type){if(type==29)continue;
  for(unsigned local=0;local<2;++local)for(unsigned ni=0;ni<3;++ni)for(unsigned ri=0;ri<3;++ri){
   Events.clear();CDefinitionManager own;CGameDefinitionManager global;CGameDefinitionManager::Instance=&global;
   own.Name="local_definition";global.Name="global_definition";own.ExpectedDefinition=global.ExpectedDefinition=ids[ni];own.Def.Type=global.Def.Type=NUISystem::EType(type);own.Def.RefCount=global.Def.RefCount=refs[ri];
   CIEngine engine;engine.Marker=67;ExpectedEngine=&engine;CDisplayEngine display;display.Engine3D=&engine;CMainGameComponent main;main.PDisplayEngine=&display;CMainGameComponent::PMainGameComponent=&main;
   NUISystem::CManager manager;manager.m_pDefinitionManager=local?&own:0;
   NUISystem::CComponent*result=manager.CreateComponent(ids[ni]);CDefinitionManager&used=local?own:global;CDefinitionManager&unused=local?static_cast<CDefinitionManager&>(global):own;
   assert(result&&result->Definition==ids[ni]&&result->ConstructedType==int(type)&&result->Name==used.Name&&result->NameWrites==1);
   assert(used.DefReads==1&&used.NameReads==1&&unused.DefReads==0&&unused.NameReads==0&&used.Def.RefCount==refs[ri]&&used.Def.Deleted==(refs[ri]==0?1u:0u)&&unused.Def.Deleted==0);
   std::vector<std::string>expected;if(!local)expected.push_back("global");expected.push_back("def:"+used.Name);if(type==17){expected.push_back("main");expected.push_back("display");expected.push_back("engine");}expected.push_back("construct");if(!local)expected.push_back("global");expected.push_back("name:"+used.Name);expected.push_back("string");expected.push_back("setname");if(refs[ri]==0)expected.push_back("delete_def");assert(Events==expected);delete result;++cases;
  }
 }
 assert(cases==774);std::puts("0041d21b_TEST PASS");return 0;
}