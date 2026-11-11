// 005e4be0 full GetQuestDescription behavior fixture.
// Exact production function body; controlled dependencies exercise caller behavior.
// UI owns fixture children; pointer wrappers here do not test engine ownership.
#include <string>
#include <vector>
#include <list>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define FABLE_FASTCALL
static void Check(bool v,int line) { if(!v) { std::fprintf(stderr,"FAIL line %d\n",line);std::exit(1); } }
#define CHECK(x) Check(!!(x),__LINE__)
class CCharString {
public: std::string Value; CCharString(const char* p=""):Value(p){}
 bool operator==(const CCharString& x)const{return Value==x.Value;}
};
class CWideString {
public: std::wstring Value;
 static const CWideString NullString;
 CWideString(const wchar_t* p=L""):Value(p){}
 CWideString(const std::wstring& p):Value(p){}
 bool operator!=(const CWideString& x)const{return Value!=x.Value;}
 CWideString& operator+=(const CWideString& x){Value+=x.Value;return *this;}
 void Sprintf(const wchar_t* format,long n){CHECK(std::wstring(format)==L"%d");char b[80];std::sprintf(b,"%ld",n);Value.erase();for(const char* p=b;*p;++p)Value+=(wchar_t)*p;}
};
const CWideString CWideString::NullString;
static CWideString operator+(const CWideString& a,const CWideString& b){return CWideString(a.Value+b.Value);}
template<class T>class CCountedPointer { public:T* Data; CCountedPointer():Data(0){} void Reset(T* p){Data=p;} T* Get()const{return Data;}T* operator->()const{return Data;} };
template<class T>class CDefPointer { public:T* Data; CDefPointer():Data(0){}T* Get()const{return Data;}T* operator->()const{return Data;}bool IsNull()const{return !Data;} };
namespace NUISystem {
class CUIState { public: struct ColourValue {unsigned char R,G,B,A;ColourValue():R(255),G(128),B(64),A(255){}}Colour; };
class CComponent {
public: std::string Name;std::vector<CComponent*> Children; CUIState States[1];
 CComponent(const char* name="container"):Name(name){}virtual ~CComponent(){}
 virtual CComponent* FindChild(const CCharString& n){for(unsigned i=0;i<Children.size();++i)if(Children[i]->Name==n.Value)return Children[i];return 0;}
 virtual void AddChild(const CCountedPointer<CComponent>& p){CHECK(p.Get()!=0);Children.push_back(p.Get());}
 virtual CUIState* GetStates(){return States;}
};
class CText:public CComponent {
public:CWideString Text;long Id;int Kind;bool First,Second;
 CText(const char* n):CComponent(n),Id(0),Kind(0),First(false),Second(false){}
 void SetValue(const CCharString& s,bool a,bool b){Text.Value.erase();for(unsigned i=0;i<s.Value.size();++i)Text.Value+=(wchar_t)s.Value[i];Kind=1;First=a;Second=b;}
 void SetValue(const CWideString& s,bool a,bool b){Text=s;Kind=2;First=a;Second=b;}
 void SetValue(long id,const CWideString& prefix,bool a){CHECK(prefix.Value.empty());Id=id;Kind=3;First=a;}
};
class CSprite:public CComponent {public:long Graphic;CSprite():CComponent("UI_CORE_COUPLE"),Graphic(-1){}void SetGraphicIndex(long n){Graphic=n;}};
class CUIMiscThingsDef {public:CWideString ColonSeparator,SpaceSeparator,OpenBracket,CloseBracket,Plus,Minus;CUIMiscThingsDef():ColonSeparator(L":"),SpaceSeparator(L" "),OpenBracket(L"("),CloseBracket(L")"),Plus(L"+"),Minus(L"-"){} };
class CManager {
public:std::vector<CComponent*> Owned;bool OmitSprite;std::vector<std::string> Created;static CManager* Instance;
 CManager():OmitSprite(false){Instance=this;}~CManager(){for(unsigned i=0;i<Owned.size();++i)delete Owned[i];Instance=0;}
 static CManager* GetInstance(){return Instance;}
 CComponent* CreateComponent(const CCharString& n,bool b){CHECK(!b);Created.push_back(n.Value);CComponent* p=0;
 if(n.Value=="PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION")p=new CText("text");
 else {p=new CComponent(n.Value.c_str());CText* t=new CText("PC_QUEST_NAME");Owned.push_back(t);p->Children.push_back(t);
 if(n.Value=="PC_CORE_ICON_AND_NAME"&&!OmitSprite){CSprite* v=new CSprite;Owned.push_back(v);p->Children.push_back(v);}}
 Owned.push_back(p);return p;}
};
CManager* CManager::Instance=0;
}
namespace NGameText {class CDataBank {public:const CWideString GetTextBySymbol(const CCharString& s)const{
 const char* names[]={"TEXT_GUI_MENU_GUILD_REWARD","TEXT_GUI_GOLD","TEXT_GUI_MENU_RENOWN_REWARD","TEXT_GUI_MENU_EXTRA_GOLD","TEXT_GUI_MENU_EXTRA_RENOWN","TEXT_GUI_STATS_LABEL_EXPER","TEXT_GUI_MENU_FAILED","TEXT_GUI_MENU_SUCCEEDED"};
 const wchar_t* values[]={L"Reward",L"Gold",L"Renown",L"Extra gold",L"Extra renown",L"Experience",L"Failed",L"Succeeded"};
 for(unsigned i=0;i<8;++i)if(s.Value==names[i])return CWideString(values[i]);CHECK(false);return CWideString();}};}
