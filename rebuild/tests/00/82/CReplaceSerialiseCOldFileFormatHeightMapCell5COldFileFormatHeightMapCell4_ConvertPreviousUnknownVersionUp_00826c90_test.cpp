#include <assert.h>
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
#define FABLE_STATIC_ASSERT(x)
// This fixture tests typed field conversion and ownership. Native packed offsets
// are checked by the production-header compiler assertions and byte comparison.
#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H
// Retail model (hand-written 2026-10-30, class-model migration wave 9): the engine's reference-counted owning pointer and
// its shared count block. Not polymorphic (no vtable, no RTTI); every member is inline in the release build.
//
// CCPPointerInfo (Ego_r.pdb, 12 bytes): RefCount +0x0 (ulong), DeleteFunc +0x4, Data +0x8 (void*). DeleteFunc is
//   __fastcall: every retail release loads `mov ecx,[eax+8]; call [eax+4]` (0x006E7AB0, 0x00CE1000, 0x00CE1040, 0x008AB2B0)
//   and the deleters it points at are `SIXPAX@Z` (static __fastcall void(void*)) CCountedPointer<T>::DeleteData in
//   FableWin.pdb (343 instantiations); Ego_r.pdb's `_func___cdecl_void_void_ptr` is the PDB's generic spelling.
//   Ctor: FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z = (unsigned long, void (__fastcall*)(void*), void*); retail
//   Reset 0x008AB2B0 expands it after `new(0xc)`: [eax]=1, [eax+4]=deleter, [eax+8]=Data (member order).
//
// CCountedPointer<T> (Ego_r.pdb, 8 bytes for every T): Data +0x0 (T*), Info +0x4 (CCPPointerInfo*). Member set = FableWin.pdb
//   (the debug build emits them; Ego_r keeps out of line only ShareData / ReleaseData / Reset / DeleteData, the rest inline):
//   ??0 (), (T*), (const&), member template ??$?0 (const CCountedPointer<U>&); ??1; ??4 (const&), member template ??$?4;
//   ??C operator-> QBE, ??D operator* QBE; DeleteData SIXPAX@Z; Get / get QBE; GetReferenceCount QBEKXZ; IsNull QBE_NXZ;
//   ReleaseData QAEXXZ; Reset / reset QAEXPAV; ShareData QAEXPAV..PAVCCPPointerInfo@@@Z; Validate QBEXXZ (debug check);
//   member template ??$SetStaticCast (63). Not modelled: the free __fastcall templates ??$?8 / ??$?9 (operator== / !=
//   of two CCountedPointers, YI_NABV..0@Z) -- no landed includer compares two pointers.
// Bodies (retail bytes; no ICF in retail: 732 separate /O2 dtor COMDATs, one per T):
//   ~CCountedPointer = 0x006E7AB0 (MovieBase) / 0x00CE1000 (CEntityScriptBase) and the other 730: `if (Info) { dec [Info];
//     reload Info; cmp [Info],0; jne; ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }`, then Data = 0
//     and Info = 0 in that order. It is its own body, not a ReleaseData call (below).
//   ReleaseData: the same release, then Info = 0 and Data = 0 in THAT order. Retail never emits it out of line (no
//     release COMDAT with the Info-first tail exists); expanded at 0x00A45330 / 0x00A5F250 / 0x006633A0 (`mov [x+4],edi;
//     mov [x],edi`). Reset and ShareData are written over it (their dead zero stores vanish):
//   Reset(T*) = 0x008AB2B0 (and 197 more): <ReleaseData> leaves only `mov [esi+4],0`, then Data = data; if (data)
//     Info = new CCPPointerInfo(1, DeleteData, Data) (the block reads Data back; a failed new leaves Info 0).
//   ShareData = 0x00CE1040 (CCountedPointer<CEntityScriptBase>: every landed caller binds that T, FableWin has the symbol; the method name is Ego_r.pdb's): `if (Info != info) { <release>; Data = data;
//     Info = info; if (info) ++info->RefCount; }`.
//   operator= = 0x00704580 (CScriptThing): loads o.Data / o.Info first, then the ShareData body, returns *this.
//   The (T*) ctor is the allocation without the release (out of line at 0x008C58F0 / 0x008C5980, `ret 4`).
//   Copy ctor: Data, Info copied, then `if (Info) ++Info->RefCount` (expanded in every CScriptThing copy 0x004ABE90).
//   DeleteData = `delete static_cast<T*>(data)`: 0x00CDEE00 (polymorphic T: `test ecx,ecx; je; mov eax,[ecx]; push 1;
//     call [eax]`), 0x00674E80 (CExpressionSet: the inline implicit dtor, then operator delete). Only its address is
//     stored (the count block); the landed DeleteData rows explicitly instantiate it.
// Model test (wave 9, 955 landed rows, logs/mc_cpvar_*.txt): this split (dtor own body, ReleaseData zeroing Info first)
//   gives 15 DIFFER; a dtor that calls a zeroing ReleaseData gives the same leaf bytes at more sites but names ReleaseData
//   where retail calls the Data-first dtor COMDAT, and `ReleaseData(); Data = 0; Info = 0;` gives 20 DIFFER.
// Inlining: every member is plain `inline` (in-class), never __forceinline / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from these bodies on the landed
//   rows (wave 9 HEADERS.md lists the shapes it does not reproduce). unproven: the source order of Reset's two stores and
//   the bodies of GetReferenceCount / reset / Get / get / IsNull (FableWin names only, no retail out-of-line copy found).

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

