# NF35 header overlay

Pass this directory as `FABLE_HDR_OVERLAY` (it sits before `rebuild/include`).

| File | Change against rebuild/include | Opt-in macro | Used by |
|---|---|---|---|
| `engine/CWStringParser.h` | The repo header, unchanged, plus one opt-in block that declares `void Init(const unsigned short* text); // 0x00A0C8E0`. Ego_r.pdb names it `?Init@CWStringParser@@QAEXPB_W@Z`, and the catalog has `CWStringParser::Init` at that address. | `FABLE_RETAIL_CWSTRINGPARSER_INIT` | row 0x0048F50C (with `FABLE_RETAIL_CWSTRINGPARSER_READUNTIL`) |

To land: copy the `#ifdef FABLE_RETAIL_CWSTRINGPARSER_INIT` block into `rebuild/include/engine/CWStringParser.h`. The block is off by default, so rows that do not define the macro are unaffected.