class CMainGameComponent {public:NGameText::CDataBank Bank;static CMainGameComponent Instance;static CMainGameComponent* Get(){return &Instance;}const NGameText::CDataBank& GetTextBank()const{return Bank;}};
CMainGameComponent CMainGameComponent::Instance;
class CInventoryItemDef {public:struct GraphicValue {long Type,BankIndex;GraphicValue():Type(1),BankIndex(17){}}Graphic;long ItemDescription;CInventoryItemDef():ItemDescription(41){} };
class CThingObjectDef {public:const CInventoryItemDef* Item;CThingObjectDef():Item(0){}void GetSubDef(CDefPointer<const CInventoryItemDef>* out)const{out->Data=Item;} };
class CQuestCardDef {public:std::vector<long> RewardObjects;long QuestEpilogue;CQuestCardDef():QuestEpilogue(0){} };
class CGameDefinitionManager {
public:NUISystem::CUIMiscThingsDef Misc;std::map<long,const CThingObjectDef*> Objects;static CGameDefinitionManager Instance;
 static CGameDefinitionManager& Get(){return Instance;}
 long GetGlobalIndexFromClassIndex(const CCharString& n,long id){CHECK(n.Value=="UI_MISC_THINGS_DEF"&&id==1);return 11;}
 void GetDef(long id,CDefPointer<const NUISystem::CUIMiscThingsDef>* out){CHECK(id==11);out->Data=&Misc;}
 void GetDef(long id,CDefPointer<const CThingObjectDef>* out){out->Data=Objects[id];}
};CGameDefinitionManager CGameDefinitionManager::Instance;
class CQuestManager {public:class CBoast {
public:CCharString ScriptName;CWideString Text;bool Failed;long Cost,Reward;
 CBoast():ScriptName("quest"),Text(L"Boast"),Failed(false),Cost(3),Reward(7){}
 const CCharString& GetScriptName()const{return ScriptName;}const CWideString& GetText()const{return Text;}bool IsFailed()const{return Failed;}long GetCost()const{return Cost;}long GetReward()const{return Reward;}
};};
class CTCQuestCompletionUI {
public:long Completion,GoldReward,RenownReward,TargetGoldGained,TargetRenownGained,TargetExperienceGained;CDefPointer<const CQuestCardDef> PCardDef;CWideString FailureReason,QuestObjective,QuestSummary,SuccessSummary;std::list<CQuestManager::CBoast> Boasts;CCharString ScriptName;
 CTCQuestCompletionUI():Completion(0),GoldReward(10),RenownReward(4),TargetGoldGained(12),TargetRenownGained(5),TargetExperienceGained(20),ScriptName("quest"){}
 void GetQuestDescription(NUISystem::CComponent* pcontainer);
};

