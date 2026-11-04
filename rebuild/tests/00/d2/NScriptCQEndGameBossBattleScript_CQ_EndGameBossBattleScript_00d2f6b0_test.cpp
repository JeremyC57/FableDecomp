#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <new>
static int g_scriptBase = 0, g_baseObject = 0, g_baseClass = 0;
class CScriptBase { public: virtual void v00() { } unsigned char _body[0x3c]; CScriptBase() { ++g_scriptBase; } };
struct __declspec(novtable) CScriptObject : CScriptBase { void* m_pInterface; void* m_pData; CScriptObject(void* data, void* iface) : CScriptBase(), m_pInterface(iface), m_pData(data) { } };
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
class CBaseObject { public: virtual void v00() { } void* m_pIntelligentPointer; CBaseObject() : m_pIntelligentPointer(0) { ++g_baseObject; } };
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public: CCountedPointerWords PImp; virtual void v00() { } CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { } };
class CBaseClass { public: virtual void v00() { } CBaseClass() { ++g_baseClass; } };
class CScriptThing : public CBaseClass { public: CCountedPointerWords PImp; virtual void v00() { } CScriptThing() : CBaseClass(), PImp() { } };
struct CVtable_012cc60c : CScriptObject {
    unsigned char _pad48[8]; CScriptGameResourceObjectScriptedThingBase m_scriptedThing; CScriptThing m_thing; virtual void v00() { }
    CVtable_012cc60c(void* data, void* iface) : CScriptObject(data, iface), m_scriptedThing(), m_thing() { }
};
int main()
{
    int data, iface; unsigned char raw[sizeof(CVtable_012cc60c)]; for (unsigned k = 0; k < sizeof(raw); ++k) raw[k] = 0xAA;
    CVtable_012cc60c* s = new (raw) CVtable_012cc60c(&data, &iface);
    if (g_scriptBase != 1 || g_baseObject != 1 || g_baseClass != 1 || s->m_pData != &data || s->m_pInterface != &iface || s->m_scriptedThing.PImp.Data != 0 || s->m_scriptedThing.PImp.Info != 0 || s->m_scriptedThing.m_pIntelligentPointer != 0 || s->m_thing.PImp.Data != 0 || s->m_thing.PImp.Info != 0 || s->_pad48[0] != 0xAA) { std::printf("00d2f6b0_TEST FAIL\n"); return 1; }
    std::printf("00d2f6b0_TEST PASS\n"); return 0;
}