// CThing::IsUnderScriptSupervision() const -- true when the thing has a script-side CScriptThing and that script thing's script counter is positive
// Leaf: ?IsUnderScriptSupervision@CThing@@QBE_NXZ
// Retail: Fable.exe @ 0x004C7E90, 169 bytes. Ego_r order GetScriptThing 0x004C9A10, HasScriptThing 0x004C9A30,
//   IsUnderScriptSupervision 0x004C9A40 = retail 0x004C7E50 (reloc_bindings: ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ, bound
//   from caller 0x009038D0), 0x004C7E70, 0x004C7E90. Callees: CGameScriptThingManager::HasScriptThing 0x00704790 and
//   ::CreateScriptThing 0x00704860 (Ego_r), CScriptThing slot 74 GetScriptCounter (+0x128), ??3@YAXPAX@Z, ??1CBaseClass@@UAE@XZ;
//   CQuestManager singleton PClass 0x013B89FC.
// Shape: the lever ChatGPT pass 10 lacked is the same-TU callees. Retail writes "HasScriptThing() && GetScriptThing().GetScriptCounter() > 0" through the two CThing members defined just above it in CThing.cpp, and VC7.1 inlines both. Inlined
//   by-value GetScriptThing returns into the caller's temporary through its own return slot, so the virtual call takes the
//   temporary's frame address (mov eax,[esp+10h] ; lea ecx,[esp+10h]) instead of the factory's returned eax, and the counted-
//   pointer release reloads Info into ecx. Calling the manager directly (pass 10): 165v169.
// Layout: engine/_quarantine/CThing.h fields, engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT: vptr, PImp Data/Info, 12 bytes),
//   engine/CQuestManager.h (PGameScriptThingManager_Data +4), engine/CCountedPointer.h.
// Note: the out-of-line GetScriptThing this TU emits matches retail 0x004C7E50 byte for byte; its HasScriptThing differs from
//   retail 0x004C7E70 in register choice (retail mov eax,ecx first), not graded here.
#include "fable_compiler.h"
#define CThing CThingLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CQuestManager.h"
// Layout: real const method on imported CThing fields; no added data or virtual slot.
class CThing:public CThingLayout{public:CScriptThing GetScriptThing()const;bool HasScriptThing()const;bool IsUnderScriptSupervision()const;};
class CGameScriptThingManager{public:bool HasScriptThing(const CThing&);CScriptThing CreateScriptThing(const CThing&);};
template<class T>class CTBaseSingletonNoDefaultAlloc{public:static T&__fastcall Get(){return *PClass;}private:static CCountedPointer<T>PClass;};
FABLE_FORCEINLINE CScriptThing::~CScriptThing(){}
CScriptThing CThing::GetScriptThing()const{
 return CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PGameScriptThingManager_Data->CreateScriptThing(*this);
}
bool CThing::HasScriptThing()const{
 return CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PGameScriptThingManager_Data->HasScriptThing(*this);
}
bool CThing::IsUnderScriptSupervision()const{
 return HasScriptThing() && GetScriptThing().GetScriptCounter()>0;
}