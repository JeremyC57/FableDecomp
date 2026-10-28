// Behaviour test for 0x00A2E9A0 C3DMeshVolume::C3DMeshVolume(const C3DMeshFileVolumeChunk&, CMovableResourceMemoryPool*)
#include <cstdio>
#include <cstring>
class CCharString { public:
    char m_text[16];
    CCharString() { m_text[0] = 0; }
    const CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
};
class CPlane { public: float A, B, C, D; };
class CMovableResourceMemoryPool { public: int id; };
class CContainmentVolume { public:
    const CPlane* planes; unsigned long count; CMovableResourceMemoryPool* pool;
    CContainmentVolume() : planes(0), count(0), pool(0) {}
    bool Initialise(const CPlane* p, unsigned long n, CMovableResourceMemoryPool* m) { planes = p; count = n; pool = m; return true; }
};
template<class T> class CCountedPointer { public:
    T* m_pObject; void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    ~CCountedPointer() { delete m_pObject; }
    void Reset(T* o) { delete m_pObject; m_pObject = o; }
    T* operator->() const { return m_pObject; }
};
template<class T> class CArray { public:
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T* GetData() const { return Size() > 0 ? m_begin : (const T*)this; }
};
struct C3DMeshFileVolumeChunk { CCharString Name; CArray<CPlane> Planes; };
class C3DMeshVolume { public:
    CCharString Name; CCountedPointer<CContainmentVolume> ConvexVolume;
    C3DMeshVolume(const C3DMeshFileVolumeChunk& chunk, CMovableResourceMemoryPool* pool)
    {
        Name = chunk.Name;
        ConvexVolume.Reset(new CContainmentVolume);
        ConvexVolume->Initialise(chunk.Planes.GetData(), chunk.Planes.Size(), pool);
    }
};
int main()
{
    CPlane planes[3];
    C3DMeshFileVolumeChunk chunk; std::strcpy(chunk.Name.m_text, "HULL");
    chunk.Planes.m_begin = planes; chunk.Planes.m_end = planes + 3; chunk.Planes.m_capacity = planes + 3;
    CMovableResourceMemoryPool pool;
    C3DMeshVolume v(chunk, &pool);
    bool ok = std::strcmp(v.Name.m_text, "HULL") == 0 && v.ConvexVolume.m_pObject != 0 && v.ConvexVolume->planes == planes
        && v.ConvexVolume->count == 3 && v.ConvexVolume->pool == &pool;
    if (!ok) { std::printf("00a2e9a0_TEST FAIL\n"); return 1; }
    std::printf("00a2e9a0_TEST PASS\n");
    return 0;
}