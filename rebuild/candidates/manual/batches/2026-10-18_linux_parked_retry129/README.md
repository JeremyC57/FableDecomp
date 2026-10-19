# Asynchronous game-text group and text loading

009C80F0 CDataBankGroupEntry::BeginLoading and009C8340 CDataBankTextEntry::BeginLoading (296bytes each) use actual game-text bank/resource/string headers with genuine10-slot resource declarations and documented native52/68-byte resource layouts. Corrected propagated owner of second row and copied metadata comments.

Root review required the actual CCPPointerInfo free fastcall deletion callback, replacing a synthetic member-pointer callback. Native9D6100 allocates12bytes and stores RefCount1, callback9A42B0 at4, object at8;9A42B0 accepts ECX object and dispatches genuine scalar deleting destructor. Final grades3/2 remain RELOCATION_MATCH; all earlier variants/counts preserved.

Twenty-four body relocations independently verified, including async-entry/read methods, allocation/constructors, resource-bank/list methods, global priority1396314 and cleanup. Actual constructors independently inspected for counted-data retention. Native AddResource immediately dereferences the resource, so native allocation failure is not a recoverable target path; tests do not pretend otherwise.

Exact-source expanded-header host C++98 O2/no-elide and VC7.1 O2 fixtures pass. Five scenarios per row cover existing resource touch, new loading, callback-installed old-resource cleanup, null count-info branches, temporary reference release, retained async data and final resource destruction. Root source grades0.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