template <class T> class CCountedPointer {
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

    CCountedPointer() : Data(0), Info(0) { }
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR) && !defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR)
    // Chunk 764 (row 0x0096DBC0 CCreatureAction_ScreamerIntoAggressive::Initialise): there retail calls the (T*) constructor's COMDAT
    // (0x00718A00, ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z) where VC7.1
    // expands the in-class body; the landed same-module rows (0x008C1A10, 0x008C1900) declare it without a body. Declaration only.
    CCountedPointer(T* data);
#else
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR) && defined(_MSC_VER)
    // Inline-budget (row 0x0041E6D3 NUISystem::CManager::Action): retail CALLS this ctor's same-TU COMDATs at every site
    // (CComponent 0x00429C15 x25, CAction 0x0042AA29 x17, CTexture 0x0041C030 in LOAD_PHOTO; body = the default one below:
    // 0x0041C030 `test eax,eax; mov [esi],eax; je` then `new CCPPointerInfo(1, DeleteData, Data)` reading the stored member).
    // A declaration-only ctor (FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR alone) exposes each object's address above c2's
    // 10000-tuple limit, so it never shares a frame slot (LOAD_PHOTO's texture: ours -3C8h alone, retail -268h shared).
    // A visible body that is not expanded keeps the object tracked. Default path unchanged.
    __declspec(noinline)
#endif
    CCountedPointer(T* data) : Data(data)
    {
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER
        // Retail (Codex pass 11): the ctor tests the member it just stored -- 0x00626E40 spills the pointer and reloads it
        // for the test and the CCPPointerInfo store, which `if (data)` (the landed default) does not reproduce.
        if (Data)
#else
        if (data)
#endif
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
#endif
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
    // Per-TU opt-in (Codex name fix 2026-11-10, ~CCountedPointer<NUISystem::CComponent> 0x004291DE): where retail calls the
    // destructor COMDAT from a speed-optimised TU (0x00429E16, the map pair destructor) the body above would be expanded.
    // Declaration only; the retail body is the #else one (0x004291DE: Data zeroed before Info).
    ~CCountedPointer();
#else
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
#endif
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   // unproven body (FableWin name only)
    void Validate() const;                                               // FableWin.pdb only (debug build check)
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR
    // Per-TU opt-in (Codex name fix 2026-11-10, row 0x00429C6F Reset<NUISystem::CComponent>, size TU): retail's out-of-line Reset
    // calls 0x004291DE, the Data-first release body that Ego_r.pdb names ??1?$CCountedPointer@VCComponent@NUISystem@@@@ (the
    // only out-of-line release that PDB has for this T: no ReleaseData). The release is spelled as that destructor; the
    // original spelling of the call is unproven.
    void Reset(T* data)
    {
        this->~CCountedPointer();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
#else
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
#endif
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   // FableWin.pdb ??$SetStaticCast@..@?$CCountedPointer@..@@QAEXABV?$CCountedPointer@..@@@Z (63)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              // landed 0x00A32620 (C3DMeshFile::GetRoot) expands it
    }
    void reset(T* data) { Reset(data); }                                 // unproven body (FableWin name only)
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    // per-T deleter, address stored in the count block
};

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H

template<class T,unsigned short Version>class CBaseSerialise {public:
 unsigned long ByteLength;unsigned char SerialiseVersionNumber;
 CBaseSerialise():SerialiseVersionNumber(Version){}
 static unsigned short FABLE_FASTCALL GetClassVersionNumber(){return Version;}
};
template<class T,class Old>class CReplaceSerialise {public:
 unsigned long ByteLength;unsigned char SerialiseVersionNumber;
 static unsigned short FABLE_FASTCALL GetClassVersionNumber(){return (unsigned short)(Old::GetClassVersionNumber()+1);}
 static CCountedPointer<T> FABLE_FASTCALL ConvertPreviousUnknownVersionUp(void* old_void);
};
class COldFileFormatHeightMapCell1:public CBaseSerialise<COldFileFormatHeightMapCell1,1>{public:
 float Height;
};
class COldFileFormatHeightMapCell2:public CReplaceSerialise<COldFileFormatHeightMapCell2,COldFileFormatHeightMapCell1>{public:
 float Height;unsigned char TextureBlendAlpha;
};
class COldFileFormatHeightMapCell3:public CReplaceSerialise<COldFileFormatHeightMapCell3,COldFileFormatHeightMapCell2>{public:
 float Height;unsigned char TextureBlendAlpha,EngineTheme[3],EngineBlend[2];
};
class COldFileFormatHeightMapCell4:public CReplaceSerialise<COldFileFormatHeightMapCell4,COldFileFormatHeightMapCell3>{public:
 float Height;unsigned char TextureBlendAlpha,EngineTheme[3],EngineBlend[2],Passable,CameraPassable;
};
class COldFileFormatHeightMapCell5:public CReplaceSerialise<COldFileFormatHeightMapCell5,COldFileFormatHeightMapCell4>{public:
 float Height;unsigned char TextureBlendAlpha,EngineTheme[3],EngineBlend[2],Passable,CameraPassable;
 long SoundLayer,MinimapZone;
 void ConvertFromOld(const COldFileFormatHeightMapCell4* old_thing){
  long count=0;
  Height=old_thing->Height;TextureBlendAlpha=old_thing->TextureBlendAlpha;
  count=3;
  for(long c1=0;c1<count;++c1)EngineTheme[c1]=old_thing->EngineTheme[c1];
  count=2;
  for(long c1=0;c1<count;++c1)EngineBlend[c1]=old_thing->EngineBlend[c1];
  Passable=old_thing->Passable;CameraPassable=old_thing->CameraPassable;
  SoundLayer=MinimapZone=0;
 }
};

