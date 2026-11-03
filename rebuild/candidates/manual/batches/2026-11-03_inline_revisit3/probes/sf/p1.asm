
${S}/pragma3/probes/sf/p1.obj:     file format pe-i386


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

00000000 <??_GCThing@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	call   8 <??_GCThing@@UAEPAXI@Z+0x8>
			4: DISP32	??1CThing@@UAE@XZ
   8:	test   BYTE PTR [esp+0x8],0x1
   d:	je     18 <??_GCThing@@UAEPAXI@Z+0x18>
   f:	push   esi
  10:	call   15 <??_GCThing@@UAEPAXI@Z+0x15>
			11: DISP32	??3@YAXPAX@Z
  15:	add    esp,0x4
  18:	mov    eax,esi
  1a:	pop    esi
  1b:	ret    0x4

Disassembly of section .text:

00000000 <??0?$CFunc1@UCScript@@UCThing@@@@QAE@ABUCName@@KPAUCScript@@P82@AEXUCThing@@@Z2@Z>:
   0:	mov    eax,DWORD PTR [esp+0x8]
   4:	push   esi
   5:	mov    esi,ecx
   7:	mov    ecx,DWORD PTR [esp+0x8]
   b:	push   eax
   c:	push   ecx
   d:	mov    ecx,esi
   f:	call   14 <??0?$CFunc1@UCScript@@UCThing@@@@QAE@ABUCName@@KPAUCScript@@P82@AEXUCThing@@@Z2@Z+0x14>
			10: DISP32	??0CBase@@QAE@ABUCName@@K@Z
  14:	mov    eax,DWORD PTR [esp+0x10]
  18:	mov    edx,DWORD PTR [esp+0x14]
  1c:	mov    ecx,DWORD PTR [esp+0x1c]
  20:	mov    DWORD PTR [esi+0x38],eax
  23:	mov    eax,DWORD PTR [esp+0x20]
  27:	test   eax,eax
  29:	mov    DWORD PTR [esi],0x0
			2b: dir32	??_7?$CFunc1@UCScript@@UCThing@@@@6B@
  2f:	mov    DWORD PTR [esi+0x34],edx
  32:	mov    DWORD PTR [esi+0x3c],0x0
			35: dir32	??_7CThing@@6B@
  39:	mov    DWORD PTR [esi+0x40],ecx
  3c:	mov    DWORD PTR [esi+0x44],eax
  3f:	je     43 <??0?$CFunc1@UCScript@@UCThing@@@@QAE@ABUCName@@KPAUCScript@@P82@AEXUCThing@@@Z2@Z+0x43>
  41:	inc    DWORD PTR [eax]
  43:	lea    ecx,[esp+0x18]
  47:	call   4c <??0?$CFunc1@UCScript@@UCThing@@@@QAE@ABUCName@@KPAUCScript@@P82@AEXUCThing@@@Z2@Z+0x4c>
			48: DISP32	??1CThing@@UAE@XZ
  4c:	mov    eax,esi
  4e:	pop    esi
  4f:	ret    0x1c

Disassembly of section .text:

00000000 <?Run@?$CFunc1@UCScript@@UCThing@@@@UAEXXZ>:
   0:	sub    esp,0xc
   3:	mov    eax,esp
   5:	mov    DWORD PTR [eax],0x0
			7: dir32	??_7CBaseClass@@6B@
   b:	mov    edx,ecx
   d:	mov    DWORD PTR [eax],0x0
			f: dir32	??_7CThing@@6B@
  13:	mov    ecx,DWORD PTR [edx+0x40]
  16:	mov    DWORD PTR [eax+0x4],ecx
  19:	mov    ecx,DWORD PTR [edx+0x44]
  1c:	test   ecx,ecx
  1e:	mov    DWORD PTR [eax+0x8],ecx
  21:	je     25 <?Run@?$CFunc1@UCScript@@UCThing@@@@UAEXXZ+0x25>
  23:	inc    DWORD PTR [ecx]
  25:	mov    ecx,DWORD PTR [edx+0x38]
  28:	call   DWORD PTR [edx+0x34]
  2b:	ret

Disassembly of section .text:

00000000 <??_G?$CFunc1@UCScript@@UCThing@@@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	lea    ecx,[esi+0x3c]
   6:	call   b <??_G?$CFunc1@UCScript@@UCThing@@@@UAEPAXI@Z+0xb>
			7: DISP32	??1CThing@@UAE@XZ
   b:	mov    ecx,esi
   d:	call   12 <??_G?$CFunc1@UCScript@@UCThing@@@@UAEPAXI@Z+0x12>
			e: DISP32	??1CBase@@UAE@XZ
  12:	test   BYTE PTR [esp+0x8],0x1
  17:	je     22 <??_G?$CFunc1@UCScript@@UCThing@@@@UAEPAXI@Z+0x22>
  19:	push   esi
  1a:	call   1f <??_G?$CFunc1@UCScript@@UCThing@@@@UAEPAXI@Z+0x1f>
			1b: DISP32	??3@YAXPAX@Z
  1f:	add    esp,0x4
  22:	mov    eax,esi
  24:	pop    esi
  25:	ret    0x4

Disassembly of section .text:

00000000 <?Init@CEnt@@QAEXXZ>:
   0:	sub    esp,0x10
   3:	push   ebx
   4:	push   ebp
   5:	push   esi
   6:	push   edi
   7:	xor    ebx,ebx
   9:	push   0x48
   b:	mov    edi,ecx
   d:	mov    DWORD PTR [esp+0x14],ebx
  11:	call   16 <?Init@CEnt@@QAEXXZ+0x16>
			12: DISP32	??2@YAPAXI@Z
  16:	mov    esi,eax
  18:	add    esp,0x4
  1b:	test   esi,esi
  1d:	je     95 <?Init@CEnt@@QAEXXZ+0x95>
  1f:	mov    eax,DWORD PTR [edi+0x8]
  22:	mov    DWORD PTR [esp+0x18],eax
  26:	mov    eax,DWORD PTR [edi+0xc]
  29:	test   eax,eax
  2b:	mov    ebx,0x1
  30:	mov    DWORD PTR [esp+0x14],0x0
			34: dir32	??_7CThing@@6B@
  38:	mov    DWORD PTR [esp+0x1c],eax
  3c:	je     40 <?Init@CEnt@@QAEXXZ+0x40>
  3e:	inc    DWORD PTR [eax]
  40:	mov    ebp,DWORD PTR [edi+0x10]
  43:	push   0x0
			44: dir32	??_C@_01FJMABOPO@x?$AA@
  48:	lea    ecx,[esp+0x14]
  4c:	call   51 <?Init@CEnt@@QAEXXZ+0x51>
			4d: DISP32	??0CName@@QAE@PBD@Z
  51:	push   0x0
  53:	push   eax
  54:	mov    ecx,esi
  56:	call   5b <?Init@CEnt@@QAEXXZ+0x5b>
			57: DISP32	??0CBase@@QAE@ABUCName@@K@Z
  5b:	mov    DWORD PTR [esi],0x0
			5d: dir32	??_7?$CFunc1@UCScript@@UCThing@@@@6B@
  61:	mov    DWORD PTR [esi+0x34],0x0
			64: dir32	?Watch@CScript@@QAEXUCThing@@@Z
  68:	mov    DWORD PTR [esi+0x38],ebp
  6b:	mov    DWORD PTR [esi+0x3c],0x0
			6e: dir32	??_7CThing@@6B@
  72:	mov    ecx,DWORD PTR [esp+0x18]
  76:	mov    DWORD PTR [esi+0x40],ecx
  79:	mov    edx,DWORD PTR [esp+0x1c]
  7d:	mov    DWORD PTR [esi+0x44],edx
  80:	mov    eax,DWORD PTR [esp+0x1c]
  84:	test   eax,eax
  86:	je     8a <?Init@CEnt@@QAEXXZ+0x8a>
  88:	inc    DWORD PTR [eax]
  8a:	lea    ecx,[esp+0x14]
  8e:	call   93 <?Init@CEnt@@QAEXXZ+0x93>
			8f: DISP32	??1CThing@@UAE@XZ
  93:	jmp    97 <?Init@CEnt@@QAEXXZ+0x97>
  95:	xor    esi,esi
  97:	mov    ecx,DWORD PTR [edi+0x10]
  9a:	push   esi
  9b:	call   a0 <?Init@CEnt@@QAEXXZ+0xa0>
			9c: DISP32	?Add@CScript@@QAEXPAUCBase@@@Z
  a0:	test   bl,0x1
  a3:	je     ae <?Init@CEnt@@QAEXXZ+0xae>
  a5:	lea    ecx,[esp+0x10]
  a9:	call   ae <?Init@CEnt@@QAEXXZ+0xae>
			aa: DISP32	??1CName@@QAE@XZ
  ae:	pop    edi
  af:	pop    esi
  b0:	pop    ebp
  b1:	pop    ebx
  b2:	add    esp,0x10
  b5:	ret
