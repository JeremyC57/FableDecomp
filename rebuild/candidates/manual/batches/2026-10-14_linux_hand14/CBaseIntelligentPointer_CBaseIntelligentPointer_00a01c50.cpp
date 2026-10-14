// CBaseIntelligentPointer::`scalar deleting destructor'(unsigned int) -- ~CBaseIntelligentPointer then optional operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01C50, oracle length 70 bytes.
// Callees: operator delete @ 0x00BFE9BC (__cdecl, `add esp,4`) twice: the record in the inlined
// ClearData (0x00A01B20) and the object itself when bit 0 of the flags argument is set.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; mov dword ptr [esi],offset vftable ;
//                je E ; ... (the ~CBaseIntelligentPointer body of 0x00A01C10) ... ; E: test byte ptr [esp+8],1 ;
//                je R ; push esi ; call operator delete ; add esp,4 ; R: mov eax,esi ; pop esi ; ret 4
// Vtable slot 0 of 0x0129C95C (rtti-vtables.tsv): the compiler-generated `??_G` of a class with a
// virtual destructor; the destructor body (0x00A01C10) is inlined into it.
struct CBaseObject;

struct CBaseObjectPointer {
    CBaseObject* PItem;        // +0x00
    long         RefCount;     // +0x04
};

struct CBaseIntelligentPointer {
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
    virtual ~CBaseIntelligentPointer();    // 0x00A01C10; `??_G` is 0x00A01C50
};

CBaseIntelligentPointer::~CBaseIntelligentPointer()
{
    ClearData();
}

void DestroyIntelligentPointer(CBaseIntelligentPointer* p)
{
    delete p;
}
