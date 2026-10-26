// C3DPrimitive2::GetUsedBones(CArray<bool>& used) -- collect the bones used by the device-independent data and by every cloth primitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8FCF0, oracle length 63 bytes.
// Callees: N3DPrimitive::CDeviceIndependent::GetUsedBones(CArray<bool>&) @ 0x00A8EF50 (manifest label
//          "CMeshDataBank::FreeDeviceSpecificResources" is propagated; it is called on the Data pointer with the
//          array), C3DClothPrimitive::GetUsedBones(CArray<bool>&) @ 0x00AC3AE0 (manifest label `_Cons_val<...>` propagated;
//          called on each cloth primitive's object).
//
// Retail bytes:  push ebx ; mov ebx,[esp+8] ; push esi ; push edi ; mov esi,ecx ; mov ecx,[esi+2Ch] ; push ebx ; call Data->GetUsedBones ;
//                mov eax,[esi+24h] ; sub eax,[esi+20h] ; sar eax,3 ; xor edi,edi ; test eax,eax ; jbe E ; L: mov ecx,[esi+20h] ;
//                mov ecx,[ecx+edi*8] ; push ebx ; call Cloth->GetUsedBones ; mov edx,[esi+24h] ; sub edx,[esi+20h] ; inc edi ; sar edx,3 ;
//                cmp edi,edx ; jb L ; E: pop edi ; pop esi ; pop ebx ; ret 4
// Retail layout (PDB struct_layouts_egor.tsv with the 12-byte retail CArray): +0x20 ClothPrimitives =
// CArray<CCountedPointer<C3DClothPrimitive>> {first, last, end}, 8-byte elements {object, count block};
// +0x2C Data (scoped_ptr<N3DPrimitive::CDeviceIndependent>). The loop re-evaluates Size() = (last - first) / 8
// every iteration with an unsigned index.
struct CBoolArray;                         // CArray<bool>

struct CDeviceIndependent {
    void GetUsedBones(CBoolArray& used);   // 0x00A8EF50
};

class C3DClothPrimitive { public:
    void GetUsedBones(CBoolArray& used);   // 0x00AC3AE0
};

struct CCountedClothPointer {              // CCountedPointer<C3DClothPrimitive>
    C3DClothPrimitive* m_pObject;
    void*              m_pCount;
};

struct CClothArray {                       // retail CArray<CCountedPointer<C3DClothPrimitive>>
    CCountedClothPointer* m_pFirst;
    CCountedClothPointer* m_pLast;
    CCountedClothPointer* m_pEnd;
    unsigned long Size() const { return m_pLast - m_pFirst; }
    CCountedClothPointer& operator[](unsigned long i) { return m_pFirst[i]; }
};

class C3DPrimitive2 { public:
    virtual void v00();
    unsigned char       _pad_0x04[0x20 - 0x04];
    CClothArray         ClothPrimitives;   // +0x20
    CDeviceIndependent* Data;              // +0x2C

    void GetUsedBones(CBoolArray& used);
};

void C3DPrimitive2::GetUsedBones(CBoolArray& used)
{
    Data->GetUsedBones(used);
    for (unsigned long i = 0; i < ClothPrimitives.Size(); ++i)
        ClothPrimitives[i].m_pObject->GetUsedBones(used);
}