// _global::GFSerialiseVectorBinaryIn<CActionInputControl>(CMemoryDataInputStream& stream, std::vector<CActionInputControl>& out) -- read the element count (inline checked 4-byte read), resize with default bindings, then TransferBinaryIn every element
// Leaf: ??$GFSerialiseVectorBinaryIn@VCActionInputControl@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412C70, 171 bytes (+5 int3 pad). Callees:
// CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path), vector<CActionInputControl>::
// resize(n, const T&) 0x004129F0 (manifest label vector<CBrainBehaviour>::resize, propagated),
// CActionInputControl::TransferBinaryIn 0x00410120.
//
// Retail bytes:  sub esp,0x20 ; push ebx ; push ebp ; push esi ; mov esi,ecx ; mov eax,[esi+4] ;
//   push edi ; lea ecx,[eax+4] ; xor edi,edi ; xor eax,eax ; <__int64 range check> ;
//   <fast read into ebx | ReadWithSrcChunkOverflow(&count, 4)> ; lea ecx,[temp] ; push ecx ;
//   push ebx ; mov ecx,ebp ; <temp = CActionInputControl(): five `mov [..],edi`, two float zero
//   immediates> ; call resize ; cmp ebx,edi ; jle X ; L: mov ecx,[ebp] ; push esi ; add ecx,edi ;
//   call TransferBinaryIn ; add edi,0x1c ; dec ebx ; jne L ; X: ... add esp,0x20 ; ret
// Identity: FableWin.pdb `??$GFSerialiseVectorBinaryIn@VCActionInputControl@@@@YIXAAVCMemoryDataInputStream@@...`;
// the manifest's <CBrainUpdateZone> is propagated: the element is 28 bytes (edi += 0x1C), its
// TransferBinaryIn is CActionInputControl's 0x00410120, and the only caller is the
// CPersistContext::TransferVector<CActionInputControl> body at 0x00412DB0 whose binary-out arm calls
// GFSerialiseVectorBinaryOut<CActionInputControl> 0x0040FCA0. CPersistTraits<T>::TransferBinaryIn
// forwards to the element's own member (inline, so the block-scoped traits object vanishes); the
// default binding is the zeroing user constructor (Ego_r.pdb layout: five enum longs, C2DVector +0x14).
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (CDataInputStream)
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

template <class T>
class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void resize(unsigned int newSize, const T& value);   // 0x00412A50 / 0x004129F0 (out of line per T)
    void resize(unsigned int newSize) { resize(newSize, T()); }
};

struct C2DVector {
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};

class CActionInputControl {
public:
    long GameAction;
    long ControllerType;
    long KeyboardKey;
    long XboxButton;
    long MouseButton;
    C2DVector ControlDirection;
    CActionInputControl() : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0), ControlDirection(0.0f, 0.0f) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream);   // 0x00410120
};

template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const { out->TransferBinaryIn(stream); }
};

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, CRetailVector<T>& out)
{
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    {
        CPersistTraits<T> traits;
        for (long i = 0; i < count; ++i)
            traits.TransferBinaryIn(stream, &out[i]);
    }
}
template void __fastcall GFSerialiseVectorBinaryIn<CActionInputControl>(CMemoryDataInputStreamReader& stream, CRetailVector<CActionInputControl>& out);