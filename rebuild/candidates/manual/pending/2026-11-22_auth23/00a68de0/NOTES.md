# 0x00A68DE0 CVolumeTexture::LoadFromDataStream -- NOTES (AUTH23): escalated, best p4 DIFFER(911v919), 41 masked lines of a new kind
See ESCALATE.md (UPDATE section first). Source `p4_00a68de0.cpp` (p3 + the same-TU Lock body; p3 was 923v919), overlay `hdr/` (`hdr.diff`), stock flags.
## Names and evidence
| name | evidence |
|---|---|
| CVolumeTexture::LoadFromDataStream | Ego_r.pdb / FableWin.pdb `?LoadFromDataStream@CVolumeTexture@@QAEXAAVCDataInputStream@@@Z`; Ego_d.pdb proc (len 1624) |
| every local | Ego_d.pdb S_BPREL32 names / types of this function: file_info (CVolumeTextureFileInfo), pf (CPixelFormat), c1, locked_box (CVolumeLockedBox), hr, desc (_D3DVOLUME_DESC), overflow_check / overflow_check2 (unsigned __int64), line_length / data_length (unsigned long), src_buffer (std::vector<unsigned char>), src / dst (unsigned char*), cz, cy |
| CVolumeTexture::GetNoLevels / GetLevelByteLength | Ego_d.pdb TPI method list of CVolumeTexture (0xC8D5); FableWin.pdb `?GetLevelByteLength@CVolumeTexture@@QBEJJ@Z`; landed row 0x00A681F0; GetNoLevels is inline in every build (no proc), retail 0x00A68EB7 `call [ecx+34h]` |
| CVolumeTexture::GetPixelFormat / Unlock (row-local inline bodies) | the landed text of 0x00A68910 GetAsTextureList (same TU); FableWin.pdb `?Unlock@CVolumeTexture@@QAEXK@Z`, `?GetPixelFormat@CVolumeTexture@@QBE?AVCPixelFormat@@XZ` |
| CVolumeTextureFileInfo Width / Height / Depth / NoLevels / PixelFormatInit / UseageFlags / Pool | Ego_d.pdb TPI field list 0x7046A (sizeof 21); retail reads 0x15 bytes and uses +0x07 / +0x0D / +0x11 |
| CVolumeLockedBox Dimensions / RowPitch / SlicePitch / PMem | Ego_d.pdb TPI field list 0xBB59 (sizeof 24); the landed Lock row 0x00A685E0 |
| _D3DVOLUME_DESC, IDirect3DVolumeTexture9 (row-local, over shared IDirect3DBaseTexture9) | DirectX 9 SDK d3d9.h; the landed Lock row's text |
No descriptive names. Fitted, not Ego_d-sized: the cz / cy countdown loop headers (ESCALATE.md).
## Lane gotcha found
A missing `#include` makes VC7.1 cl.exe hang under wine on this lane (no C1083 output). That is why the recorded draft "hangs cl".
