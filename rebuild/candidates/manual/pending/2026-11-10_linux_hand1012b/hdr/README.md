# Header overlay, chunk 1012b

Every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported. gall.sh exports it too.

- `engine/CGameEventDataPackage.h`: copied from rebuild/include (HEAD). New opt-in `FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPEDREADERS`
  (inside `FABLE_RETAIL_CGAMEEVENTDATAPACKAGE`) adds the six FableWin.pdb typed readers as inline members:
  - `?GetULONG@CGameEventDataPackage@@QAEKXZ`
  - `?GetUBYTE@...@@QAEEXZ`
  - `?GetFloat@...@@QAEMXZ`
  - `?GetSLONG@...@@QAEJXZ`
  - `?GetUHUGE@...@@QAE_KXZ`
  - `?GetUWORD@...@@QAEGXZ`

  Each body is `T value; GetData(sizeof(value), &value); return value;`. None of them is in Ego_r.pdb or in Fable.exe, so they are
  inline in retail. UHUGE is a `CGameEventDataPackageUHUGE` typedef: `unsigned __int64` under `_MSC_VER`, `unsigned long long`
  elsewhere.

  The default path is byte-identical. Only differ/0062bd80.cpp uses the opt-in (it took that row's masked diff from 66 to 40 lines).
  The two matched rows (src/) do not need the overlay.
