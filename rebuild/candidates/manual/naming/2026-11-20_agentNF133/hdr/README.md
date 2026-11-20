# NF133 header overlays

None. Every row in rows/ grades on the repo headers as they stand (rebuild/include), with no opt-in added: the ports use existing
opt-ins only (FABLE_RETAIL_CRENDERSTATEMANAGER, FABLE_RETAIL_CRENDERMANAGERCORE, FABLE_RETAIL_C2DBOXF, FABLE_RETAIL_CDATAOUTPUTSTREAM,
FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL, FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL) and the unguarded engine/GFGetTime.h,
engine/IDirect3DDevice9.h, engine/CShaderRenderManager.h, engine/CRenderManager.h, engine/CPersistTraits.h declarations.
orig/ is empty for the same reason. No FABLE_STATIC_ASSERT was touched; pin_asserts.py was not needed.
