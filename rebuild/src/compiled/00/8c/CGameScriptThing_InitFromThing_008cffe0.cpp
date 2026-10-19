// CGameScriptThing::InitFromThing -- bind a thing and copy script identity and spatial state
// Actual headers retained; native CThing differs from quarantined donor (nine component words, twelve-byte containers).
#include "engine/CGameScriptThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/_quarantine/CThing.h"
#include "fable_string.h"
struct Vector3{float X,Y,Z;float LengthSquared()const{return X*X+Y*Y+Z*Z;}};
struct PhysicsMethods:CTCPhysicsBase{const Vector3& GetPosition()const{return *reinterpret_cast<const Vector3*>(&Position_X);}typedef float(PhysicsMethods::*AngleMethod)()const;float GetAngleXY()const{return(this->*static_cast<AngleMethod*>(__vftable)[63])();}}; //nativeVT125D2C4 slot63=006B1230
FABLE_STATIC_ASSERT(sizeof(PhysicsMethods::AngleMethod)==4);
struct ThingNative{void* VTable;unsigned char Prefix[12];unsigned char Category;unsigned char BeforeUID[7];unsigned __int64 UniqueID;unsigned char BeforePhysics[0x40];PhysicsMethods* Physics;unsigned char BeforeName[0x10];CCharString Name;CCharString DataString;unsigned char BeforeHome[0x6c];Vector3 Home;const PhysicsMethods* GetPhysicsTC()const{return Physics;}};
FABLE_STATIC_ASSERT(offsetof(ThingNative,Physics)==0x60);
FABLE_STATIC_ASSERT(offsetof(ThingNative,Name)==0x74);
FABLE_STATIC_ASSERT(offsetof(ThingNative,Home)==0xe8);
struct IntelligentInfo{ThingNative* Object;long References;};
struct IntelligentPointerBase{IntelligentInfo* Info;IntelligentPointerBase();virtual ~IntelligentPointerBase();ThingNative* Get()const;void Set(ThingNative*);}; //00A01B10,00A01C10,00A01B50,00A01B90; baseVT129C95C
struct IntelligentPointer:IntelligentPointerBase{IntelligentPointer(){}explicit IntelligentPointer(ThingNative* thing){Set(thing);}IntelligentPointer& operator=(const IntelligentPointer& other){Set(other.Get());return *this;}}; //nativeVT1238C6C only deleting destructor004A9710
__forceinline float PhysicsAngle(const IntelligentPointer& pointer){return pointer.Get()->GetPhysicsTC()->GetAngleXY();}
// Typed native projection permits actual intelligent-pointer/string members on either host ABI; offsets asserted against actual header.
struct GameScriptThingNative{void* VTable;unsigned char Prefix[8];IntelligentPointer PThing;long ScriptCounter;unsigned __int64 ThingUID;CCharString Name,DataString;Vector3 Pos,HomePos;float AngleXY;CWorld* World;void InitFromThing(const ThingNative&);};
FABLE_STATIC_ASSERT(sizeof(GameScriptThingNative)==sizeof(CGameScriptThing));
FABLE_STATIC_ASSERT(offsetof(GameScriptThingNative,PThing)==offsetof(CGameScriptThing,PThing));
FABLE_STATIC_ASSERT(offsetof(GameScriptThingNative,ThingUID)==offsetof(CGameScriptThing,ThingUID));
FABLE_STATIC_ASSERT(offsetof(GameScriptThingNative,HomePos)==offsetof(CGameScriptThing,HomePos_X));
extern const float PositionTolerance; //0129BA3C actual float0.0001, not a folded square literal
void GameScriptThingNative::InitFromThing(const ThingNative& thing){
 {IntelligentPointer temporary(const_cast<ThingNative*>(&thing));ThingNative* rebound=temporary.Get();PThing.Set(rebound);}Name=PThing.Get()->Name;ThingUID=PThing.Get()->UniqueID;DataString=PThing.Get()->DataString;
 if(!(HomePos.LengthSquared()>PositionTolerance*PositionTolerance)&&PThing.Get()->Category==1)HomePos=PThing.Get()->Home;
 if(PThing.Get()->GetPhysicsTC()){const Vector3& position=PThing.Get()->GetPhysicsTC()->GetPosition();Pos=position;AngleXY=PThing.Get()->GetPhysicsTC()->GetAngleXY();}
}