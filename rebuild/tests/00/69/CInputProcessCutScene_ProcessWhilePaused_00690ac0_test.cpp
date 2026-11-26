// Behaviour test for 0x00690AC0 CInputProcessCutScene::ProcessWhilePaused (NF478 respell): links the row object, defines the singleton's
// counted pointer, and checks the result follows the player GUI's byte at +0xF9.
template <class T> class CCountedPointer { public: T* Data; void* Info; T& operator*() const { return *Data; } };   // test model: Data +0x0 / Info +0x4 (engine/CCountedPointer.h layout)
template <class T> class CTBaseSingletonNoDefaultAlloc {
    static CCountedPointer<T> PClass;
public:
    static T& __fastcall Get() { return *PClass; }
    static void Set(T* p) { PClass.Data = p; }
};
#pragma pack(push,1)
class CPlayerGui { public: char pad[0xf9]; bool flag; };
#pragma pack(pop)
template <class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;   // the row object references ?PClass@..@@0V?$CCountedPointer@VCPlayerGui@@@@A
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessCutScene.h"
#include <stdio.h>
static CPlayerGui g_gui;
static unsigned char g_process[0x40];
int main(void)
{
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&g_gui);
    const CInputProcessCutScene* p = (const CInputProcessCutScene*)g_process;
    g_gui.flag = true; bool a = p->CInputProcessCutScene::ProcessWhilePaused();
    g_gui.flag = false; bool b = p->CInputProcessCutScene::ProcessWhilePaused();
    if (a && !b) { printf("PASS_00690ac0\n"); return 0; }
    printf("FAIL_00690ac0\n"); return 1;
}
