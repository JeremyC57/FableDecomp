00b6edf0 CEngineWaterRenderer::CEngineWaterRenderer::GenerateWaterMesh DIFFER(3144v3144) flags= []
@@ retail 21-21 built 21-22
   00b6ee22  c1e204                 shl edx, 4
   00b6ee25  03d6                   add edx, esi
 + 00b6ee27  89542430               mov dword ptr [esp + 0x30], edx
   00b6ee27  c744241c00000000       mov dword ptr [esp + 0x1c], 0
   00b6ee2f  89542430               mov dword ptr [esp + 0x30], edx
@@ retail 22-23 built 23-23
   00b6ee25  03d6                   add edx, esi
   00b6ee27  c744241c00000000       mov dword ptr [esp + 0x1c], 0
 - 00b6ee2f  89542430               mov dword ptr [esp + 0x30], edx
   00b6ee33  eb0b                   jmp 0xb6ee40
   00b6ee35  8b7c244c               mov edi, dword ptr [esp + 0x4c]
@@ retail 894-894 built 894-895
   00b6f84f  83caff                 or edx, 0xffffffff
   00b6f852  89500c                 mov dword ptr [eax + 0xc], edx
 + 00b6f855  894818                 mov dword ptr [eax + 0x18], ecx
   00b6f855  89481c                 mov dword ptr [eax + 0x1c], ecx
   00b6f858  894818                 mov dword ptr [eax + 0x18], ecx
@@ retail 895-896 built 896-896
   00b6f852  89500c                 mov dword ptr [eax + 0xc], edx
   00b6f855  89481c                 mov dword ptr [eax + 0x1c], ecx
 - 00b6f858  894818                 mov dword ptr [eax + 0x18], ecx
   00b6f85b  894820                 mov dword ptr [eax + 0x20], ecx
   00b6f85e  894828                 mov dword ptr [eax + 0x28], ecx
@@ retail 927-927 built 927-929
   00b6f8c0  898890000000           mov dword ptr [eax + 0x90], ecx
   00b6f8c6  898898000000           mov dword ptr [eax + 0x98], ecx
 + 00b6f8cc  bdcdccccbd             mov ebp, 0xbdcccccd
 + 00b6f8d1  89a894000000           mov dword ptr [eax + 0x94], ebp
   00b6f8cc  8988a0000000           mov dword ptr [eax + 0xa0], ecx
   00b6f8d2  c780a4000000cdcccc3d   mov dword ptr [eax + 0xa4], 0x3dcccccd
@@ retail 930-932 built 932-932
   00b6f8d2  c780a4000000cdcccc3d   mov dword ptr [eax + 0xa4], 0x3dcccccd
   00b6f8dc  89909c000000           mov dword ptr [eax + 0x9c], edx
 - 00b6f8e2  bdcdccccbd             mov ebp, 0xbdcccccd
 - 00b6f8e7  89a894000000           mov dword ptr [eax + 0x94], ebp
   00b6f8ed  89b8a8000000           mov dword ptr [eax + 0xa8], edi
   00b6f8f3  89a8ac000000           mov dword ptr [eax + 0xac], ebp
@@ retail 962-962 built 962-965
   00b6f999  89981c010000           mov dword ptr [eax + 0x11c], ebx
   00b6f99f  899014010000           mov dword ptr [eax + 0x114], edx
 + 00b6f9a5  89902c010000           mov dword ptr [eax + 0x12c], edx
 + 00b6f9ab  898830010000           mov dword ptr [eax + 0x130], ecx
 + 00b6f9b1  898834010000           mov dword ptr [eax + 0x134], ecx
   00b6f9a5  898820010000           mov dword ptr [eax + 0x120], ecx
   00b6f9ab  898828010000           mov dword ptr [eax + 0x128], ecx
@@ retail 964-966 built 967-967
   00b6f9a5  898820010000           mov dword ptr [eax + 0x120], ecx
   00b6f9ab  898828010000           mov dword ptr [eax + 0x128], ecx
 - 00b6f9b1  898830010000           mov dword ptr [eax + 0x130], ecx
 - 00b6f9b7  898834010000           mov dword ptr [eax + 0x134], ecx
   00b6f9bd  89a824010000           mov dword ptr [eax + 0x124], ebp
   00b6f9c3  89902c010000           mov dword ptr [eax + 0x12c], edx
@@ retail 967-968 built 968-968
   00b6f9b7  898834010000           mov dword ptr [eax + 0x134], ecx
   00b6f9bd  89a824010000           mov dword ptr [eax + 0x124], ebp
 - 00b6f9c3  89902c010000           mov dword ptr [eax + 0x12c], edx
   00b6f9c9  898840010000           mov dword ptr [eax + 0x140], ecx
   00b6f9cf  89884c010000           mov dword ptr [eax + 0x14c], ecx
@@ retail 969-971 built 969-969
   00b6f9c3  89902c010000           mov dword ptr [eax + 0x12c], edx
   00b6f9c9  898840010000           mov dword ptr [eax + 0x140], ecx
 - 00b6f9cf  89884c010000           mov dword ptr [eax + 0x14c], ecx
 - 00b6f9d5  8bce                   mov ecx, esi
   00b6f9d7  89b838010000           mov dword ptr [eax + 0x138], edi
   00b6f9dd  89a83c010000           mov dword ptr [eax + 0x13c], ebp
@@ retail 973-973 built 971-972
   00b6f9d7  89b838010000           mov dword ptr [eax + 0x138], edi
   00b6f9dd  89a83c010000           mov dword ptr [eax + 0x13c], ebp
 + 00b6f9db  89884c010000           mov dword ptr [eax + 0x14c], ecx
   00b6f9e3  899848010000           mov dword ptr [eax + 0x148], ebx
   00b6f9e9  899044010000           mov dword ptr [eax + 0x144], edx
@@ retail 974-974 built 973-974
   00b6f9dd  89a83c010000           mov dword ptr [eax + 0x13c], ebp
   00b6f9e3  899848010000           mov dword ptr [eax + 0x148], ebx
 + 00b6f9e7  8bce                   mov ecx, esi
   00b6f9e9  899044010000           mov dword ptr [eax + 0x144], edx
   00b6f9ef  e8bc26f5ff             call 0xac20b0
DIFFLINES 18 retail_instr 1002 built_instr 1002
