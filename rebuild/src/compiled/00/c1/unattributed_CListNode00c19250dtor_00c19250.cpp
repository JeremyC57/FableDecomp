// CListNode_00c19250::~CListNode_00c19250() -- EH-framed destructor (vtable 0x012B2F0C over the base 0x012B2DB0): when the head pointer at +0x20 does not point at the sentinel at +0xc the list is not empty and `.\\PKCTRRedblackTree.cpp(79) : Error: There are still items in this map` is logged through a stack string (in its own block, state 1 over the base state 0; chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) with an empty trace temporary on a dead frame byte (@ 0x00C16750); the block at +8 is freed (`operator delete`) and nulled; the inlined base destructor stores its vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19250, oracle length 227 bytes. Built under /EHsc.
// Retail bytes:  ... mov dword ptr [esi],012B2F0Ch ; mov ecx,[esi+20h] ; lea eax,[esi+0Ch] ; cmp ecx,eax ; mov dword ptr [esp+18h],0 ; je free ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset file ; lea ecx,[esp+8] ; mov byte ptr [esp+18h],1 ; call Append ; ... AppendNumber(4Fh) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov byte ptr [esp+18h],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; free: mov eax,[esi+8] ; test eax,eax ; je done ; push eax ; call operator delete ; add esp,4 ; mov dword ptr [esi+8],0 ; done: mov ecx,[esp+10h] ; mov dword ptr [esi],012B2DB0h ; ...
// The 0x28-byte node every 0x012B1AB0-family container embeds (its destructor is called from
// all of them). The manifest label is a description; the classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c19250)(const char* text);
extern PFNLOGSINK_00c19250 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c19250 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c19250() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c19250();                       // 0x00C0BA40
    CString_00c19250* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c19250* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c19250 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c19250();                         // 0x00C16750
};

void __cdecl operator delete(void* block);

struct CListBase_00c19250 {                // vtable 0x012B2DB0
    virtual void v00();
    ~CListBase_00c19250() { }
};

struct CListNode_00c19250 : CListBase_00c19250 {   // vtable 0x012B2F0C
    long m_4;
    void* m_pBlock;                        // +8
    unsigned char m_sentinel[0x14];        // +0xc
    unsigned char* m_pHead;                // +0x20
    long m_24;
    ~CListNode_00c19250();
};

CListNode_00c19250::~CListNode_00c19250()
{
    if (m_pHead != m_sentinel) {
        {
            CString_00c19250 line;
            line.Append(".\\PKCTRRedblackTree.cpp")->Append("(")->AppendNumber(79)->Append(") : ")->Append("Error: There are still items in this map")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c19250();
    }
    if (m_pBlock) {
        operator delete(m_pBlock);
        m_pBlock = 0;
    }
}