// CBaseIntelligentPointer::~CBaseIntelligentPointer() -- the plain destructor: own vtable, then ClearData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01C10, oracle length 52 bytes.
// Callees: operator delete @ 0x00BFE9BC (__cdecl, `add esp,4`), through the inlined ClearData (0x00A01B20).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; mov dword ptr [esi],offset vftable ;
//                je E ; dec dword ptr [eax+4] ; mov eax,[esi+4] ; mov ecx,[eax+4] ; test ecx,ecx ; jne N ;
//                cmp dword ptr [eax],0 ; jne N ; push eax ; call operator delete ; add esp,4 ;
//                N: mov dword ptr [esi+4],0 ; E: pop esi ; ret
// The vtable (0x0129C95C, rtti-vtables.tsv: CBaseIntelligentPointer) is re-installed as every
// destructor does and ClearData is inlined.  The class is polymorphic through a plain virtual
// function here so only the `??1` body is emitted (a virtual destructor would make the lander grade
// the deleting `??_G`, which is 0x00A01C50).
struct CBaseObject;

struct CBaseObjectPointer {
    CBaseObject* PItem;        // +0x00
    long         RefCount;     // +0x04
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObjectPointer* m_pData;   // +0x04
    void ClearData()               // 0x00A01B20, inlined here
    {
        if (m_pData != 0) {
            m_pData->RefCount--;
            if (m_pData->RefCount == 0 && m_pData->PItem == 0)
                delete m_pData;
            m_pData = 0;
        }
    }
    ~CBaseIntelligentPointer();    // 0x00A01C10
};

CBaseIntelligentPointer::~CBaseIntelligentPointer()
{
    ClearData();
}