// CReplaceSerialise::ConvertPreviousUnknownVersionUp -- upgrade a height-map cell to version5.
// Retail: 0x00826c90,288 bytes,static fastcall with hidden value return.
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatHeightMapCell5@@VCOldFileFormatHeightMapCell4@@@@SI?AV?$CCountedPointer@VCOldFileFormatHeightMapCell5@@@@PAX@Z
template<class T,class Old>
CCountedPointer<T> FABLE_FASTCALL CReplaceSerialise<T,Old>::ConvertPreviousUnknownVersionUp(void* old_void)
{
 CCountedPointer<T> new_thing(new T);
 CCountedPointer<Old> old_thing;
 Old* p_old_thing=(Old*)old_void;
 if(p_old_thing->SerialiseVersionNumber!=Old::GetClassVersionNumber()){
  old_thing=Old::ConvertPreviousUnknownVersionUp(old_void);
  p_old_thing=old_thing.Get();
 }
 new_thing->ConvertFromOld(p_old_thing);
 return new_thing;
}
template<> CCountedPointer<COldFileFormatHeightMapCell4> FABLE_FASTCALL
CReplaceSerialise<COldFileFormatHeightMapCell4,COldFileFormatHeightMapCell3>::ConvertPreviousUnknownVersionUp(void*);
template CCountedPointer<COldFileFormatHeightMapCell5> FABLE_FASTCALL
CReplaceSerialise<COldFileFormatHeightMapCell5,COldFileFormatHeightMapCell4>::ConvertPreviousUnknownVersionUp(void*);

static int calls;
static COldFileFormatHeightMapCell4 supplied;
template<> CCountedPointer<COldFileFormatHeightMapCell4> FABLE_FASTCALL
CReplaceSerialise<COldFileFormatHeightMapCell4,COldFileFormatHeightMapCell3>::ConvertPreviousUnknownVersionUp(void* data)
{
 assert(data!=0); ++calls;
 return CCountedPointer<COldFileFormatHeightMapCell4>(new COldFileFormatHeightMapCell4(supplied));
}
int main()
{
 memset(&supplied,0,sizeof supplied);
 supplied.SerialiseVersionNumber=4; supplied.Height=12.5f;
 supplied.TextureBlendAlpha=221;
 supplied.EngineTheme[0]=3;supplied.EngineTheme[1]=99;supplied.EngineTheme[2]=240;
 supplied.EngineBlend[0]=17;supplied.EngineBlend[1]=230;
 supplied.Passable=1;supplied.CameraPassable=2;
 for(int version=0;version<8;++version)
 {
  COldFileFormatHeightMapCell4 input=supplied;input.SerialiseVersionNumber=(unsigned char)version;
  input.Height=-4.5f;calls=0;
  CCountedPointer<COldFileFormatHeightMapCell5> result=
   CReplaceSerialise<COldFileFormatHeightMapCell5,COldFileFormatHeightMapCell4>::ConvertPreviousUnknownVersionUp(&input);
  assert(calls==(version==4?0:1));assert(result.Get()!=0 && result.Info!=0);
  assert(result->Height==(version==4?-4.5f:12.5f));
  assert(result->TextureBlendAlpha==221);
  for(int i=0;i<3;++i)assert(result->EngineTheme[i]==supplied.EngineTheme[i]);
  for(int j=0;j<2;++j)assert(result->EngineBlend[j]==supplied.EngineBlend[j]);
  assert(result->Passable==1 && result->CameraPassable==2);
  assert(result->SoundLayer==0 && result->MinimapZone==0);
  assert(result.GetReferenceCount()==1);
  {CCountedPointer<COldFileFormatHeightMapCell5> other(result);assert(result.GetReferenceCount()==2);}
  assert(result.GetReferenceCount()==1);
 }
 puts("00826c90_TEST PASS");return 0;
}