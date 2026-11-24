// Exercise the submitted Draw body with deterministic game and rendering dependencies.
#include <cassert>
#include <cstdio>
#include <cstring>
struct C2DVector {
    float X,Y;
    C2DVector(float x=0,float y=0):X(x),Y(y) {}
    C2DVector operator+(const C2DVector& b)const {return C2DVector(X+b.X,Y+b.Y);}
    C2DVector& operator+=(const C2DVector& b){X+=b.X;Y+=b.Y;return *this;}
};
struct C2DExtentsF {float w,h;C2DExtentsF(float x,float y):w(x),h(y){}};
struct C2DBoxF {C2DVector p;float w,h;C2DBoxF(C2DVector v,C2DExtentsF e):p(v),w(e.w),h(e.h){}};
static C2DVector GFToScreen(const C2DVector& v){return v;}
static C2DBoxF GFToScreen(const C2DBoxF& v){return v;}
struct C3DVector {float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
struct CRightHandedSet {CRightHandedSet(C3DVector a,C3DVector b){assert(a.Z==1 && b.Y==1);}};
struct CRGBColour {
 unsigned char R,G,B,A;
 CRGBColour(unsigned char r=0,unsigned char g=0,unsigned char b=0,unsigned char a=255):R(r),G(g),B(b),A(a){}
};
enum {TCI_HERO_STATS,TCI_BERSERK,TCI_HASTE,TCI_INVENTORY};
enum {ENGINE_2D_LAYER_1=1,ENGINE_2D_LAYER_2,ENGINE_2D_LAYER_5=5,ENGINE_2D_LAYER_6,ENGINE_2D_LAYER_7,
 ENGINE_PRIMITIVE_BLEND_ALPHA=2,ENGINE_INTERPOLATION_SET_DEFAULT=0};
struct CFontBank {};
struct CTCHeroStats {};
struct CTCHaste {float speed;float GetActionSpeedMultiplier(){return speed;}};
struct CTCInventory {long count;long GetNumberOfItemsOfTypeInInventory(long id)const{assert(id==9);return count;}};
struct CThingPlayerCreature {
 bool alive,berserk,haste,inventory;float health,max;
 struct Base {float Health;} def;
 CTCHaste accelerated;CTCInventory items;CTCHeroStats stats;
 float GetHealth()const{return health;}float GetMaxHealth()const{return max;}
 float GetHealthPercentage()const{return health/max;}const Base& PeekBaseDef()const{return def;}
 bool HasTC(int id)const{return id==TCI_BERSERK?berserk:id==TCI_HASTE?haste:false;}
 bool PeekTC(int id,const CTCHeroStats** p)const{assert(id==TCI_HERO_STATS);*p=&stats;return true;}
 bool PeekTC(int id,const CTCHaste** p)const{assert(id==TCI_HASTE);*p=haste?&accelerated:0;return haste;}
 bool PeekTC(int id,const CTCInventory** p)const{assert(id==TCI_INVENTORY);*p=inventory?&items:0;return inventory;}
 bool PeekTC(const CTCHeroStats** p)const{return PeekTC(TCI_HERO_STATS,p);}
 bool PeekTC(const CTCHaste** p)const{return PeekTC(TCI_HASTE,p);}
 bool PeekTC(const CTCInventory** p)const{return PeekTC(TCI_INVENTORY,p);}
};
static bool IS_THING_ALIVE(const CThingPlayerCreature* p){return p && p->alive;}
struct Player {CThingPlayerCreature* hero;const CThingPlayerCreature* GetPControlledCreature()const{return hero;}};
struct CPlayerGuiDef {
 C2DVector HealthBarPos,HealthBarHealthOffset,HealthBarHealthIncreaseEffectOffset,HealthBarResurrectionOffset,HealthBarMiddleOffset,HealthBarHealthInnerOffset;
 int HealthBarHealthGraphic,HealthBarLeftGraphic,HealthBarMiddleGraphic,HealthBarRightGraphic,HealthBarHealthInnerGraphic;
 long HealthBarHealthIncreaseEffect,HealthBarHealthBerserkEffect,HealthBarHealthAdrenalineEffect,HealthBarHealthLowHealthEffect,HealthBarHealthDefaultEffect;
 float HealthBarHealthLowPercentage,HealthBarMiddleLength;CRGBColour HealthBarResurrectionColour;
};
static CPlayerGuiDef def;
static float GFAXToVX(float v){return v*2;}static float GFAYToVY(float v){return v*4;}
static long GFIntToWideString(long n){return n;}
struct CPlayerGui {
 int texts;long number;C2DVector pos;CRGBColour ink;
 static const CPlayerGuiDef& PeekGuiDef(){return def;}
 void Draw2DText(C2DVector xy,long n,const CFontBank&,CRGBColour c,float scale,int layer,bool last){
  assert(scale==0.85f && layer==ENGINE_2D_LAYER_1 && !last);++texts;number=n;pos=xy;ink=c;
 }
};
struct ComponentMock {CFontBank font;const CFontBank& PeekMainFont()const{return font;}};
struct WorldMock {
 int id;const WorldMock& PeekDefinitionManager()const{return *this;}
 int GetDefGlobalIndexFromName(const char* name)const{assert(std::strcmp(name,"OBJECT_RESURRECTION_PHIAL")==0);return id;}
};
struct CGraphicDataBank {int GetGraphicFrameWidth(int g,int f)const{assert(g==14 && !f);return 16;}
 int GetGraphicFrameHeight(int g,int f)const{assert(!f && (g==13 || g==15));return g==13?8:4;}};
namespace NParticleEngine {
struct CParticleEmitter {int kills;void Kill(){++kills;}};
struct Manager {
 int spawned;long id;C2DVector pos;CParticleEmitter emitter;
 CParticleEmitter* Create2DEmitter(long i,C2DVector p,int layer,bool on,const CRightHandedSet&){assert(layer==1 && on);++spawned;id=i;pos=p;return &emitter;}
};
static Manager manager;
static Manager& GetParticleEmitterManager(){return manager;}
}
template<class T> struct CIntelligentPointer {
 T* item;CIntelligentPointer(T* p=0):item(p){}T* Get()const{return item;}
 bool operator==(int zero)const{assert(!zero);return !item;}bool operator!=(int zero)const{return !(*this==zero);}
};
struct Fade {
 bool opening;int opens,updates;bool closeOnUpdate;
 bool IsOpening()const{return opening;}
 void Open(float a,float b,CRGBColour c){assert(a==1 && b==0 && !c.R && !c.G && !c.B);opening=true;++opens;}
 void Update(float dt){assert(dt==0.25f);++updates;if(closeOnUpdate)opening=false;}
};
struct CEnginePrimitive2DSprite {
 int graphic,layer,anchor;C2DVector pos;float width,height;
 template<class Shape> CEnginePrimitive2DSprite(int nil,const Shape& s,int l,int g,int frame,int blend,int interp,float a,float b,int alpha,int r,int green,int blue,int an,C2DVector uv,C2DVector extra):
 graphic(g),layer(l),anchor(an),width(0),height(0){
  assert(!nil && !frame && blend==2 && !interp && !a && !b && alpha==127 && r==255 && green==255 && blue==255 && !uv.X && !uv.Y && !extra.X && !extra.Y);set(s);
 }
 void set(C2DVector p){pos=p;}void set(C2DBoxF b){pos=b.p;width=b.w;height=b.h;}
};
struct EngineMock {
 int count,handles[5],graphics[5],layers[5],anchors[5];C2DVector pos[5];float width[5],height[5];
 void SetPersistentPrimitive(int h,const CEnginePrimitive2DSprite& s,int flags){
  assert(flags==0xc0 && count<5);int i=count++;handles[i]=h;graphics[i]=s.graphic;layers[i]=s.layer;anchors[i]=s.anchor;pos[i]=s.pos;width[i]=s.width;height[i]=s.height;
 }
};
struct CEnginePrimitivePrepareInfo {EngineMock& Engine;struct {struct {float GTPredictedTimeSinceLastRenderFrame;}Current;}Interpolate;};
namespace NPlayerGui {
struct CDrawHealth {
 Player player;CPlayerGui PlayerGui;WorldMock World;ComponentMock Component;CGraphicDataBank Bank;
 int PrimitiveHandles[10];CIntelligentPointer<NParticleEngine::CParticleEmitter> Effects[2];Fade HealthIncreaseFade;
 float PreviousHealth,LowHealthPulseScale;long PreviousEffect;bool drawing;int clears;
 Player& GetPlayer(){return player;}void SetAsDrawing(bool v){drawing=v;}unsigned char ScaleAlpha(int a){assert(a==255);return 127;}
 void ClearEffects(CIntelligentPointer<NParticleEngine::CParticleEmitter>* effects){assert(effects==Effects);++clears;}
 void ClearEffects(){ClearEffects(Effects);}
 void Draw(const CEnginePrimitivePrepareInfo&);
};
}

namespace NPlayerGui {
void CDrawHealth::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* phero = GetPlayer().GetPControlledCreature();
    if (!IS_THING_ALIVE(phero))
        return;

