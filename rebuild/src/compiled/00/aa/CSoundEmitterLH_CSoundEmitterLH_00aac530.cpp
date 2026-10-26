// CSoundEmitterLH::CSoundEmitterLH(CGameScriptThing* thing) -- CBaseClass, the thing at +4, this class's vtable, an empty retail list at +0x14 (its sentinel node malloc'd inline), three cleared flag bytes, the thing's script counter value at +0x1c; the position (+8..+0x10) is left unset
// Leaf: ??0CSoundEmitterLH
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC530, length 72 bytes (+8 int3), `ret 4`.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, malloc @ 0x00BFEA0E (the 16-byte list sentinel),
//   CGameScriptThing::DecrementScriptCounter @ 0x00A3E420 (post-increments the thing's counter; result at +0x1c).
// Identity: vtable 0x0129E6EC = CSoundEmitterLH (rtti-vtables.tsv); the one-argument sibling of the landed
//   CSoundEmitterLH(thing, position) @ 0x00AAC580 (same model). The catalog label
//   "NHeroInformationScreens_CBase_Constructor" comes from the propagated CBase name of the 0x0099A2D0 call.
// Shape: the three flag bytes are all false here, so VC shares one zero register (ebx) between the list head's
//   pre-allocation zero store and the three byte stores; the position members get no initialiser (no stores).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CGameScriptThing { public:
    long DecrementScriptCounter();                 // 0x00A3E420
};

class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};

struct SListNode_00aac530 { SListNode_00aac530* m_pNext; SListNode_00aac530* m_pPrev; long m_a; long m_b; };   // 16 bytes

struct CRetailList_00aac530 {                      // VC6 Dinkumware list: the sentinel node is malloc'd by the constructor
    SListNode_00aac530* m_pHead;
    CRetailList_00aac530()
    {
        m_pHead = 0;
        SListNode_00aac530* node = (SListNode_00aac530*)malloc(sizeof(SListNode_00aac530));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
};

struct CSoundEmitterLH : CBaseClass {              // vtable 0x0129E6EC
    CGameScriptThing*    m_pThing;                 // +0x04
    float                m_positionX;              // +0x08 (not set by this constructor)
    float                m_positionY;              // +0x0c
    float                m_positionZ;              // +0x10
    CRetailList_00aac530 m_sounds;                 // +0x14
    bool                 m_active;                 // +0x18
    bool                 m_paused;                 // +0x19
    bool                 m_finished;               // +0x1a
    long                 m_scriptCounter;          // +0x1c
    virtual ~CSoundEmitterLH();
    CSoundEmitterLH(CGameScriptThing* thing);
};

CSoundEmitterLH::CSoundEmitterLH(CGameScriptThing* thing)
    : CBaseClass()
    , m_pThing(thing)
    , m_sounds()
    , m_active(false), m_paused(false), m_finished(false)
    , m_scriptCounter(m_pThing->DecrementScriptCounter())
{
}