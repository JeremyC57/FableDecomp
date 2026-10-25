// CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::Reset(NPlayerGui::CDrawMeleeActionButtonEffects* data) -- drop the shared count block (destroying the object on last
// reference), then adopt data with a fresh count block {1, DeleteData, data}.
// Leaf: ?Reset@?$CCountedPointer@VCDrawMeleeActionButtonEffects@NPlayerGui@@@@QAEXPA
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00444b40, oracle length 103 bytes, thiscall `ret 4`.
// Renamed (rename1, 2026-10-24) from CCountedPointer_00443aa0::Reset @ 00/44/CCountedPointer00443aa0_Reset_00444b40.cpp: the deleter this body stores into the
//   count block (0x00443AA0) calls the landed ~NPlayerGui::CDrawMeleeActionButtonEffects and is CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::DeleteData
//   (PDB name set); the PDB name set also has `?Reset@?$CCountedPointer@VCDrawMeleeActionButtonEffects@NPlayerGui@@@@QAEXPAV...@Z`, and the PDB
//   layout is CCountedPointer {T* Data +0; CCPPointerInfo* Info +4}, CCPPointerInfo {ulong RefCount +0;
//   DeleteFunc +4; void* Data +8}. The manifest's CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset is propagated.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je A ; dec dword ptr [eax] ; mov eax,[esi+4] ;
//   cmp dword ptr [eax],0 ; jne A ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ;
//   add esp,4 ; A: mov eax,[esp+8] ; test eax,eax ; mov dword ptr [esi+4],0 ; mov [esi],eax ; je Z ; push 0Ch ;
//   call operator new ; ... mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset DeleteData ; mov [eax+8],ecx ; ...
// Release() is inlined (the count is re-read after the decrement); Data is re-read through `this` for the block.
namespace NPlayerGui {
class CDrawMeleeActionButtonEffects;
}

struct CCPPointerInfo {
    unsigned long RefCount;                       // +0
    void (__fastcall* DeleteFunc)(void* data);    // +4
    void* Data;                                   // +8
    CCPPointerInfo(unsigned long ref_count, void (__fastcall* delete_func)(void*), void* data)
        : RefCount(ref_count), DeleteFunc(delete_func), Data(data) { }
};

template <class T> class CCountedPointer {
public:
    T*              Data;                         // +0
    CCPPointerInfo* Info;                         // +4
    static void __fastcall DeleteData(void* data);   // 0x00443AA0
    void Reset(T* data);
};

template <class T> void CCountedPointer<T>::Reset(T* data)
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Info = 0;
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, &DeleteData, Data);   // re-read through `this` (retail loads [esi]), not the argument
}

template void CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::Reset(NPlayerGui::CDrawMeleeActionButtonEffects* data);