    SetAsDrawing(true);
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    const CTCHeroStats* ptc_hero_stats;
    phero->PeekTC(TCI_HERO_STATS, &ptc_hero_stats);
    if (PreviousHealth == -1.0f)
        PreviousHealth = phero->GetHealth();
    LowHealthPulseScale = 1.0f;
    const C2DVector health_pos = gui_def.HealthBarPos + gui_def.HealthBarHealthOffset;
    CEnginePrimitive2DSprite health_sprite(0, GFToScreen(health_pos), ENGINE_2D_LAYER_2,
        gui_def.HealthBarHealthGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[3], health_sprite, 0xc0);

    long effect_index = 0;
    if (PreviousHealth < phero->GetHealth()) {
        HealthIncreaseFade.Open(1.0f, 0.0f, CRGBColour(0, 0, 0));
        effect_index = gui_def.HealthBarHealthIncreaseEffect;
    } else if (HealthIncreaseFade.IsOpening()) {
        effect_index = gui_def.HealthBarHealthIncreaseEffect;
    }
    HealthIncreaseFade.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
    PreviousHealth = phero->GetHealth();
    if (phero->HasTC(TCI_BERSERK)) {
        effect_index = gui_def.HealthBarHealthBerserkEffect;
    } else if (phero->HasTC(TCI_HASTE)) {
        const CTCHaste* ptc_haste;
        phero->PeekTC(TCI_HASTE, &ptc_haste);
        if (const_cast<CTCHaste*>(ptc_haste)->GetActionSpeedMultiplier() > 1.0f)
            effect_index = gui_def.HealthBarHealthAdrenalineEffect;
    }
    if (effect_index == 0)
        effect_index = phero->GetHealthPercentage() < gui_def.HealthBarHealthLowPercentage
            ? gui_def.HealthBarHealthLowHealthEffect : gui_def.HealthBarHealthDefaultEffect;

