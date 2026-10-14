// CSoundEmitterLH::CSoundEmitterLH(CGameScriptThing* thing, const C3DVector& position) -- CBase, the thing at +4, this class's vtable, the position at +8..+0x10, an empty retail list at +0x14 (its sentinel node malloc'd inline), three flag bytes, the thing's script counter value at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC580, length 94 bytes.
// Callees: CBase::CBase @ 0x0099A2D0, malloc @ 0x00BFEA0E (the 16-byte sentinel), CGameScriptThing::DecrementScriptCounter @ 0x00A3E420 (on the thing; its result at +0x1c).
// Vtable 0x0129E6EC = CSoundEmitterLH (rtti-vtables.tsv). Shape: the VC6 list constructor inlined (`mov [esi+14h],0 ; push 10h ; call malloc ; mov [eax],eax ;
// mov [eax+4],eax ; mov [esi+14h],eax`), the three bytes stored around the `add esp,4`, the thing re-read from +4 for the last call.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct C3DVector { float X, Y, Z; };

struct CGameScriptThing {
    long DecrementScriptCounter();                 // 0x00A3E420
};

struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2D0
};

struct SListNode_00aac580 { SListNode_00aac580* m_pNext; SListNode_00aac580* m_pPrev; long m_a; long m_b; };   // 16 bytes

struct CRetailList_00aac580 {                      // VC6 Dinkumware list: the sentinel node is malloc'd by the constructor
    SListNode_00aac580* m_pHead;
    CRetailList_00aac580()
    {
        m_pHead = 0;
        SListNode_00aac580* node = (SListNode_00aac580*)malloc(sizeof(SListNode_00aac580));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
};

struct CSoundEmitterLH : CBase {                   // vtable 0x0129E6EC
    CGameScriptThing*    m_pThing;                 // +0x04
    float                m_positionX;              // +0x08 (three floats initialised one by one)
    float                m_positionY;              // +0x0c
    float                m_positionZ;              // +0x10
    CRetailList_00aac580 m_sounds;                 // +0x14
    bool                 m_active;                 // +0x18
    bool                 m_paused;                 // +0x19
    bool                 m_finished;               // +0x1a
    long                 m_scriptCounter;          // +0x1c
    virtual void v00();
    CSoundEmitterLH(CGameScriptThing* thing, const C3DVector& position);
};

CSoundEmitterLH::CSoundEmitterLH(CGameScriptThing* thing, const C3DVector& position)
    : CBase()
    , m_pThing(thing)
    , m_positionX(position.X), m_positionY(position.Y), m_positionZ(position.Z)
    , m_sounds()
    , m_active(true), m_paused(false), m_finished(false)
    , m_scriptCounter(m_pThing->DecrementScriptCounter())
{
}
