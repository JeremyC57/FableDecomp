#pragma optimize("s",on)
// std::vector<CSubtitleRenderer::CEmbeddedGraphic>::push_back(const CEmbeddedGraphic& x) -- STLport push_back: copy-construct at _M_finish when there
// is room (the implicit copy constructor out of line at 0x004935F8, size TU) and advance, else _M_insert_overflow (0x00493FF8) for one element at
// the end
// Leaf: ?push_back@?$vector@VCEmbeddedGraphic@CSubtitleRenderer@@V?$allocator@VCEmbeddedGraphic@CSubtitleRenderer@@@std@@@std@@QAEXABVCEmbeddedGraphic@CSubtitleRenderer@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049417D, 56 bytes, thiscall, `ret 4`.
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call 0x004935F8 ; S: add dword ptr [esi+4],0x10 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call 0x00493FF8 ; pop esi ; ret 4
// NF39 (2026-11-16): moved off its `CRetailVector_CElement16_004935f8::PushBack...` stand-in (a generated shape model) onto the shared
//   stlport/fable_containers.h vector and engine/CSubtitleRenderer.h (opt-in FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED: Ego_r.pdb
//   CSubtitleRenderer::CEmbeddedGraphic, 16 bytes, EmbeddedGraphics +0xB8). Identity: the one call at 0x0049243A (ProcessEmbeddedGraphicTags
//   0x00492381, already bound) pushes onto this+0xB8; the body is STLport push_back exactly (the third argument is the __false_type tag).
// Model: explicit instantiation emits the member as its own COMDAT, as retail kept every used inline member.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CSubtitleRenderer.h"

template void std::vector<CSubtitleRenderer::CEmbeddedGraphic>::push_back(const CSubtitleRenderer::CEmbeddedGraphic&);
