# w24C header overlay (mirrors rebuild/include)

Rows 0x00C44090 / 0x00C44100 (CCodaSource::EAXSet / EAXGet). Both changes are additions inside one new opt-in,
`FABLE_RETAIL_CCODASOURCE_EAX`; the default path and every existing opt-in path are token-identical (no line removed, no
`FABLE_STATIC_ASSERT` / `__LINE__` user in either file).

- `engine/CCodaSource.h`: forward declaration `struct _GUID;` and, after `virtual void Release();`, the rest of the native source vtable
  0x012B5A88 -- slot 5 `virtual float GetEffectiveGain() const = 0` (2D 0x00C445B0, 3D 0x00C412D0 landed as GetEffectiveGain),
  slot 6 `SetLPFilter(const _CLPFILTER*)` 0x00C43780, slot 7 `SetEQFilter(const _CEQFILTER*)` 0x00C437A0, slot 8
  `virtual int AllocateVoice(CSalBuffer*, CSalVoice**) = 0` (2D 0x00C448A0, 3D 0x00C418C0) -- and the two non-virtual members
  `EAXSet` / `EAXGet`. The opt-in needs `FABLE_RETAIL_PASS22_CCODA3D_CREATE` (it supplies slots 0..4 and the dtor-only CCodaObject).
  Slot and member names are descriptive (the SDK symbol list for CCodaSource is not on this machine); the voice-side names
  EAXSet / EAXGet are the SDK CodeView ones already in `engine/CSalVoice.h`.
- `engine/CCodaBufferQueue.h`: `CSalBuffer* GetPlayBuffer() const` (the buffer at m_nPlay, null for an uncreated queue). Descriptive.