    if (HealthIncreaseFade.IsOpening() && effect_index == gui_def.HealthBarHealthIncreaseEffect) {
        if (Effects[1] == 0) {
            const C2DVector health_effect_pos = health_pos + gui_def.HealthBarHealthIncreaseEffectOffset;
            Effects[1] = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(effect_index, health_effect_pos,
                    ENGINE_2D_LAYER_1, true, CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
        }
    } else {
        if (Effects[1] != 0) {
            Effects[1].Get()->Kill();
            Effects[1] = 0;
        }
        if (effect_index != PreviousEffect) {
            ClearEffects();
            PreviousEffect = effect_index;
        }
    }

    const CTCInventory* ptc_inventory;
    if (phero->PeekTC(TCI_INVENTORY, &ptc_inventory)) {
        const long def_index = World.PeekDefinitionManager().GetDefGlobalIndexFromName("OBJECT_RESURRECTION_PHIAL");
        if (def_index > 0) {
            const long num_potions = ptc_inventory->GetNumberOfItemsOfTypeInInventory(def_index);
            if (num_potions > 0) {
                // Fidelity: the reference preserves both retail operand orders for the potion position.
                const C2DVector& offset = gui_def.HealthBarResurrectionOffset;
                const C2DVector potion_pos = offset + gui_def.HealthBarPos;
                CRGBColour colour(gui_def.HealthBarResurrectionColour);
                colour.A = ScaleAlpha(255);
                const CFontBank& font = Component.PeekMainFont();
                PlayerGui.Draw2DText(C2DVector(GFAXToVX(potion_pos.X), GFAYToVY(potion_pos.Y)),
                    GFIntToWideString(num_potions), font, colour, 0.85f, ENGINE_2D_LAYER_1, false);
            }
        }
    }

    const CGraphicDataBank& graphic_bank = Bank;
    C2DVector health_bar_pos = gui_def.HealthBarPos;
    CEnginePrimitive2DSprite health_stamina_left_sprite(0, GFToScreen(health_bar_pos), ENGINE_2D_LAYER_5,
        gui_def.HealthBarLeftGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[5], health_stamina_left_sprite, 0xc0);

    const float health_stamina_middle_height = (float)graphic_bank.GetGraphicFrameHeight(gui_def.HealthBarMiddleGraphic, 0);
    const float health_stamina_right_width = (float)graphic_bank.GetGraphicFrameWidth(gui_def.HealthBarRightGraphic, 0);
    health_bar_pos += gui_def.HealthBarMiddleOffset;
    float health_stamina_middle_length = gui_def.HealthBarMiddleLength - health_stamina_right_width;
    const float original_max_health = phero->PeekBaseDef().Health;
    const float current_max_health = phero->GetMaxHealth();
    health_stamina_middle_length *= current_max_health / original_max_health;
    C2DBoxF health_stamina_middle_box(health_bar_pos, C2DExtentsF(health_stamina_middle_length, health_stamina_middle_height));
    CEnginePrimitive2DSprite health_stamina_middle_sprite(0, GFToScreen(health_stamina_middle_box), ENGINE_2D_LAYER_7,
        gui_def.HealthBarMiddleGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[7], health_stamina_middle_sprite, 0xc0);
    health_bar_pos.X += health_stamina_middle_length;
    CEnginePrimitive2DSprite health_stamina_right_sprite(0,
        GFToScreen(health_bar_pos), ENGINE_2D_LAYER_7,
        gui_def.HealthBarRightGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[9], health_stamina_right_sprite, 0xc0);

    const float health_as_percentage = phero->GetHealthPercentage();
    C2DVector health_bar_position = gui_def.HealthBarPos + gui_def.HealthBarHealthInnerOffset;
    const float health_bar_width = (gui_def.HealthBarHealthInnerOffset.X - gui_def.HealthBarMiddleOffset.X
        + health_stamina_middle_length);
    const float health_bar_height = (float)graphic_bank.GetGraphicFrameHeight(gui_def.HealthBarHealthInnerGraphic, 0);
    C2DBoxF scaled_health_bar_box(health_bar_position, C2DExtentsF(health_as_percentage * health_bar_width, health_bar_height));
    CEnginePrimitive2DSprite health_bar_sprite(0, GFToScreen(scaled_health_bar_box), ENGINE_2D_LAYER_6,
        gui_def.HealthBarHealthInnerGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], health_bar_sprite, 0xc0);
}
}

