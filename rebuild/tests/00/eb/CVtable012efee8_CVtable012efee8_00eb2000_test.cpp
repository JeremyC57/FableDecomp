#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <new>
static int g_copies = 0, g_baseObject = 0, g_strings = 0; static const void* g_copySource = 0;
struct CScriptThing { unsigned char _body[0xc]; CScriptThing() { } CScriptThing(const CScriptThing& other) { ++g_copies; g_copySource = &other; } };
struct CScriptOwner { unsigned char _pad00[0x40]; void* m_pInterface; };
struct CVtable_012c3224 {
    virtual void v00() { } void* m_pInterface; CScriptThing m_thing;
    CVtable_012c3224(const CScriptThing& thing, void* iface) : m_pInterface(iface), m_thing(thing) { }
};
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
struct CBaseObject { virtual void v00() { } void* m_pIntelligentPointer; CBaseObject() : m_pIntelligentPointer(0) { ++g_baseObject; } };
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject { CCountedPointerWords PImp; virtual void v00() { } CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { } };
struct CCharString { void* m_pStringData; CCharString() : m_pStringData(0) { ++g_strings; } };
struct CVtable_012efee8 : CVtable_012c3224 {
    CScriptOwner* m_pOwner; long m_index; CScriptGameResourceObjectScriptedThingBase m_scriptedThing; unsigned char _pad2c[8]; CCharString m_name; virtual void v00() { }
    CVtable_012efee8(CScriptOwner* owner, long index, const CScriptThing& thing) : CVtable_012c3224(thing, owner->m_pInterface), m_pOwner(owner), m_scriptedThing(), m_name() { m_index = index; }
};
int main()
{
    int iface; CScriptOwner owner; owner.m_pInterface = &iface; CScriptThing thing;
    unsigned char raw[sizeof(CVtable_012efee8)]; for (unsigned k = 0; k < sizeof(raw); ++k) raw[k] = 0xAA;
    CVtable_012efee8* h = new (raw) CVtable_012efee8(&owner, 5, thing);
    if (g_copies != 1 || g_copySource != &thing || g_baseObject != 1 || g_strings != 1 || h->m_pInterface != &iface || h->m_pOwner != &owner || h->m_index != 5 || h->m_scriptedThing.PImp.Data != 0 || h->m_scriptedThing.PImp.Info != 0 || h->m_name.m_pStringData != 0 || h->_pad2c[0] != 0xAA) { std::printf("00eb2000_TEST FAIL\n"); return 1; }
    std::printf("00eb2000_TEST PASS\n"); return 0;
}