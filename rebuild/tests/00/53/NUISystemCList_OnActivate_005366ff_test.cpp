#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_deleted = 0; struct COwner { virtual void v000() { } virtual void Delete() { ++g_deleted; } unsigned long IVRefCount; unsigned char _pad[0x154]; bool m_active; };
struct CDefPtr { COwner* m_pObject; CDefPtr() { } CDefPtr(const CDefPtr& o) : m_pObject(o.m_pObject) { ++m_pObject->IVRefCount; } ~CDefPtr() { COwner* o = m_pObject; unsigned long* c = &m_pObject->IVRefCount; --*c; if (*(volatile unsigned long*)c == 0) o->Delete(); } };
struct CList { COwner* m_pOwner; bool m_ownerActive; virtual CDefPtr GetOwner() { CDefPtr p; p.m_pObject = m_pOwner; return p; } /* hands over one existing reference */ void OnActivate() { CDefPtr owner = GetOwner(); if (owner.m_pObject->m_active) m_ownerActive = true; } };

int main() {
    COwner o; o.IVRefCount = 2; o.m_active = true; CList l; l.m_pOwner = &o; l.m_ownerActive = false; l.OnActivate(); if (!l.m_ownerActive || o.IVRefCount != 1 || g_deleted != 0) { std::printf("005366ff_TEST FAIL\n"); return 1; }
    o.m_active = false; l.m_ownerActive = false; l.OnActivate(); if (l.m_ownerActive || o.IVRefCount != 0 || g_deleted != 1) { std::printf("005366ff_TEST FAIL\n"); return 1; }
    std::printf("005366ff_TEST PASS\n"); return 0;
}