static void checkpos(C2DVector p,float x,float y){assert(p.X==x && p.Y==y);}
int main(){
 def.HealthBarPos=C2DVector(16,32);def.HealthBarHealthOffset=C2DVector(1,2);def.HealthBarHealthIncreaseEffectOffset=C2DVector(8,16);def.HealthBarResurrectionOffset=C2DVector(2,4);
 def.HealthBarMiddleOffset=C2DVector(4,2);def.HealthBarHealthInnerOffset=C2DVector(6,4);def.HealthBarMiddleLength=80;
 def.HealthBarHealthGraphic=11;def.HealthBarLeftGraphic=12;def.HealthBarMiddleGraphic=13;def.HealthBarRightGraphic=14;def.HealthBarHealthInnerGraphic=15;
 def.HealthBarHealthIncreaseEffect=21;def.HealthBarHealthBerserkEffect=22;def.HealthBarHealthAdrenalineEffect=23;def.HealthBarHealthLowHealthEffect=24;def.HealthBarHealthDefaultEffect=25;
 def.HealthBarHealthLowPercentage=0.25f;def.HealthBarResurrectionColour=CRGBColour(13,17,23,255);
 const int expectedHandles[]={3,5,7,9,0},layers[]={2,5,7,7,6};
 for(int scenario=0;scenario<2048;++scenario){
  CThingPlayerCreature hero={};hero.alive=(scenario&3)!=0;hero.health=(scenario&4)?32.0f:16.0f;hero.max=128;hero.def.Health=64;
  hero.berserk=(scenario&8)!=0;hero.haste=(scenario&16)!=0;hero.accelerated.speed=(scenario&32)?2.0f:1.0f;
  hero.inventory=(scenario&64)!=0;hero.items.count=(scenario&128)?3:0;
  NPlayerGui::CDrawHealth row={};row.World.id=(scenario&256)?9:0;row.player.hero=(scenario&3)==3?0:&hero;
  row.PreviousHealth=(scenario&512)?8.0f:((scenario&16)?-1.0f:64.0f);row.PreviousEffect=(scenario&4)?25:-1;
  row.HealthIncreaseFade.opening=(scenario&1024)!=0;row.HealthIncreaseFade.closeOnUpdate=(scenario&64)!=0;
  for(int j=0;j<10;++j)row.PrimitiveHandles[j]=j;
  NParticleEngine::Manager& mgr=NParticleEngine::manager;mgr.spawned=0;mgr.emitter.kills=0;
  bool had=(scenario&128)!=0;if(had)row.Effects[1]=&mgr.emitter;
  bool alive=hero.alive && row.player.hero,healed=row.PreviousHealth!=-1 && row.PreviousHealth<hero.health;
  bool opens=row.HealthIncreaseFade.opening || healed;
  long effect=hero.berserk?22:hero.haste && hero.accelerated.speed>1?23:opens?21:hero.health/hero.max<0.25f?24:25;
  bool show=opens && !row.HealthIncreaseFade.closeOnUpdate && effect==21;
  long old=row.PreviousEffect;float previous=row.PreviousHealth;
  EngineMock engine={};CEnginePrimitivePrepareInfo info={engine,{{0.25f}}};row.Draw(info);
  assert(row.drawing==alive && row.HealthIncreaseFade.updates==(alive?1:0));
  if(!alive){assert(!engine.count && row.PreviousHealth==previous && !mgr.spawned && !mgr.emitter.kills && !row.clears && !row.PlayerGui.texts);continue;}
  assert(row.HealthIncreaseFade.opens==(healed?1:0) && row.LowHealthPulseScale==1 && row.PreviousHealth==hero.health);
  assert(mgr.spawned==(show && !had?1:0) && mgr.emitter.kills==(!show && had?1:0));
  if(mgr.spawned){assert(mgr.id==21);checkpos(mgr.pos,25,50);}
  assert((row.Effects[1].Get()!=0)==show && row.clears==(!show && old!=effect?1:0) && row.PreviousEffect==(show?old:effect));
  bool text=hero.inventory && row.World.id>0 && hero.items.count>0;assert(row.PlayerGui.texts==(text?1:0));
  if(text){assert(row.PlayerGui.number==3 && row.PlayerGui.ink.R==13 && row.PlayerGui.ink.G==17 && row.PlayerGui.ink.B==23 && row.PlayerGui.ink.A==127);checkpos(row.PlayerGui.pos,36,144);}
  assert(engine.count==5);
  for(int k=0;k<5;++k)assert(engine.handles[k]==expectedHandles[k] && engine.graphics[k]==11+k && engine.layers[k]==layers[k] && engine.anchors[k]==(k?2:0));
  checkpos(engine.pos[0],17,34);checkpos(engine.pos[1],16,32);checkpos(engine.pos[2],20,34);checkpos(engine.pos[3],148,34);checkpos(engine.pos[4],22,36);
  assert(engine.width[2]==128 && engine.height[2]==8 && engine.width[4]==130*(hero.health/hero.max) && engine.height[4]==4);
 }
 std::puts("00647ab4_TEST PASS");return 0;
}