inline CWideString FABLE_FASTCALL GFIntToWideString(long n)
{
    CWideString temp;
    temp.Sprintf(L"%d", n);
    return temp;
}
void CTCQuestCompletionUI::GetQuestDescription(NUISystem::CComponent* pcontainer)
{
    const NGameText::CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    NUISystem::CManager* pmanager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> ptext;
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> pmiscthingsdef;
    long misc_thing_def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(misc_thing_def_index, &pmiscthingsdef);
    if (Completion == 3) {
        ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
        pcontainer->AddChild(ptext);
        NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
        ptext_real->SetValue("TEXT_GUI_MENU_DEATH_EXPLANATION", true, true);
    } else {
    if (Completion == 2 && FailureReason != L"") {
        ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
        pcontainer->AddChild(ptext);
        NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
        ptext_real->SetValue(FailureReason, true, true);
    }
    if (Completion == 0) {
        CCountedPointer<NUISystem::CComponent> picon_couple;
        if (PCardDef.Get()) {
            ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
            NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
            pcontainer->AddChild(ptext);
            if (QuestObjective != L"") ptext_real->SetValue(QuestObjective, true, true);
            else ptext_real->SetValue(QuestSummary, true, true);
            picon_couple.Reset(pmanager->CreateComponent("PC_GOLD_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_GUILD_REWARD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(GoldReward) + pmiscthingsdef->SpaceSeparator + rTextBank.GetTextBySymbol("TEXT_GUI_GOLD");
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_RENOWN_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN_REWARD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(RenownReward);
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
        }
    }
    if (Completion == 1) {
        NUISystem::CText* ptext_real;
        if (SuccessSummary != CWideString::NullString) {
            ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
            NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
            pcontainer->AddChild(ptext);
            ptext_real->SetValue(SuccessSummary, true, true);
        }
        CCountedPointer<NUISystem::CComponent> picon_couple;
        if (PCardDef.Get()) {
            for (std::vector<long>::const_iterator it = PCardDef->RewardObjects.begin(); it != PCardDef->RewardObjects.end(); ++it) {
                picon_couple.Reset(pmanager->CreateComponent("PC_CORE_ICON_AND_NAME", false));
                ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
                CDefPointer<const CThingObjectDef> pobject_def;
                CDefPointer<const CInventoryItemDef> pitem_def;
                CGameDefinitionManager::Get().GetDef(*it, &pobject_def);
                if (!pobject_def.IsNull()) {
                    pobject_def->GetSubDef(&pitem_def);
                    if (!pitem_def.IsNull()) {
                        NUISystem::CSprite* picon = (NUISystem::CSprite*)picon_couple->FindChild("UI_CORE_COUPLE");
                        if (picon && pitem_def->Graphic.Type == 1) picon->SetGraphicIndex(pitem_def->Graphic.BankIndex);
                        ptext_real->SetValue(pitem_def->ItemDescription, L"", true);
                        pcontainer->AddChild(picon_couple);
                    }
                }
            }
            picon_couple.Reset(pmanager->CreateComponent("PC_GOLD_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_GUILD_REWARD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(GoldReward) + pmiscthingsdef->SpaceSeparator + rTextBank.GetTextBySymbol("TEXT_GUI_GOLD");
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_GOLD_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            long extra_gold_to_display = TargetGoldGained - GoldReward;
            if (extra_gold_to_display < 0) extra_gold_to_display = 0;
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_EXTRA_GOLD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(extra_gold_to_display) + pmiscthingsdef->SpaceSeparator + rTextBank.GetTextBySymbol("TEXT_GUI_GOLD");
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_RENOWN_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN_REWARD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(RenownReward);
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_RENOWN_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            long extra_renown_to_display = TargetRenownGained - RenownReward;
            if (extra_renown_to_display < 0) extra_renown_to_display = 0;
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_EXTRA_RENOWN") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(extra_renown_to_display);
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_EXPERIENCE_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_EXPER") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(TargetExperienceGained);
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
        }
    }
    if (Completion == 1 || Completion == 0) {
        for (std::list<CQuestManager::CBoast>::iterator it = Boasts.begin(); it != Boasts.end(); ++it) {
            if (it->GetScriptName() == ScriptName) {
                ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
                pcontainer->AddChild(ptext);
                NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
                text = it->GetText() + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator;
                if (Completion == 1) {
                    if (it->IsFailed()) {
                        text += rTextBank.GetTextBySymbol("TEXT_GUI_MENU_FAILED") + pmiscthingsdef->SpaceSeparator + pmiscthingsdef->OpenBracket + pmiscthingsdef->Minus + GFIntToWideString(it->GetCost()) + pmiscthingsdef->CloseBracket;
                        ptext_real->SetValue(text, true, true);
                        ptext_real->GetStates()[0].Colour.B *= 0.0f;
                        ptext_real->GetStates()[0].Colour.G *= 0.0f;
                    } else {
                        text += rTextBank.GetTextBySymbol("TEXT_GUI_MENU_SUCCEEDED") + pmiscthingsdef->SpaceSeparator + pmiscthingsdef->OpenBracket + pmiscthingsdef->Plus + GFIntToWideString(it->GetReward()) + pmiscthingsdef->CloseBracket;
                        ptext_real->SetValue(text, true, true);
                        ptext_real->GetStates()[0].Colour.B *= 0.0f;
                    }
                } else {
                    text += pmiscthingsdef->OpenBracket + pmiscthingsdef->Plus + GFIntToWideString(it->GetReward()) + pmiscthingsdef->CloseBracket;
                    ptext_real->SetValue(text, true, true);
                    ptext_real->GetStates()[0].Colour.B *= 0.0f;
                }
            }
        }
    }
    if (Completion == 1 && PCardDef.Get() && PCardDef->QuestEpilogue) {
        ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
        NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
        pcontainer->AddChild(ptext);
        ptext_real->SetValue(PCardDef->QuestEpilogue, L"", true);
    }
    }
}


static NUISystem::CText* Text(NUISystem::CComponent& p,unsigned index){CHECK(index<p.Children.size());NUISystem::CComponent* x=p.Children[index];if(x->Name=="text")return static_cast<NUISystem::CText*>(x);NUISystem::CComponent* child=x->FindChild("PC_QUEST_NAME");CHECK(child!=0);return static_cast<NUISystem::CText*>(child);}
static void Value(NUISystem::CComponent& p,unsigned n,const wchar_t* s){NUISystem::CText* t=Text(p,n);CHECK(t->Kind==2);CHECK(t->Text.Value==s);CHECK(t->First&&t->Second);}
int main(){
 unsigned scenarios=0;
 // Death and failure suppress unrelated rewards/boasts; failure emptiness is significant.
 for(int mode=2;mode<=3;++mode)for(int supplied=0;supplied<2;++supplied){NUISystem::CManager m;NUISystem::CComponent c;CTCQuestCompletionUI q;CQuestCardDef card;card.QuestEpilogue=77;q.PCardDef.Data=&card;q.Completion=mode;q.FailureReason=supplied?L"Failure":L"";q.Boasts.push_back(CQuestManager::CBoast());q.GetQuestDescription(&c);
 if(mode==3){CHECK(c.Children.size()==1);NUISystem::CText* t=Text(c,0);CHECK(t->Kind==1&&t->Text.Value==L"TEXT_GUI_MENU_DEATH_EXPLANATION"&&t->First&&t->Second);}
 else if(supplied){CHECK(c.Children.size()==1);Value(c,0,L"Failure");}else CHECK(c.Children.empty());++scenarios;}
 // Preview requires a card for objective and numeric rewards, uses summary only for an empty objective.
 for(int cardPresent=0;cardPresent<2;++cardPresent)for(int objective=0;objective<2;++objective){NUISystem::CManager m;NUISystem::CComponent c;CTCQuestCompletionUI q;CQuestCardDef card;q.PCardDef.Data=cardPresent?&card:0;q.QuestObjective=objective?L"Objective":L"";q.QuestSummary=L"Summary";q.GetQuestDescription(&c);
 CHECK(c.Children.size()==(cardPresent?3u:0u));if(cardPresent){Value(c,0,objective?L"Objective":L"Summary");Value(c,1,L"Reward: 10 Gold");Value(c,2,L"Renown: 4");}++scenarios;}
 // Success summary independent of card; epilogue is an ID, after all reward lines.
 for(int cardPresent=0;cardPresent<2;++cardPresent)for(int summary=0;summary<2;++summary)for(int epilogue=0;epilogue<2;++epilogue){NUISystem::CManager m;NUISystem::CComponent c;CTCQuestCompletionUI q;CQuestCardDef card;q.Completion=1;q.PCardDef.Data=cardPresent?&card:0;q.SuccessSummary=summary?L"Success":L"";card.QuestEpilogue=epilogue?77:0;q.GetQuestDescription(&c);
 CHECK(c.Children.size()==unsigned(summary+(cardPresent?5+epilogue:0)));if(summary)Value(c,0,L"Success");if(cardPresent){unsigned n=summary;Value(c,n++,L"Reward: 10 Gold");Value(c,n++,L"Extra gold: 2 Gold");Value(c,n++,L"Renown: 4");Value(c,n++,L"Extra renown: 1");Value(c,n++,L"Experience: 20");if(epilogue){CHECK(Text(c,n)->Kind==3&&Text(c,n)->Id==77&&Text(c,n)->First);}}++scenarios;}
 // Exercise both clamps below, at, and above zero independently.
 for(int gold=-1;gold<=1;++gold)for(int renown=-1;renown<=1;++renown){NUISystem::CManager m;NUISystem::CComponent c;CTCQuestCompletionUI q;CQuestCardDef card;q.Completion=1;q.PCardDef.Data=&card;q.TargetGoldGained=q.GoldReward+gold;q.TargetRenownGained=q.RenownReward+renown;q.GetQuestDescription(&c);CHECK(c.Children.size()==5);Value(c,1,gold>0?L"Extra gold: 1 Gold":L"Extra gold: 0 Gold");Value(c,3,renown>0?L"Extra renown: 1":L"Extra renown: 0");++scenarios;}
 // Reward object lookup success, missing object/item/icon, and unsupported graphic type.
 for(int state=0;state<5;++state){NUISystem::CManager m;NUISystem::CComponent c;CTCQuestCompletionUI q;CQuestCardDef card;CThingObjectDef object;CInventoryItemDef item;q.Completion=1;q.PCardDef.Data=&card;card.RewardObjects.push_back(71);object.Item=state==1?0:&item;item.Graphic.Type=state==4?0:1;m.OmitSprite=state==3;CGameDefinitionManager::Get().Objects[71]=state==0?0:&object;q.GetQuestDescription(&c);CHECK(c.Children.size()==(state<2?5u:6u));if(state>=2){NUISystem::CText* t=Text(c,0);CHECK(t->Kind==3&&t->Id==41&&t->First);NUISystem::CSprite* icon=static_cast<NUISystem::CSprite*>(c.Children[0]->FindChild("UI_CORE_COUPLE"));if(state==3)CHECK(icon==0);else CHECK(icon->Graphic==(state==4?-1:17));}CGameDefinitionManager::Get().Objects.clear();++scenarios;}
 // Boasts use the script filter, preserve order, and tint only the proven channels.
 for(int mode=0;mode<=1;++mode)for(int failed=0;failed<2;++failed){NUISystem::CManager m;NUISystem::CComponent c;CTCQuestCompletionUI q;q.Completion=mode;CQuestManager::CBoast ignore;ignore.ScriptName="other";q.Boasts.push_back(ignore);CQuestManager::CBoast boast;boast.Failed=failed!=0;q.Boasts.push_back(boast);q.GetQuestDescription(&c);CHECK(c.Children.size()==1);Value(c,0,mode==0?L"Boast: (+7)":failed?L"Boast: Failed (-3)":L"Boast: Succeeded (+7)");NUISystem::CUIState& state=Text(c,0)->States[0];CHECK(state.Colour.R==255&&state.Colour.A==255&&state.Colour.B==0);CHECK(state.Colour.G==((mode==1&&failed)?0:128));++scenarios;}
 // Multiple valid/invalid rewards, success text, boasts, and epilogue remain in native order.
 {NUISystem::CManager m;NUISystem::CComponent c;CTCQuestCompletionUI q;CQuestCardDef card;CThingObjectDef object;CInventoryItemDef item;object.Item=&item;q.Completion=1;q.PCardDef.Data=&card;q.SuccessSummary=L"Success";card.QuestEpilogue=78;card.RewardObjects.push_back(71);card.RewardObjects.push_back(72);card.RewardObjects.push_back(71);CGameDefinitionManager::Get().Objects[71]=&object;q.Boasts.push_back(CQuestManager::CBoast());q.GetQuestDescription(&c);CHECK(c.Children.size()==10);Value(c,0,L"Success");CHECK(Text(c,1)->Id==41&&Text(c,2)->Id==41);Value(c,3,L"Reward: 10 Gold");Value(c,8,L"Boast: Succeeded (+7)");CHECK(Text(c,9)->Id==78);CGameDefinitionManager::Get().Objects.clear();++scenarios;}
 CHECK(scenarios==35);std::puts("005e4be0_TEST PASS");return 0;
}