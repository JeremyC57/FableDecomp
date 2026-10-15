#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <new>
static int g_scriptBase = 0, g_baseObject = 0;
struct CScriptBase { virtual void v00() { } unsigned char _body[0x3c]; CScriptBase() { ++g_scriptBase; } };
struct __declspec(novtable) CScriptObject : CScriptBase { void* m_pInterface; void* m_pData; CScriptObject(void* data, void* iface) : CScriptBase(), m_pInterface(iface), m_pData(data) { } };
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
struct CBaseObject { virtual void v00() { } void* m_pIntelligentPointer; CBaseObject() : m_pIntelligentPointer(0) { ++g_baseObject; } };
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject { CCountedPointerWords PImp; virtual void v00() { } CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { } };
struct CVtable_012f17dc : CScriptObject {
    CScriptGameResourceObjectScriptedThingBase m_first; CScriptGameResourceObjectScriptedThingBase m_second; virtual void v00() { }
    CVtable_012f17dc(void* data, void* iface) : CScriptObject(data, iface), m_first(), m_second() { }
};
int main()
{
    int data, iface; unsigned char raw[sizeof(CVtable_012f17dc)]; for (unsigned k = 0; k < sizeof(raw); ++k) raw[k] = 0xAA;
    CVtable_012f17dc* s = new (raw) CVtable_012f17dc(&data, &iface);
    if (g_scriptBase != 1 || g_baseObject != 2 || s->m_pData != &data || s->m_pInterface != &iface || s->m_first.PImp.Data != 0 || s->m_first.PImp.Info != 0 || s->m_second.PImp.Data != 0 || s->m_second.PImp.Info != 0) { std::printf("00ec53c0_TEST FAIL\n"); return 1; }
    std::printf("00ec53c0_TEST PASS\n"); return 0;
}