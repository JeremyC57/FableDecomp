
${S}/pragma3/probes/sf/p5.obj:     file format pe-i386


Disassembly of section .text:

00000000 <??0CPred@@QAE@XZ>:
   0:	mov    eax,ecx
   2:	mov    DWORD PTR [eax],0x0
			4: dir32	??_7CPred@@6B@
   8:	ret

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

00000000 <??1P@@QAE@XZ>:
   0:	push   esi
   1:	lea    esi,[ecx+0x4]
   4:	mov    DWORD PTR [ecx],0x0
			6: dir32	??_7P@@6B@
   a:	mov    eax,DWORD PTR [esi+0x8]
   d:	test   eax,eax
   f:	mov    DWORD PTR [esi],0x0
			11: dir32	??_7CThing@@6B@
  15:	je     33 <??1P@@QAE@XZ+0x33>
  17:	dec    DWORD PTR [eax]
  19:	mov    eax,DWORD PTR [esi+0x8]
  1c:	cmp    DWORD PTR [eax],0x0
  1f:	jne    33 <??1P@@QAE@XZ+0x33>
  21:	mov    ecx,DWORD PTR [eax+0x8]
  24:	call   DWORD PTR [eax+0x4]
  27:	mov    eax,DWORD PTR [esi+0x8]
  2a:	push   eax
  2b:	call   30 <??1P@@QAE@XZ+0x30>
			2c: DISP32	??3@YAXPAX@Z
  30:	add    esp,0x4
  33:	mov    DWORD PTR [esi+0x4],0x0
  3a:	mov    DWORD PTR [esi+0x8],0x0
  41:	mov    ecx,esi
  43:	pop    esi
  44:	jmp    49 <@feat.00+0x48>
			45: DISP32	??1CBaseClass@@UAE@XZ

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
   0:	sub    esp,0x1c
   3:	push   ebx
   4:	push   ebp
   5:	push   esi
   6:	mov    esi,ecx
   8:	mov    eax,DWORD PTR [esi+0x8]
   b:	mov    DWORD PTR [esp+0x20],eax
   f:	mov    eax,DWORD PTR [esi+0xc]
  12:	push   edi
  13:	xor    ebx,ebx
  15:	cmp    eax,ebx
  17:	mov    ebp,0x0
			18: dir32	??_7P@@6B@
  1c:	mov    edi,0x0
			1d: dir32	??_7CThing@@6B@
  21:	mov    DWORD PTR [esp+0x1c],ebp
  25:	mov    DWORD PTR [esp+0x20],edi
  29:	mov    DWORD PTR [esp+0x28],eax
  2d:	je     31 <?Main@E@@QAEXXZ+0x31>
  2f:	inc    DWORD PTR [eax]
  31:	lea    ecx,[esp+0x1c]
  35:	push   ecx
  36:	call   3b <?Main@E@@QAEXXZ+0x3b>
			37: DISP32	?Set@@YAXABUCPred@@@Z
  3b:	mov    eax,DWORD PTR [esp+0x2c]
  3f:	add    esp,0x4
  42:	cmp    eax,ebx
  44:	mov    DWORD PTR [esp+0x1c],ebp
  48:	mov    DWORD PTR [esp+0x20],edi
  4c:	je     6b <?Main@E@@QAEXXZ+0x6b>
  4e:	dec    DWORD PTR [eax]
  50:	mov    eax,DWORD PTR [esp+0x28]
  54:	cmp    DWORD PTR [eax],ebx
  56:	jne    6b <?Main@E@@QAEXXZ+0x6b>
  58:	mov    ecx,DWORD PTR [eax+0x8]
  5b:	call   DWORD PTR [eax+0x4]
  5e:	mov    edx,DWORD PTR [esp+0x28]
  62:	push   edx
  63:	call   68 <?Main@E@@QAEXXZ+0x68>
			64: DISP32	??3@YAXPAX@Z
  68:	add    esp,0x4
  6b:	lea    ecx,[esp+0x20]
  6f:	mov    DWORD PTR [esp+0x24],ebx
  73:	mov    DWORD PTR [esp+0x28],ebx
  77:	call   7c <?Main@E@@QAEXXZ+0x7c>
			78: DISP32	??1CBaseClass@@UAE@XZ
  7c:	mov    eax,DWORD PTR [esi+0x8]
  7f:	mov    DWORD PTR [esp+0x14],eax
  83:	mov    eax,DWORD PTR [esi+0xc]
  86:	cmp    eax,ebx
  88:	mov    DWORD PTR [esp+0x10],edi
  8c:	mov    DWORD PTR [esp+0x18],eax
  90:	je     98 <?Main@E@@QAEXXZ+0x98>
  92:	inc    DWORD PTR [eax]
  94:	mov    eax,DWORD PTR [esp+0x18]
  98:	cmp    eax,ebx
  9a:	mov    ecx,DWORD PTR [esp+0x14]
  9e:	mov    DWORD PTR [esp+0x1c],ebp
  a2:	mov    DWORD PTR [esp+0x20],edi
  a6:	mov    DWORD PTR [esp+0x24],ecx
  aa:	mov    DWORD PTR [esp+0x28],eax
  ae:	je     b2 <?Main@E@@QAEXXZ+0xb2>
  b0:	inc    DWORD PTR [eax]
  b2:	lea    edx,[esp+0x1c]
  b6:	push   edx
  b7:	call   bc <?Main@E@@QAEXXZ+0xbc>
			b8: DISP32	?Set@@YAXABUCPred@@@Z
  bc:	mov    eax,DWORD PTR [esp+0x2c]
  c0:	add    esp,0x4
  c3:	cmp    eax,ebx
  c5:	mov    DWORD PTR [esp+0x1c],ebp
  c9:	mov    DWORD PTR [esp+0x20],edi
  cd:	je     ec <?Main@E@@QAEXXZ+0xec>
  cf:	dec    DWORD PTR [eax]
  d1:	mov    eax,DWORD PTR [esp+0x28]
  d5:	cmp    DWORD PTR [eax],ebx
  d7:	jne    ec <?Main@E@@QAEXXZ+0xec>
  d9:	mov    ecx,DWORD PTR [eax+0x8]
  dc:	call   DWORD PTR [eax+0x4]
  df:	mov    eax,DWORD PTR [esp+0x28]
  e3:	push   eax
  e4:	call   e9 <?Main@E@@QAEXXZ+0xe9>
			e5: DISP32	??3@YAXPAX@Z
  e9:	add    esp,0x4
  ec:	lea    ecx,[esp+0x20]
  f0:	mov    DWORD PTR [esp+0x24],ebx
  f4:	mov    DWORD PTR [esp+0x28],ebx
  f8:	call   fd <?Main@E@@QAEXXZ+0xfd>
			f9: DISP32	??1CBaseClass@@UAE@XZ
  fd:	mov    eax,DWORD PTR [esp+0x18]
 101:	cmp    eax,ebx
 103:	mov    DWORD PTR [esp+0x10],edi
 107:	je     126 <?Main@E@@QAEXXZ+0x126>
 109:	dec    DWORD PTR [eax]
 10b:	mov    eax,DWORD PTR [esp+0x18]
 10f:	cmp    DWORD PTR [eax],ebx
 111:	jne    126 <?Main@E@@QAEXXZ+0x126>
 113:	mov    ecx,DWORD PTR [eax+0x8]
 116:	call   DWORD PTR [eax+0x4]
 119:	mov    ecx,DWORD PTR [esp+0x18]
 11d:	push   ecx
 11e:	call   123 <?Main@E@@QAEXXZ+0x123>
			11f: DISP32	??3@YAXPAX@Z
 123:	add    esp,0x4
 126:	lea    ecx,[esp+0x10]
 12a:	mov    DWORD PTR [esp+0x14],ebx
 12e:	mov    DWORD PTR [esp+0x18],ebx
 132:	call   137 <?Main@E@@QAEXXZ+0x137>
			133: DISP32	??1CBaseClass@@UAE@XZ
 137:	pop    edi
 138:	pop    esi
 139:	pop    ebp
 13a:	pop    ebx
 13b:	add    esp,0x1c
 13e:	ret
