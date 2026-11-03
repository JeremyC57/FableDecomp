
${S}/pragma3/probes/sf/p2.obj:     file format pe-i386


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
   0:	sub    esp,0x10
   3:	push   ebx
   4:	push   esi
   5:	push   edi
   6:	xor    ebx,ebx
   8:	push   0x40
   a:	mov    edi,ecx
   c:	mov    DWORD PTR [esp+0x10],ebx
  10:	call   15 <?Init@CEnt@@QAEXXZ+0x15>
			11: DISP32	??2@YAPAXI@Z
  15:	mov    esi,eax
  17:	add    esp,0x4
  1a:	test   esi,esi
  1c:	je     c1 <?Init@CEnt@@QAEXXZ+0xc1>
  22:	mov    eax,DWORD PTR [edi+0x8]
  25:	mov    edi,DWORD PTR [edi+0xc]
  28:	test   edi,edi
  2a:	mov    ebx,0x1
  2f:	mov    DWORD PTR [esp+0x10],0x0
			33: dir32	??_7CThing@@6B@
  37:	mov    DWORD PTR [esp+0x14],eax
  3b:	mov    DWORD PTR [esp+0x18],edi
  3f:	je     43 <?Init@CEnt@@QAEXXZ+0x43>
  41:	inc    DWORD PTR [edi]
  43:	push   0x0
			44: dir32	??_C@_01FJMABOPO@x?$AA@
  48:	lea    ecx,[esp+0x10]
  4c:	call   51 <?Init@CEnt@@QAEXXZ+0x51>
			4d: DISP32	??0CName@@QAE@PBD@Z
  51:	xor    edi,edi
  53:	push   edi
  54:	push   eax
  55:	mov    ecx,esi
  57:	call   5c <?Init@CEnt@@QAEXXZ+0x5c>
			58: DISP32	??0CBase@@QAE@ABUCName@@K@Z
  5c:	mov    DWORD PTR [esi],0x0
			5e: dir32	??_7?$CFunc1T@UCThing@@@@6B@
  62:	mov    DWORD PTR [esi+0x34],0x0
			65: dir32	??_7CThing@@6B@
  69:	mov    ecx,DWORD PTR [esp+0x14]
  6d:	mov    DWORD PTR [esi+0x38],ecx
  70:	mov    edx,DWORD PTR [esp+0x18]
  74:	mov    DWORD PTR [esi+0x3c],edx
  77:	mov    eax,DWORD PTR [esp+0x18]
  7b:	cmp    eax,edi
  7d:	je     85 <?Init@CEnt@@QAEXXZ+0x85>
  7f:	inc    DWORD PTR [eax]
  81:	mov    eax,DWORD PTR [esp+0x18]
  85:	cmp    eax,edi
  87:	mov    DWORD PTR [esp+0x10],0x0
			8b: dir32	??_7CThing@@6B@
  8f:	je     ae <?Init@CEnt@@QAEXXZ+0xae>
  91:	dec    DWORD PTR [eax]
  93:	mov    eax,DWORD PTR [esp+0x18]
  97:	cmp    DWORD PTR [eax],edi
  99:	jne    ae <?Init@CEnt@@QAEXXZ+0xae>
  9b:	mov    ecx,DWORD PTR [eax+0x8]
  9e:	call   DWORD PTR [eax+0x4]
  a1:	mov    eax,DWORD PTR [esp+0x18]
  a5:	push   eax
  a6:	call   ab <?Init@CEnt@@QAEXXZ+0xab>
			a7: DISP32	??3@YAXPAX@Z
  ab:	add    esp,0x4
  ae:	lea    ecx,[esp+0x10]
  b2:	mov    DWORD PTR [esp+0x14],edi
  b6:	mov    DWORD PTR [esp+0x18],edi
  ba:	call   bf <?Init@CEnt@@QAEXXZ+0xbf>
			bb: DISP32	??1CBaseClass@@UAE@XZ
  bf:	jmp    c3 <?Init@CEnt@@QAEXXZ+0xc3>
  c1:	xor    esi,esi
  c3:	push   esi
  c4:	call   c9 <?Init@CEnt@@QAEXXZ+0xc9>
			c5: DISP32	?Add@@YAXPAUCBase@@@Z
  c9:	add    esp,0x4
  cc:	test   bl,0x1
  cf:	je     da <?Init@CEnt@@QAEXXZ+0xda>
  d1:	lea    ecx,[esp+0xc]
  d5:	call   da <?Init@CEnt@@QAEXXZ+0xda>
			d6: DISP32	??1CName@@QAE@XZ
  da:	pop    edi
  db:	pop    esi
  dc:	pop    ebx
  dd:	add    esp,0x10
  e0:	ret
