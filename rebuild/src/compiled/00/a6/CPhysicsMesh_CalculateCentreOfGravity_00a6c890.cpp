// CPhysicsMesh::CalculateCentreOfGravity() -- zero the centre of gravity, sum every mesh point into it with for_each(CAddElements), then scale it by 1 / point count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6C890, length 110 bytes (+ int3 padding).
// Leaf: CalculateCentreOfGravity
// Callees: for_each<C3DVector*, CAddElements<C3DVector>> @ 0x00A6ECE0 (`__fastcall`: hidden result in ecx -- the
// `push ecx` frame word --, first in edx, last and the functor by value on the stack).
// Constants: 4294967296.0f @ 0x0122DCB4 (unsigned-to-float fix-up), 1.0f @ 0x0122DED8.
// Layout: Ego_r.pdb CPhysicsMesh with the retail 12-byte CArray: TotalPoints +0x3c/+0x40 (12-byte C3DVector elements),
// CentreOfGravity +0xa8 (PDB +0xc4).
// Shape: the centre is a reference local (`lea esi,[edi+0A8h]` before the stores; naming the member folds the offsets);
// the zero stores run z, y, x (`x = y = z = 0` through one register); the count is the unsigned element count
// (`/12` by the 0x2AAAAAAB multiply) converted with the 2^32 fix-up; `1.0f / count` is kept on the x87 stack and
// multiplied into x, y, z in place; `ret`.
class C3DVector { public:
    float x, y, z;
    C3DVector& operator*=(float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
};

template <class T>
class CAddElements { public:
    T* Sum;
    CAddElements(T& sum) : Sum(&sum) { }
    void operator()(const T& value) { Sum->x += value.x; Sum->y += value.y; Sum->z += value.z; }
};
template <class It, class F> F __fastcall for_each(It first, It last, F f);   // 0x00A6ECE0 for (C3DVector*, CAddElements<C3DVector>)

template <class T>
class CArray { public:                                    // retail CArray<T>: first / last / end
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
};

class CPhysicsMesh { public:
    unsigned char      _pad00[0x3c];
    CArray<C3DVector>  TotalPoints;                // +0x3c
    unsigned char      _pad48[0xa8 - 0x48];
    C3DVector          CentreOfGravity;            // +0xa8
    void CalculateCentreOfGravity();
};

void CPhysicsMesh::CalculateCentreOfGravity()
{
    C3DVector& centre = CentreOfGravity;
    centre.x = centre.y = centre.z = 0.0f;
    for_each(TotalPoints.begin(), TotalPoints.end(), CAddElements<C3DVector>(centre));
    centre *= 1.0f / TotalPoints.size();
}