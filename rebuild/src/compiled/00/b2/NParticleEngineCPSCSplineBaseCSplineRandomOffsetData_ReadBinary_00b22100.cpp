// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::ReadBinary(CDataInputStream& in) -- read the two flags and the offset count, size and fill the offset limit/speed arrays, then either snapshot the current spline as the previous one or clear the previous spline's points
// Leaf: ReadBinary@CSplineRandomOffsetData
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B22100, 528 bytes, `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of each inlined Read),
//   CArray<C3DVector>::resize(n, const C3DVector&) 0x0075C040, CDataInputStream::Read3DVector 0x009950F0 (C3DVector by
//   hidden pointer), CArray<C3DVector>::operator= 0x0075B8D0 (inside CCardinalSpline's implicit operator=).
// Layout: as the landed ctor 0x00B21DD0 (retail 12-byte STLport vectors): ThisSpline +4, PreviousSpline +0x18,
//   OffsetOnlyAtStartup +0x2C, ResetEachFrame +0x2D, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3C.
// Shape: inlined ReadBool/ReadULong (the CSplinePosParamData::ReadBinary 0x00B21C90 family), both resizes share one
//   uninitialised C3DVector() slot, the loop stores through `*(begin() + i)` (the array start is loaded before the
//   Read3DVector call), and the else arm is the inline STLport clear() = erase(begin(), end()) whose copy loop runs
//   over the empty [finish, finish) range.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
};

class CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14
    C3DVector Read3DVector();                                          // 0x009950F0
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
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

template <class T> class CArray { public:              // STLport vector<T> (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    CArray& operator=(const CArray& other);                                  // 0x0075B8D0
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());  // 0x0075C040
    static __forceinline T* copy(T* first, T* last, T* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    __forceinline T* erase(T* first, T* last)
    {
        T* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    __forceinline void clear() { erase(begin(), end()); }
};

class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;    // +0x00
    float             Tension;          // +0x0C
    bool              EndPointsConnected; // +0x10
};

namespace NParticleEngine {
class CPSCSplineBase { public:
    class CSplineRandomOffsetData { public:
        CCardinalSpline*  InitialSpline;          // +0x00
        CCardinalSpline   ThisSpline;             // +0x04
        CCardinalSpline   PreviousSpline;         // +0x18
        bool              OffsetOnlyAtStartup;    // +0x2C
        bool              ResetEachFrame;         // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;  // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;   // +0x3C
        void ReadBinary(CDataInputStream& in);
    };
};

void CPSCSplineBase::CSplineRandomOffsetData::ReadBinary(CDataInputStream& in)
{
    OffsetOnlyAtStartup = in.ReadBool();
    ResetEachFrame = in.ReadBool();
    unsigned long count = in.ReadULong();
    SplinePosOffsetLimits.resize(count);
    SplinePosOffsetSpeed.resize(count);
    for (unsigned long i = 0; i < count; ++i) {
        SplinePosOffsetLimits[i] = in.Read3DVector();
        SplinePosOffsetSpeed[i] = in.Read3DVector();
    }
    if (OffsetOnlyAtStartup)
        PreviousSpline = ThisSpline;
    else
        PreviousSpline.ControlPoints.clear();
}
}