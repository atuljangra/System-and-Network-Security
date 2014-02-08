	.file	"target2.c"
	.text
.globl nstrcpy
	.type	nstrcpy, @function
nstrcpy:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	subl	$20, %esp
	movl	16(%ebp), %eax
	movl	$-1, %ecx
	movl	%eax, -24(%ebp)
	movb	$0, %al
	cld
	movl	-24(%ebp), %edi
	repnz
	scasb
	movl	%ecx, %eax
	notl	%eax
	decl	%eax
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	12(%ebp), %eax
	jle	.L2
	movl	12(%ebp), %eax
	movl	%eax, -8(%ebp)
.L2:
	movl	$0, -12(%ebp)
	jmp	.L4
.L5:
	movl	-12(%ebp), %eax
	movl	%eax, %edx
	addl	8(%ebp), %edx
	movl	-12(%ebp), %eax
	addl	16(%ebp), %eax
	movzbl	(%eax), %eax
	movb	%al, (%edx)
	incl	-12(%ebp)
.L4:
	movl	-12(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jle	.L5
	addl	$20, %esp
	popl	%edi
	popl	%ebp
	ret
	.size	nstrcpy, .-nstrcpy
.globl bar
	.type	bar, @function
bar:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$220, %esp
	movl	8(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$200, 4(%esp)
	leal	-200(%ebp), %eax
	movl	%eax, (%esp)
	call	nstrcpy
	leave
	ret
	.size	bar, .-bar
.globl foo
	.type	foo, @function
foo:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	bar
	leave
	ret
	.size	foo, .-foo
	.section	.rodata
.LC0:
	.string	"target2: argc != 2\n"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$20, %esp
	movl	%ecx, -8(%ebp)
	movl	-8(%ebp), %eax
	cmpl	$2, (%eax)
	je	.L13
	movl	stderr, %eax
	movl	%eax, 12(%esp)
	movl	$19, 8(%esp)
	movl	$1, 4(%esp)
	movl	$.LC0, (%esp)
	call	fwrite
	movl	$1, (%esp)
	call	exit
.L13:
	movl	-8(%ebp), %edx
	movl	4(%edx), %eax
	movl	%eax, (%esp)
	call	foo
	movl	$0, %eax
	addl	$20, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2 20061115 (prerelease) (Debian 4.1.1-21)"
	.section	.note.GNU-stack,"",@progbits
