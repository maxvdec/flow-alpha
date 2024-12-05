" Vim syntax file for the Flow Computer Assembly language

if exists("b:current_syntax")
  finish
endif

syn keyword fcaCommands
    \ add sub mul div inc dec
    \ switch and or xor not
    \ shiftl shiftr mov push pop
    \ jmp jnz in out jz jgr jgl
    \ halt reset nothing

syn keyword fcaRegisters
    \ r0 r1 r2 r3 r4 r5 r6 r7
syn match fcaReferencedRegisters "&\s*\<\%(r0\|r1\|r2\|r3\|r4\|r5\|r6\|r7\)\>"

syn match fcaComment "%.*$"
syn match fcaLabel "^\s*[a-zA-Z_]\w*:"
syn match fcaNumber "\d\+"
syn match fcaMacro "@\w\+"
syn match fcaConstant "\<[A-Z_]\+\!"
syn match fcaHexNumber "0[xX][0-9a-fA-F]\+"
syn match fcaLabelCall "(\<[a-zA-Z_]\w*\>)"


" Strings
syn region fcaString start=/"/ skip=/\\"/ end=/"/

" Directives
syn match fcaDirective "!include\>"
syn match fcaDirective "!lang\>\s\+\S\+"
syn match fcaDirective "!origin\>"

" Macro arguments
syn match fcaMacroArg "\$\w\+"

" Directives
syn match fcaMacroCall "^\s*[a-zA-Z0-9_]\+$"

hi def link fcaCommands Keyword
hi def link fcaRegisters Type
hi def link fcaComment Comment
hi def link fcaLabel Label
hi def link fcaNumber Number
hi def link fcaMacro Keyword
hi def link fcaConstant Identifier
hi def link fcaHexNumber Number
hi def link fcaString String
hi def link fcaDirective PreProc
hi def link fcaReferencedRegisters Identifier
hi def link fcaOthers Keyword
hi def link fcaMacroArg Special
hi def link fcaMacroCall PreProc
hi def link fcaLabelCall Special

let b:current_syntax = "fca"
