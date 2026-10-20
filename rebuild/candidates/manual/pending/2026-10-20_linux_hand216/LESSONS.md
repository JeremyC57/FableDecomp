# Chunk 216 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A never-read CCharString local that retail builds in a temporary, copy-constructs and then destroys is `CCharString x = Getter();` over an inline getter returning a NAMED local (`{ CCharString n("lit"); return n; }`; no NRVO) -- 0x00E8A1D0, 0x00EA5690, 0x00EC3AD0.
- Implicit-conversion CCharString temporaries in separate switch arms each get their own frame word; named locals in per-case blocks share one word under the RTM compiler (0x00EDAAD0).
- `CRGBColour(r, g, b, a)` with body stores R, G, B, A into the B,G,R,A layout reproduces retail's grouped 0xff / 0 byte stores for two colour temporaries (0x00E8AF70).
- `member = Game->GetThingWithScriptName("..")` = inline counted operator= + the temporary's inline dtor; a CScriptThing *named local* initialised from the same call is built in place and destroyed at the end (0x00EB6BD0, 0x00EF49A0, 0x00E15830).
- In a CTimer yield loop, `if (IsActiveThreadTerminating()) return;` before the final statement keeps the in-loop exit's own deregister tail; `if (!IsActiveThreadTerminating()) X();` merges them (0x00EB0700).
- `AddSpawnedFunction(new CSpawnedFunc<T>("Name", 0, this, &T::M), "Region")`: an implicit region literal is built before operator new; the thread name gets the ebx construction flag (0x00EA1630).
- STLport `vector::erase(pos)` over non-POD 8-byte records inlines the SGI `for (n = last - first; n > 0; --n)` copy (0x00AC7B00).
- A loop whose retail body walks a pointer (`add edi,4`) while counting an index is `for (i = 0; i < n; ++i, ++p)` over a by-value pointer parameter of a `__forceinline` helper; locals read before the outer loop stay in callee-saved registers (0x00AD8D10).
- Two-way choices of a call argument: an if/else of two complete calls tail-merges to `push imm` per arm; a ternary of two constants compiles branchless (`neg ; sbb ; and ; add`); a choice through a defaulted local gives `mov eax,imm ; je ; mov eax,imm` (0x00A90560).
- `const C3DVector& GetPosition()` copied into a local leaves X/Y on the x87 stack and copies Z through ecx; a by-value getter declaration does not (0x00860430, 0x00860ED0).
- Four complete `name = Dir() + name + L"ext";` statements under nested flag ifs reproduce the cross-jumped literal pushes; a ternary on the literal loads it into eax first (0x0049D550, 0x0049D6B0).
- A local fetched before a default-constructed vector local keeps the vector's zero stores after the call (`mgr = Get(); vector v; mgr->Fill(&v);`); in one expression the vector is built first (0x0049F3F0).
- An inline `ProcessEvent` that indexes a member-pointer table by the event type folds to a constant entry address in a loop that already tested `type == 1` (0x0049DFB0); the table member is an 8-byte `__multiple_inheritance` pointer to member padded to 16.
- A per-character `for` over a wide span is unrolled by four by the RTM compiler; retail's read-ahead (`c = text[++i]` at the loop end, reading the terminator) is a `while (i < length)` loop (0x00A0CA90).
- `local = Member = value;` chains store the member first and copy to the register, where `local = value; Member = local;` loads into the register first (0x00C351B0).
- `(min)(a, b)` of retail is `a < b ? a : b` (ties pick b); the 8-aligned frame for 8-byte CVolume temporaries is automatic (0x00A68130).
Review at landing:
- 0x00AD8D10 matched on its 4th grade (pointer-walk form), every parked row used 4 grades except 0x00CBC850 (1, known lane gap).
- Local models next to existing engine headers: CVillageTaskPatrol (0x00860950) and CVillageTaskSecuritySweep (0x00860430/0x00860ED0) need the intelligent-pointer types the headers keep opaque; CVirtualKeyboard (0x00851770) needs CWideString/CEditBox member types; CWorld (0x0049D550..0x0049F3F0) because engine/CWorld.h carries Ego_r.pdb offsets past +0xac (retail is 0xc lower: FrameCachedLODCentre +0xe8, CachedLODCentre +0xec).
- Name conflicts to sweep: 0x009E9F40 DisplayProgress is landed with two signatures (text, float, bool, bool) and (text, bool, float, bool) -- this batch uses the manifest one; 0x009E3830 is landed as CPixelFormat::Initialise and as a CPixelFormatFromD3D ctor; 0x00A0AA70 is landed as CRenderStatistics::RemoveTexture but CVolumeTexture::Lock (parked 0x00A685E0) calls it with 1.0f on lock, so the name is doubtful.
- 0x00C351B0's class is not CWeaponDrawState (audio sample playback record); 0x00EF49A0's owning quest is unproven.
- 0x00A0D1A0's row holds three functions (ReadAsString, ReadAsSymbol 0x00A0D200, a stub 0x00A0D260): split in the ledger.
