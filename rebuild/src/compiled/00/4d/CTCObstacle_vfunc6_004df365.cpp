// CTCObstacle::vfunc_6() -- creates the obstacle's navigator interface for the parent thing, then its steering agent on that interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF365, length 79 bytes.
// Callees: operator new @ 0x00BFEA1A (0x14 and 0x68 bytes), CSteeringAgent::CSteeringAgent(CObstacleNavigatorExternalInterface*,
// bool) @ 0x00A1C700 (manifest label "CNavigatorBase_Constructor").
// Retail CTCObstacle (PDB, 0x14 bytes): SteeringAgent +0x0c, PInterface +0x10. CObstacleNavigatorExternalInterface (vtable
// 0x01241D04): the thing +0x04, then three cleared words +0x08..+0x10 that are an STLport vector member (its constructor inlined).
// Shape: the size-optimised module (`pop ecx` after operator new, `and dword ptr [eax+N],0`); each `new` null-tested with the
// `xor eax,eax` arm; the interface's vtable and thing stores come BEFORE its three zero stores, so the zeroed words are a
// member object with an inline constructor (as plain initialisers they are scheduled before the vtable store); the agent is
// built from PInterface re-read from its member.
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);      // 0x00BFEA1A

class CThing;

struct SAllocator { SAllocator() {} };          // empty allocator (user constructor: no zero fill of the temporary)

template<class T> struct SAllocProxy : SAllocator { // STLport _STLP_alloc_proxy: the end-of-storage pointer over the allocator
    T* m_data;
    SAllocProxy(const SAllocator& a, T* data) : SAllocator(a), m_data(data) {}
};

template<class T> struct CVectorBase {              // STLport _Vector_base
    T*             m_pFirst;
    T*             m_pLast;
    SAllocProxy<T> m_end;
    CVectorBase(const SAllocator& a) : m_pFirst(0), m_pLast(0), m_end(a, 0) {}
};

template<class T> struct CRetailVector : CVectorBase<T> {
    CRetailVector() : CVectorBase<T>(SAllocator()) {}
};

class CObstacleNavigatorExternalInterface { public:
    virtual void v00();
    CThing*               m_pThing;                 // +0x04
    CRetailVector<void*>  m_entries;                // +0x08 (three words)
    CObstacleNavigatorExternalInterface(CThing* thing) : m_pThing(thing) {}
};

class CSteeringAgent { public:
    unsigned char _body[0x68];
    CSteeringAgent(CObstacleNavigatorExternalInterface* navigatorInterface, bool flag);   // 0x00A1C700
};

struct CTCBase_004df365 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

class CTCObstacle : public CTCBase_004df365 { public:
    CSteeringAgent*                      SteeringAgent;   // +0x0c
    CObstacleNavigatorExternalInterface* PInterface;      // +0x10
    virtual void vfunc_6();
};

void CTCObstacle::vfunc_6()
{
    PInterface = new CObstacleNavigatorExternalInterface(ParentThing);
    SteeringAgent = new CSteeringAgent(PInterface, true);
}