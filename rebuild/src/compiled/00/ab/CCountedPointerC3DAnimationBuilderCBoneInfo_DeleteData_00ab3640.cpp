// CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData(void* object) -- the per-type deleter the count block stores: destroy the bone info and free it (null-guarded `delete`).
// Leaf: ?DeleteData@?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB3640; static __fastcall, object in ecx. Ego_r.pdb name (chunk 685: the deleter
//   C3DAnimationBuilder::BuildBoneInfoTree 0x00AB1F30 stores in the count block, see the landed Reset 0x00AB36B0); the catalog's
//   CTCMapwho::OnDie / DeleteObject_00ab3640 were propagated labels of the shared `delete p` shape (naming wave 19).
// Callees: ??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ 0x00AB34C0 (FableWin.pdb), operator delete 0x00BFE9BC.
class C3DAnimationBuilder { public:
    class CBoneInfo { public:
        ~CBoneInfo();   // 0x00AB34C0
    };
};
void operator delete(void*);   // 0x00bfe9bc
template <class T> class CCountedPointer { public:
    static void __fastcall DeleteData(void* object);
};
template <class T> void __fastcall CCountedPointer<T>::DeleteData(void* object)
{
    delete (T*)object;
}
template class CCountedPointer<C3DAnimationBuilder::CBoneInfo>;
