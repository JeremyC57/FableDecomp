// Behaviour test for 0x006445BA NPlayerGui::CDrawBase::ClearEffects: every entry with a live emitter has Kill called
// once and is reset to a null pointer; null entries are left alone.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;                                      // 0x00A01B50
    virtual ~CBaseIntelligentPointer();                                         // 0x00A01C10
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                                       // 0x00655318
    CIntelligentPointer& operator=(const CIntelligentPointer& other);  // 0x0065533C
    T* operator->() const { return (T*)GetPItem(); }
};

namespace NParticleEngine {
class CParticleEmitter { public:
    void Kill();                                                        // 0x00AEE8C0
};
}

namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

namespace NPlayerGui {

class CDrawBase {
public:
    static void __fastcall ClearEffects(std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >& effects);
};

void __fastcall CDrawBase::ClearEffects(std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >& effects)
{
    for (unsigned int i = 0; i < effects.size(); ++i) {
        if (effects[i].GetPItem()) {
            effects[i]->Kill();
            CIntelligentPointer<NParticleEngine::CParticleEmitter> none(0);
            effects[i] = none;
        }
    }
}

}


static int g_kills = 0;
static int g_dtors = 0;
void CBaseIntelligentPointer::v00() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
template <class T> CIntelligentPointer<T>::CIntelligentPointer(T* item) { m_pItem = (CBaseObject*)item; }
template <class T> CIntelligentPointer<T>& CIntelligentPointer<T>::operator=(const CIntelligentPointer& other)
{ m_pItem = other.m_pItem; return *this; }
void NParticleEngine::CParticleEmitter::Kill() { ++g_kills; }

int main()
{
    typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> TPtr;
    static NParticleEngine::CParticleEmitter emitters[3];
    static unsigned char storage[4 * sizeof(TPtr)];
    TPtr* p = (TPtr*)storage;
    memset(storage, 0, sizeof(storage));
    p[0].m_pItem = (CBaseObject*)&emitters[0];
    p[1].m_pItem = 0;
    p[2].m_pItem = (CBaseObject*)&emitters[1];
    p[3].m_pItem = (CBaseObject*)&emitters[2];
    std::vector<TPtr> v;
    v._M_start = p; v._M_finish = p + 4; v._M_end_of_storage = p + 4;

    int ok = 1;
    NPlayerGui::CDrawBase::ClearEffects(v);
    if (g_kills != 3 || g_dtors != 3) ok = 0;
    for (unsigned int i = 0; i < 4; ++i)
        if (p[i].m_pItem != 0) ok = 0;
    NPlayerGui::CDrawBase::ClearEffects(v);            // all null now: nothing happens
    if (g_kills != 3 || g_dtors != 3) ok = 0;
    v._M_finish = p;                                    // empty vector
    NPlayerGui::CDrawBase::ClearEffects(v);
    if (g_kills != 3) ok = 0;

    printf(ok ? "006445ba_TEST PASS\n" : "006445ba_TEST FAIL\n");
    return ok ? 0 : 1;
}