
${S}/pragma3/probes/sf/p3.obj:     file format pe-i386


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

00000000 <??0?$CFunc1T@UCThing@@@@QAE@ABUCName@@UCThing@@@Z>:
   0:	mov    eax,DWORD PTR [esp+0x4]
   4:	push   esi
   5:	push   0x0
   7:	push   eax
   8:	mov    esi,ecx
   a:	call   f <??0?$CFunc1T@UCThing@@@@QAE@ABUCName@@UCThing@@@Z+0xf>
			b: DISP32	??0CBase@@QAE@ABUCName@@K@Z
   f:	mov    eax,DWORD PTR [esp+0x14]
  13:	test   eax,eax
  15:	mov    edx,DWORD PTR [esp+0x10]
  19:	mov    DWORD PTR [esi],0x0
			1b: dir32	??_7?$CFunc1T@UCThing@@@@6B@
  1f:	mov    ecx,0x0
			20: dir32	??_7CThing@@6B@
  24:	mov    DWORD PTR [esi+0x34],ecx
  27:	mov    DWORD PTR [esi+0x38],edx
  2a:	mov    DWORD PTR [esi+0x3c],eax
  2d:	je     35 <??0?$CFunc1T@UCThing@@@@QAE@ABUCName@@UCThing@@@Z+0x35>
  2f:	inc    DWORD PTR [eax]
  31:	mov    eax,DWORD PTR [esp+0x14]
  35:	test   eax,eax
  37:	mov    DWORD PTR [esp+0xc],ecx
  3b:	je     5b <??0?$CFunc1T@UCThing@@@@QAE@ABUCName@@UCThing@@@Z+0x5b>
  3d:	dec    DWORD PTR [eax]
  3f:	mov    eax,DWORD PTR [esp+0x14]
  43:	cmp    DWORD PTR [eax],0x0
  46:	jne    5b <??0?$CFunc1T@UCThing@@@@QAE@ABUCName@@UCThing@@@Z+0x5b>
  48:	mov    ecx,DWORD PTR [eax+0x8]
  4b:	call   DWORD PTR [eax+0x4]
  4e:	mov    eax,DWORD PTR [esp+0x14]
  52:	push   eax
  53:	call   58 <??0?$CFunc1T@UCThing@@@@QAE@ABUCName@@UCThing@@@Z+0x58>
			54: DISP32	??3@YAXPAX@Z
  58:	add    esp,0x4
  5b:	lea    ecx,[esp+0xc]
  5f:	mov    DWORD PTR [esp+0x10],0x0
  67:	mov    DWORD PTR [esp+0x14],0x0
  6f:	call   74 <??0?$CFunc1T@UCThing@@@@QAE@ABUCName@@UCThing@@@Z+0x74>
			70: DISP32	??1CBaseClass@@UAE@XZ
  74:	mov    eax,esi
  76:	pop    esi
  77:	ret    0x10

Disassembly of section .text:

00000000 <?Run@?$CFunc1T@UCThing@@@@UAEXXZ>:
   0:	ret

Disassembly of section .text:

00000000 <??_G?$CFunc1T@UCThing@@@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	lea    ecx,[esi+0x34]
   6:	call   b <??_G?$CFunc1T@UCThing@@@@UAEPAXI@Z+0xb>
			7: DISP32	??1CThing@@UAE@XZ
   b:	mov    ecx,esi
   d:	call   12 <??_G?$CFunc1T@UCThing@@@@UAEPAXI@Z+0x12>
			e: DISP32	??1CBase@@UAE@XZ
  12:	test   BYTE PTR [esp+0x8],0x1
  17:	je     22 <??_G?$CFunc1T@UCThing@@@@UAEPAXI@Z+0x22>
  19:	push   esi
  1a:	call   1f <??_G?$CFunc1T@UCThing@@@@UAEPAXI@Z+0x1f>
			1b: DISP32	??3@YAXPAX@Z
  1f:	add    esp,0x4
  22:	mov    eax,esi
  24:	pop    esi
  25:	ret    0x4

Disassembly of section .text:

00000000 <?Init@CEnt@@QAEXXZ>:
   0:	push   ecx
   1:	push   ebx
   2:	push   esi
   3:	push   edi
   4:	xor    ebx,ebx
   6:	push   0x40
   8:	mov    esi,ecx
   a:	mov    DWORD PTR [esp+0x10],ebx
   e:	call   13 <?Init@CEnt@@QAEXXZ+0x13>
			f: DISP32	??2@YAPAXI@Z
  13:	mov    edi,eax
  15:	add    esp,0x4
  18:	test   edi,edi
  1a:	je     5c <?Init@CEnt@@QAEXXZ+0x5c>
  1c:	sub    esp,0xc
  1f:	mov    eax,esp
  21:	mov    DWORD PTR [eax],0x0
			23: dir32	??_7CBaseClass@@6B@
  27:	mov    DWORD PTR [eax],0x0
			29: dir32	??_7CThing@@6B@
  2d:	mov    ecx,DWORD PTR [esi+0x8]
  30:	mov    DWORD PTR [eax+0x4],ecx
  33:	mov    ecx,DWORD PTR [esi+0xc]
  36:	test   ecx,ecx
  38:	mov    ebx,0x1
  3d:	mov    DWORD PTR [eax+0x8],ecx
  40:	je     44 <?Init@CEnt@@QAEXXZ+0x44>
  42:	inc    DWORD PTR [ecx]
  44:	push   0x0
			45: dir32	??_C@_01FJMABOPO@x?$AA@
  49:	lea    ecx,[esp+0x1c]
  4d:	call   52 <?Init@CEnt@@QAEXXZ+0x52>
			4e: DISP32	??0CName@@QAE@PBD@Z
  52:	push   eax
  53:	mov    ecx,edi
  55:	call   5a <?Init@CEnt@@QAEXXZ+0x5a>
			56: DISP32	??0?$CFunc1T@UCThing@@@@QAE@ABUCName@@UCThing@@@Z
  5a:	jmp    5e <?Init@CEnt@@QAEXXZ+0x5e>
  5c:	xor    eax,eax
  5e:	push   eax
  5f:	call   64 <?Init@CEnt@@QAEXXZ+0x64>
			60: DISP32	?Add@@YAXPAUCBase@@@Z
  64:	add    esp,0x4
  67:	test   bl,0x1
  6a:	je     75 <?Init@CEnt@@QAEXXZ+0x75>
  6c:	lea    ecx,[esp+0xc]
  70:	call   75 <?Init@CEnt@@QAEXXZ+0x75>
			71: DISP32	??1CName@@QAE@XZ
  75:	pop    edi
  76:	pop    esi
  77:	pop    ebx
  78:	pop    ecx
  79:	ret
