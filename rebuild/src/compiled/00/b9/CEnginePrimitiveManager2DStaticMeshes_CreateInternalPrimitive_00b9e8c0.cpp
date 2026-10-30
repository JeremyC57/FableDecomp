// CEnginePrimitiveManager2DStaticMeshes::CreateInternalPrimitive(long type) -- only for type 0x24: allocates and constructs a 2D static mesh primitive (0x130 bytes); null otherwise or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9E8C0, length 136 bytes (144 with padding).
// Callees: operator new @ 0x00BFEA1A, CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long flags)
// @ 0x00B8FF00 (with (0x24, 0x80000000)), CBaseClass::CBaseClass @ 0x0099A2D0 (the orientation interpolator's base).
// Vtables (rtti-vtables.tsv): 0x012A42E4 = CEngineInternalPrimitive2DStaticMesh, 0x01239824 = C3DOrientationInterpolator.
// Shape: the type test returns 0 before any register is saved; the inline constructor runs the base constructor, an
// intermediate `__declspec(novtable)` base's initialisers (+0x48/+0x4C/+0x50, +0x64/+0x68 words and the +0x6C..+0x6E bytes,
// all from one `xor ebx,ebx` zero register -- as initialisers of the derived class they sink below the vtable store),
// the derived vtable store, then the member objects in declaration order: a four-word block at +0xC4 with an inline
// zeroing constructor (stored after the vtable) and the orientation interpolator at +0xD4 (CBaseClass call + its vtable).
// The constructors are `__forceinline` (VC7.1 keeps a constructor of this size out of line). Member names are descriptive
// (no PDB layout for the 2D static mesh primitive).
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class C3DOrientationInterpolator : public CBaseClass { public:   // vtable 0x01239824
    unsigned char _body[0x30];
    __forceinline C3DOrientationInterpolator() { }
};

struct SFourWords {                                // four words cleared by an inline constructor
    long m_words[4];
    __forceinline SFourWords() { m_words[0] = 0; m_words[1] = 0; m_words[2] = 0; m_words[3] = 0; }
};

struct __declspec(novtable) CEngineInternalPrimitiveStaticMeshBase : CEngineInternalPrimitiveBase {   // intermediate base
    long          m_mesh;                          // +0x48
    long          m_material;                      // +0x4C
    long          m_animation;                     // +0x50
    unsigned char _pad54[0x10];
    long          m_frame;                         // +0x64
    long          m_flags;                         // +0x68
    bool          m_visible;                       // +0x6C
    bool          m_lit;                           // +0x6D
    bool          m_dirty;                         // +0x6E
    unsigned char _pad6f[0x55];
    __forceinline CEngineInternalPrimitiveStaticMeshBase(long type, unsigned long flags)
        : CEngineInternalPrimitiveBase(type, flags), m_mesh(0), m_material(0), m_animation(0), m_frame(0), m_flags(0),
          m_visible(false), m_lit(false), m_dirty(false) { }
};

struct CEngineInternalPrimitive2DStaticMesh : CEngineInternalPrimitiveStaticMeshBase {   // vtable 0x012A42E4, 0x130 bytes
    SFourWords                 m_bounds;           // +0xC4
    C3DOrientationInterpolator m_orientation;      // +0xD4
    unsigned char              _rest[0x130 - 0xd4 - sizeof(C3DOrientationInterpolator)];
    virtual void v00();
    __forceinline CEngineInternalPrimitive2DStaticMesh() : CEngineInternalPrimitiveStaticMeshBase(0x24, 0x80000000) { }
};

class CEnginePrimitiveManager2DStaticMeshes { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManager2DStaticMeshes::CreateInternalPrimitive(long type)
{
    if (type != 0x24)
        return 0;
    return new CEngineInternalPrimitive2DStaticMesh;
}