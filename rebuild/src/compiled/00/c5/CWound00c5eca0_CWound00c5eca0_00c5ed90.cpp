// CSalOutputKS::CSalOutputKS() -- the GameCODA kernel-streaming output's constructor: the class vtable, the five members built inline
// (m_filter / m_pin handles -1, m_eventTerminate / m_threadNotify handles 0, m_mutexFrameCount's critical section initialised through the
// import), then the four body words zeroed
// Leaf: ??0CSalOutputKS@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5ED90, length 89 bytes (96 with padding).
// Name (NF404, joins the T3 nf401 0x00C5ECA0 unit; was the stand-in ??0CWound_00c5eca0@@QAE@XZ): reloc_bindings already binds
//   ??0CSalOutputKS@@QAE@XZ to 0x00C5ED90 (CSalPlatform::CreateSw 0x00C3F9D0, `new CSalOutputKS`), and the original GameCODA 2.10 SDK
//   CODAApi.lib Release/SalOutputKS.obj ??0CSalOutputKS@@QAE@XZ (80 B) is this body with the same vtable stores (CSalOutputKS, CSalFile,
//   CSalKsPin, CSalEvent, CSalThread), the same -1 / -1 / 0 / 0 handles and the same zeroing order +0x1c, +0x14, +0x18, +0x20; it calls
//   ??0CSalMutex out of line where retail expands it (InitializeCriticalSection, IAT 0x0143FEE4, under vtable 0x012B8FC8).
// Layout: the shared engine/CSalOutputKS.h with FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23 (declares this constructor) and the NF404 opt-in
//   FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS (the member constructors; with FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR, which it requires).
//   FABLE_RETAIL_CSALOUTPUTKS_KSPIN_DTOR_DECL_ONLY (scaffolding): the vtable store for m_pin makes this TU emit ??_GCSalKsPin; with
//   ~CSalKsPin left declared it is retail's 30-byte copy (0x00C5EC80, `call ??1CSalKsPin`) instead of the SDK 2.10 53-byte expansion.
//   The CSalKsPin store at +0xc is kept (its base CSalFile's is dead) and
//   the CSalOutput / CCodaObjectBase stores at +0 are dead under the CSalOutputKS one.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALOUTPUTKS_KSPIN_DTOR_DECL_ONLY
#include "engine/CSalOutputKS.h"

CSalOutputKS::CSalOutputKS()
{
    m_pBuffer = 0;
    m_pPacketHeaderTable = 0;
    m_pEventTable = 0;
    m_nFrames = 0;
}
