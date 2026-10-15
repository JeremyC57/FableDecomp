// CDispatcher_00c1ac40::CDispatcher_00c1ac40(CSink_00c1ac40* sink) -- constructor: keep the sink, make the node list's head sentinel point at itself, clear the two counters, park both cursors on the head
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AE20, length 33 bytes (+15 int3 padding).
// Leaf: ??0CDispatcher_00c1ac40
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; lea ecx,[eax+4] ; mov [eax+1Ch],ecx ; mov [eax+20h],ecx ;
//                mov [ecx],ecx ; mov [eax+8],ecx ; xor ecx,ecx ; mov [eax+0Ch],ecx ; mov [eax+10h],ecx ; ret 4
// Manifest label "CResourceList::CResourceList" is propagated and wrong: no vtable, and the landed CFileManager
// constructor (0x00C1AC40) calls this as the constructor of its 0x24-byte dispatcher at +0x20, passing the vptr-only sink
// at +0x1C; the class is named by address as in that model. +0x14/+0x18 are not touched here.
// Shape: the sink is stored first (initialiser), then the body in source order: the two cursors, the sentinel's
// next/prev, the two zeroed words.
struct CSink_00c1ac40 { virtual void v00(); };     // vtable 0x012B30EC

struct SDispatchNode_00c1ae20 {
    SDispatchNode_00c1ae20* m_pNext;                  // +0x00
    SDispatchNode_00c1ae20* m_pPrev;                  // +0x04
};

struct CDispatcher_00c1ac40 {                         // 0x24 bytes
    CSink_00c1ac40*         m_pSink;                  // +0x00
    SDispatchNode_00c1ae20  m_head;                   // +0x04 (sentinel)
    long                    m_count;                  // +0x0C
    long                    m_pending;                // +0x10
    long                    m_reserved14;             // +0x14
    long                    m_reserved18;             // +0x18
    SDispatchNode_00c1ae20* m_pCursor;                // +0x1C
    SDispatchNode_00c1ae20* m_pLast;                  // +0x20
    CDispatcher_00c1ac40(CSink_00c1ac40* sink);
};

CDispatcher_00c1ac40::CDispatcher_00c1ac40(CSink_00c1ac40* sink)
    : m_pSink(sink)
{
    m_pCursor = &m_head;
    m_pLast = &m_head;
    m_head.m_pNext = &m_head;
    m_head.m_pPrev = &m_head;
    m_count = 0;
    m_pending = 0;
}
