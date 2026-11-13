## 0x00BB5040 CEngineClothRenderer::CEngineClothRenderer() -- RELOCATION_MATCH on the first grade (strict headers too)
- What made it match: the whole ctor written straight from OUTLINE/DOSSIER on the shared headers (CArray / CVertexShader / CIndexBuffer /
  CSimpleMemoryPool / CEngineResourceManager / CShaderResource / CDisplayManager), init list `IndexBufferSize(-1), PositionBufferPool(0,
  0x1000), NormalBufferPool(0, 0x1000)`, the four buffer zeroes as body statements, and the bank-0 vertex shader resource read ONCE
  into a local (`const CShaderResource* shaders = GPResourceManager->GetVShaderResource((EShaderBankNo)0)`). The 8-byte aligned frame
  (`and esp,-8`) came out by itself from plain source (DOSSIER hypothesis A: no helper needed). The global-reload variant (b.cpp) was not
  needed (fg stops at the first match).
- Headers / flags: no flags (no /EHsc). Defines FABLE_RETAIL_CENGINERESOURCEMANAGER / CCHARSTRING / CARRAY / CVERTEXSHADER / CINDEXBUFFER /
  CMOVABLERESOURCE / CSIMPLEMEMORYPOOL and the NEW overlay opt-in FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION (hdr/engine/CDisplayManager.h:
  `unsigned long GetPixelShaderVersion() const;` 0x009BEE00, placed outside the FABLE_RETAIL_CDISPLAYMANAGER block). Include
  engine/Direct3D9Types.h ahead of CDisplayManager.h (HWND / D3DCAPS9). CEngineClothRenderer has no shared header: class modelled in the row
  with the Ego_r member names (retail offsets, sizeof 0xF0).
- Name fix: 0x00A5D5F0 landed as `GetPixelShader` (free __fastcall) is `CShaderResource::GetVertexShader(const CCharString&) const`
  (engine/CShaderResource.h + landed 0x00BC14E0 / 0x00BB6E00 + VSHADER_* literals) -- NAME_FIXES.tsv.
- Global 0x01436E98 spelled `NEngineInternal::GPResourceManager` (landed 0x00BBCC70 / 0x00BADB40); 0x01436E1C `NEngineInternal::GPDisplayManager`.
