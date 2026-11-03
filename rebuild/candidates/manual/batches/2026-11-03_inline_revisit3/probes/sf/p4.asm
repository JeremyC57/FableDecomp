
${S}/pragma3/probes/sf/p4.obj:     file format pe-i386


Disassembly of section .text:

00000000 <??1CThing@@UAE@XZ>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	mov    eax,DWORD PTR [esi+0x8]
   6:	test   eax,eax
   8:	mov    DWORD PTR [esi],0x0
			a: dir32	??_7CThing@@6B@
   e:	je     2c <??1CThing@@UAE@XZ+0x2c>
  10:	dec    DWORD PTR [eax]
  12:	mov    eax,DWORD PTR [esi+0x8]
  15:	cmp    DWORD PTR [eax],0x0
  18:	jne    2c <??1CThing@@UAE@XZ+0x2c>
  1a:	mov    ecx,DWORD PTR [eax+0x8]
  1d:	call   DWORD PTR [eax+0x4]
  20:	mov    eax,DWORD PTR [esi+0x8]
  23:	push   eax
  24:	call   29 <??1CThing@@UAE@XZ+0x29>
			25: DISP32	??3@YAXPAX@Z
  29:	add    esp,0x4
  2c:	mov    DWORD PTR [esi+0x4],0x0
  33:	mov    DWORD PTR [esi+0x8],0x0
  3a:	mov    ecx,esi
  3c:	pop    esi
  3d:	jmp    42 <@feat.00+0x41>
			3e: DISP32	??1CBaseClass@@UAE@XZ

Disassembly of section .text:

00000000 <??_GCThing@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	mov    eax,DWORD PTR [esi+0x8]
   6:	test   eax,eax
   8:	mov    DWORD PTR [esi],0x0
			a: dir32	??_7CThing@@6B@
   e:	je     2c <??_GCThing@@UAEPAXI@Z+0x2c>
  10:	dec    DWORD PTR [eax]
  12:	mov    eax,DWORD PTR [esi+0x8]
  15:	cmp    DWORD PTR [eax],0x0
  18:	jne    2c <??_GCThing@@UAEPAXI@Z+0x2c>
  1a:	mov    ecx,DWORD PTR [eax+0x8]
  1d:	call   DWORD PTR [eax+0x4]
  20:	mov    eax,DWORD PTR [esi+0x8]
  23:	push   eax
  24:	call   29 <??_GCThing@@UAEPAXI@Z+0x29>
			25: DISP32	??3@YAXPAX@Z
  29:	add    esp,0x4
  2c:	mov    ecx,esi
  2e:	mov    DWORD PTR [esi+0x4],0x0
  35:	mov    DWORD PTR [esi+0x8],0x0
  3c:	call   41 <??_GCThing@@UAEPAXI@Z+0x41>
			3d: DISP32	??1CBaseClass@@UAE@XZ
  41:	test   BYTE PTR [esp+0x8],0x1
  46:	je     51 <??_GCThing@@UAEPAXI@Z+0x51>
  48:	push   esi
  49:	call   4e <??_GCThing@@UAEPAXI@Z+0x4e>
			4a: DISP32	??3@YAXPAX@Z
  4e:	add    esp,0x4
  51:	mov    eax,esi
  53:	pop    esi
  54:	ret    0x4

Disassembly of section .text:

00000000 <??0CBaseClass@@QAE@ABU0@@Z>:
   0:	mov    eax,ecx
   2:	mov    DWORD PTR [eax],0x0
			4: dir32	??_7CBaseClass@@6B@
   8:	ret    0x4

Disassembly of section .text:

00000000 <??_GCBaseClass@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	call   8 <??_GCBaseClass@@UAEPAXI@Z+0x8>
			4: DISP32	??1CBaseClass@@UAE@XZ
   8:	test   BYTE PTR [esp+0x8],0x1
   d:	je     18 <??_GCBaseClass@@UAEPAXI@Z+0x18>
   f:	push   esi
  10:	call   15 <??_GCBaseClass@@UAEPAXI@Z+0x15>
			11: DISP32	??3@YAXPAX@Z
  15:	add    esp,0x4
  18:	mov    eax,esi
  1a:	pop    esi
  1b:	ret    0x4

Disassembly of section .text:

00000000 <??1CPred@@QAE@XZ>:
   0:	mov    DWORD PTR [ecx],0x0
			2: dir32	??_7CPred@@6B@
   6:	ret

Disassembly of section .text:

