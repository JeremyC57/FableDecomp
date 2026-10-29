// CSoundEmitterLH::CSoundEmitterLH(CSoundManagerLH* manager, const C3DVector& position) -- CBase, the owning manager at +4, this class's vtable, the position at +8..+0x10, an empty retail list at +0x14 (its sentinel node malloc'd inline), three flag bytes, the manager's next emitter number at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC580, length 94 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, malloc @ 0x00BFEA0E (the 16-byte sentinel), the manager's counter @ 0x00A3E420 (`return Counter++;`
// on the manager's +0x50, descriptive name; its result at +0x1c).
// Owner: the only caller is CSoundManagerLH vtable slot 4 (0x00A3ECB0, vtable 0x0129D2C4), which passes its own `this` as the
// first argument (`push edi` = the manager) after its slot-6 test; 0x00A3E420 reads and bumps the manager's +0x50, beside the
// manager's own slot 1 0x00A3E430 (FINDINGS chunk 418). The catalog called the owner CGameScriptThing and the counter
// DecrementScriptCounter (a propagated label; the body increments).
// Vtable 0x0129E6EC = CSoundEmitterLH (rtti-vtables.tsv). Shape: the VC6 list constructor inlined (`mov [esi+14h],0 ; push 10h ; call malloc ; mov [eax],eax ;
// mov [eax+4],eax ; mov [esi+14h],eax`), the three bytes stored around the `add esp,4`, the manager re-read from +4 for the last call.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class C3DVector { public: float X, Y, Z; };

struct CSoundManagerLH {
    long TakeNextEmitterNumber();                  // 0x00A3E420 (descriptive): return EmitterCounter++ (+0x50)
};

class CBaseClass {
public:
    virtual void v00();
    CBaseClass();                                       // 0x0099A2D0
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

struct CSoundEmitterLH : CBaseClass {                   // vtable 0x0129E6EC
    CSoundManagerLH*     m_pManager;               // +0x04
    float                m_positionX;              // +0x08 (three floats initialised one by one)
    float                m_positionY;              // +0x0c
    float                m_positionZ;              // +0x10
    CRetailList_00aac580 m_sounds;                 // +0x14
    bool                 m_active;                 // +0x18
    bool                 m_paused;                 // +0x19
    bool                 m_finished;               // +0x1a
    long                 m_number;          // +0x1c (descriptive)
    virtual void v00();
    CSoundEmitterLH(CSoundManagerLH* manager, const C3DVector& position);
};

CSoundEmitterLH::CSoundEmitterLH(CSoundManagerLH* manager, const C3DVector& position)
    : CBaseClass()
    , m_pManager(manager)
    , m_positionX(position.X), m_positionY(position.Y), m_positionZ(position.Z)
    , m_sounds()
    , m_active(true), m_paused(false), m_finished(false)
    , m_number(m_pManager->TakeNextEmitterNumber())
{
}