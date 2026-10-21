// CActionInputControl::TransferBinaryIn(CMemoryDataInputStream& stream) -- read one control binding: game action, controller type, keyboard key, Xbox button, mouse button and the 2D control direction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410120, 468 bytes; `ret 4`. Callees: CDataInputStream::ReadWithSrcChunkOverflow
//          @ 0x00993CA0, CMemoryDataInputStream::ReadC2DVector @ 0x00995030 (by-value C2DVector through the hidden
//          pointer; manifest label "CPersistTraits<CFloatRange>::TransferBinaryIn" is propagated, the body reads two floats,
//          as in the landed 0x005DBDA0).
// Layout: Ego_r.pdb CActionInputControl (0x1C): GameAction +0, ControllerType +4, KeyboardKey +8, XboxButton +0xC,
//         MouseButton +0x10 (all 4-byte enums), ControlDirection +0x14 (C2DVector).
// Shape: five inlined CMemoryDataInputStream::Read calls of 4 bytes (64-bit position range check, in-chunk copy or
//        the overflow reader), each value homed in the dead stream-argument slot through a by-value inline helper;
//        the direction is assigned straight from the by-value call (8-byte temporary in the frame).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct C2DVector {
    float X, Y;
    C2DVector() {}                              // non-POD: returned through a hidden pointer
};

struct CMemoryDataInputStream {
    void* m_pVtable;                            // +0x00
    unsigned long m_position;                   // +0x04
    long m_08;
    const unsigned char* m_pData;               // +0x0c current chunk read pointer
    long m_10;
    long m_remaining;                           // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    C2DVector ReadC2DVector();                  // 0x00995030
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long v; Read(&v, sizeof(v)); return v; }
};

enum EGameAction { GAME_ACTION_NONE };
enum EControllerType { CONTROLLER_TYPE_NONE };
enum EInputKey { INPUT_KEY_NONE };
enum EXboxControllerButton { XBOX_BUTTON_NONE };
enum EMouseButtonControl { MOUSE_BUTTON_NONE };

struct CActionInputControl {
    EGameAction           GameAction;           // +0x00
    EControllerType       ControllerType;       // +0x04
    EInputKey             KeyboardKey;          // +0x08
    EXboxControllerButton XboxButton;           // +0x0C
    EMouseButtonControl   MouseButton;          // +0x10
    C2DVector             ControlDirection;     // +0x14
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CActionInputControl::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    GameAction = (EGameAction)stream.ReadLong();
    ControllerType = (EControllerType)stream.ReadLong();
    KeyboardKey = (EInputKey)stream.ReadLong();
    XboxButton = (EXboxControllerButton)stream.ReadLong();
    MouseButton = (EMouseButtonControl)stream.ReadLong();
    ControlDirection = stream.ReadC2DVector();
}