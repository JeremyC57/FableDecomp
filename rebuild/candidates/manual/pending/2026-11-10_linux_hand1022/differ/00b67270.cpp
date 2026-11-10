// CEngineLandscapeRenderer::BuildBlendingTables() -- build the five 128x128 landscape blend-falloff textures
// Grade: DIFFER(not graded) -- parked with 0 of 10 grades (session budget spent on the four matched rows and 0x00417747); decoded skeleton below
// Leaf: ?BuildBlendingTables@CEngineLandscapeRenderer@@AAEXXZ (spelling to confirm in the Ego_r name set before landing)
// Retail: Fable.exe 0x00B67270, 0x206 bytes to `ret` at 0x00B67475 (the catalog's 528 includes int3 padding); ebp frame, 0xB4 locals.
// Retail-asm (expected): 0x00B673C9..0x00B673E0 `fsub [ebp-4Ch] ; fistp dword ptr [ebp-18h] ; fild [ebp-18h] ; fadd [ebp-48h] ;
//   fstp [ebp-30h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-30h] ; jne ; inc dword ptr [ebp-18h]` (0.5f / 1.0f frame constants) -- the
//   shared engine/GFFloatToLongFloor.h-family inline-asm block (check which helper: fsub 0.5 / fadd 1 / inc is the floor variant).
// Decoded body: a texture format local (0x009E3790 (8,8,8,8,0) then 0x009E3800 -> bytes per pixel, kept in ebx);
//   for (i = 0; i < 5; ++i) { the i-th texture at this+0x5BC+i*8: create 128x128 (0x009FA280), lock (0x009FA450) -> a 16-byte
//   locked-rect copy; pitch and bits pointer saved per texture };
//   for (y = 0; y < 128; ++y) for (x = 0; x < 128; ++x) { C2DVector p((x - 64) * k, (y - 64) * k) (k at 0x0125B24C);
//     d = sqrt(max(1 - p.x*p.x - p.y*p.y, floor 0x0122DEDC)) (1.0f at 0x0122DED8); p.Normalise() (0x00A14440);
//     for (i = 0; i < 5; ++i) { v = this+0x10 ->0x00BE2B70(i, p) * 255? (0x01230014); byte b = round-floor(v);
//       memset(bits[i] + y*pitch + x*bpp, b, bpp) (inline rep stosd / stosb) } };
//   for (i = 0; i < 5; ++i) unlock (0x009F9DE0(0)).
// Header debt: CEngineLandscapeRenderer texture array (+0x5BC, 5 x 8 bytes), the +0x10 member whose 0x00BE2B70 returns the
// per-layer weight, and the texture create / lock / unlock spellings (CTexture? D3D wrapper) all need shared-header opt-ins.