00000000 <??1P@@UAE@XZ>:
   0:	push   esi
   1:	push   edi
   2:	mov    edi,ecx
   4:	lea    esi,[edi+0x4]
   7:	mov    DWORD PTR [edi],0x0
			9: dir32	??_7P@@6B@
   d:	mov    eax,DWORD PTR [esi+0x8]
  10:	test   eax,eax
  12:	mov    DWORD PTR [esi],0x0
			14: dir32	??_7CThing@@6B@
  18:	je     36 <??1P@@UAE@XZ+0x36>
  1a:	dec    DWORD PTR [eax]
  1c:	mov    eax,DWORD PTR [esi+0x8]
  1f:	cmp    DWORD PTR [eax],0x0
  22:	jne    36 <??1P@@UAE@XZ+0x36>
  24:	mov    ecx,DWORD PTR [eax+0x8]
  27:	call   DWORD PTR [eax+0x4]
  2a:	mov    eax,DWORD PTR [esi+0x8]
  2d:	push   eax
  2e:	call   33 <??1P@@UAE@XZ+0x33>
			2f: DISP32	??3@YAXPAX@Z
  33:	add    esp,0x4
  36:	mov    ecx,esi
  38:	mov    DWORD PTR [esi+0x4],0x0
  3f:	mov    DWORD PTR [esi+0x8],0x0
  46:	call   4b <??1P@@UAE@XZ+0x4b>
			47: DISP32	??1CBaseClass@@UAE@XZ
  4b:	mov    DWORD PTR [edi],0x0
			4d: dir32	??_7CPred@@6B@
  51:	pop    edi
  52:	pop    esi
  53:	ret

Disassembly of section .text:

00000000 <??_GP@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	call   8 <??_GP@@UAEPAXI@Z+0x8>
			4: DISP32	??1P@@UAE@XZ
   8:	test   BYTE PTR [esp+0x8],0x1
   d:	je     18 <??_GP@@UAEPAXI@Z+0x18>
   f:	push   esi
  10:	call   15 <??_GP@@UAEPAXI@Z+0x15>
			11: DISP32	??3@YAXPAX@Z
  15:	add    esp,0x4
  18:	mov    eax,esi
  1a:	pop    esi
  1b:	ret    0x4

Disassembly of section .text:

00000000 <??0CPred@@QAE@XZ>:
   0:	mov    eax,ecx
   2:	mov    DWORD PTR [eax],0x0
			4: dir32	??_7CPred@@6B@
   8:	ret

Disassembly of section .text:

00000000 <??0CThing@@QAE@ABU0@@Z>:
   0:	mov    eax,ecx
   2:	mov    ecx,DWORD PTR [esp+0x4]
   6:	mov    DWORD PTR [eax],0x0
			8: dir32	??_7CThing@@6B@
   c:	mov    edx,DWORD PTR [ecx+0x4]
   f:	mov    DWORD PTR [eax+0x4],edx
  12:	mov    ecx,DWORD PTR [ecx+0x8]
  15:	test   ecx,ecx
  17:	mov    DWORD PTR [eax+0x8],ecx
  1a:	je     1e <??0CThing@@QAE@ABU0@@Z+0x1e>
  1c:	inc    DWORD PTR [ecx]
  1e:	ret    0x4

Disassembly of section .text:

00000000 <??0P@@QAE@ABUCThing@@@Z>:
   0:	mov    eax,ecx
   2:	mov    ecx,DWORD PTR [esp+0x4]
   6:	mov    DWORD PTR [eax],0x0
			8: dir32	??_7P@@6B@
   c:	mov    DWORD PTR [eax+0x4],0x0
			f: dir32	??_7CThing@@6B@
  13:	mov    edx,DWORD PTR [ecx+0x4]
  16:	mov    DWORD PTR [eax+0x8],edx
  19:	mov    ecx,DWORD PTR [ecx+0x8]
  1c:	test   ecx,ecx
  1e:	mov    DWORD PTR [eax+0xc],ecx
  21:	je     25 <??0P@@QAE@ABUCThing@@@Z+0x25>
  23:	inc    DWORD PTR [ecx]
  25:	ret    0x4

Disassembly of section .text:

00000000 <?Main@E@@QAEXXZ>:
   0:	sub    esp,0x10
   3:	mov    eax,DWORD PTR [ecx+0x8]
   6:	mov    ecx,DWORD PTR [ecx+0xc]
   9:	test   ecx,ecx
   b:	mov    DWORD PTR [esp],0x0
			e: dir32	??_7P@@6B@
  12:	mov    DWORD PTR [esp+0x4],0x0
			16: dir32	??_7CThing@@6B@
  1a:	mov    DWORD PTR [esp+0x8],eax
  1e:	mov    DWORD PTR [esp+0xc],ecx
  22:	je     26 <?Main@E@@QAEXXZ+0x26>
  24:	inc    DWORD PTR [ecx]
  26:	lea    ecx,[esp]
  29:	push   ecx
  2a:	call   2f <?Main@E@@QAEXXZ+0x2f>
			2b: DISP32	?Set@@YAXABUCPred@@@Z
  2f:	add    esp,0x4
  32:	lea    ecx,[esp]
  35:	call   3a <?Main@E@@QAEXXZ+0x3a>
			36: DISP32	??1P@@UAE@XZ
  3a:	add    esp,0x10
  3d:	ret
